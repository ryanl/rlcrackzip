#!/usr/bin/python

import sys
import os
import os.path
import time

red   = "\033[01;31m"
green = "\033[01;32m"
off   = "\033[0m"

SUCCESS = green  + "SUCCESS" + off + "\n"
FAILURE = red    + "FAILURE" + off + "\n"

def test_against_log(test_id, test_desc, prog, rc_ok=True, sortuniq=False, max_time=None):
    print "TEST {}: {}:".format(test_id, test_desc),
    sys.stdout.flush()

    run_log  = "test/test{}_output".format(test_id)
    good_log = run_log + "_good"

    if sortuniq:
        postfix = " | sort -u"
    else:
        postfix = ""

    start_time = time.time()
    rc = os.system("{} > {}{}".format(prog, run_log, postfix))
    run_time = time.time() - start_time

    if max_time is not None and run_time > max_time:
        print FAILURE + " max time {} exceeded: run time was {}".format(max_time, run_time)
        return False

    with open(run_log, "r") as f:
        seen_output = f.read().strip()

    if not os.path.exists(good_log):   
        print "log not present " + FAILURE
        with open(good_log, "w") as f:
            f.write(seen_output)
        return False
 
    with open(good_log, "r") as f:
        correct_output = f.read().strip()
    
    if (rc == 0) != rc_ok:
        print "(rc was {}) ".format(rc) + FAILURE
        return False
    elif seen_output != correct_output:
        print "See {} vs. {} ".format(run_log, good_log) + FAILURE
        return False
    else:
        print SUCCESS
        os.unlink(run_log)
        return True
    

def test1():
    test_against_log(test_id=1,
                     test_desc="read noradi.zip", 
                     prog="bin/test_read_zip test/noradi.zip")

def test2():
    test_against_log(test_id=2,
                     test_desc="crack noradi.zip",
                     prog="bin/rlcrackzip test/noradi.zip",
                     sortuniq=True)

def test3():
    test_against_log(test_id=3,
                     test_desc="wrong args, should print usage", 
                     prog="bin/rlcrackzip",
                     rc_ok=False)

def test4():
    test_against_log(test_id=4,
                     test_desc="generator test", 
                     prog="bin/test_generators")

def test5():
    test_against_log(test_id=5,
                     test_desc="decode check test", 
                     prog="bin/test_decode_check")

def test6():
    test_against_log(test_id=6,
                     test_desc="cipher implementation test", 
                     prog="bin/test_crack_zip")

def test7():
    test_against_log(test_id=7,
                     test_desc="read noradi.zip, wordlist mode", 
                     prog="echo 'noradi' | bin/test_read_zip test/noradi.zip -",
                     max_time=0.1)


def testC1():
    print "TEST c1: check all files have a copyright header",

    fmc_fn = "files_missing_copyright"
    os.system("find src/ -regextype posix-extended -regex '.*\.(h|cpp)' | xargs grep -L Copyright > " + fmc_fn)

    with open(fmc_fn, "r") as f:
        fmc = [l for l in f.read().strip().split("\n") if l]
    
    if fmc:
        print FAILURE + "\nThe following files are missing copyright headers:"
        print "\n".join("  * " + line for line in fmc)
    else:
        print SUCCESS
        os.unlink(fmc_fn)


print ""
print "Running tests..."
print "----------------"        

test1()
test2()
test3()
test4()
test5()
test6()
test7()

print "Running coding standards tests..."
print "---------------------------------"        
testC1()

# That's it for now.

