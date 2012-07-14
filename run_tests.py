#!/usr/bin/python

import os
import os.path

red   = "\033[01;31m"
green = "\033[01;32m"
off   = "\033[0m"

SUCCESS = green  + "SUCCESS" + off + "\n"
FAILURE = red    + "FAILURE" + off + "\n"

def test_against_log(test_id, test_desc, prog, rc_ok=True):
    print "TEST {}: {}:".format(test_id, test_desc),

    run_log  = "test/test{}_output".format(test_id)
    good_log = run_log + "_good"
    rc = os.system("{} > {}".format(prog, run_log))

    with open(run_log, "r") as f:
        seen_output = f.read().strip()

    if not os.path.exists(good_log):   
        print FAILURE + " writing log for first time"
        with open(good_log, "w") as f:
            f.write(seen_output)
        return
 
    with open(good_log, "r") as f:
        correct_output = f.read().strip()
    
    if (rc == 0) != rc_ok:
        print FAILURE, " (rc was {})".format(rc)
    elif seen_output != correct_output:
        print "See {} vs. {} ".format(run_log, good_log) + FAILURE
    else:
        print SUCCESS
        os.unlink(run_log)
    

def test1():
    test_against_log(test_id=1,
                     test_desc="read noradi.zip", 
                     prog="bin/test_read_zip test/noradi.zip")

def test2():
    test_against_log(test_id=2,
                     test_desc="crack noradi.zip",
                     prog="bin/rlcrackzip test/noradi.zip")

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

print "\nRunning tests...\n"        
test1()
test2()
test3()
test4()
test5()

# That's it for now.
print ""
