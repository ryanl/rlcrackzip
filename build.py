#!/usr/bin/python

# Automagical C/C++ project builder.
# Copyright (C) 2009-2012 Ryan J. Lothian
#-----------------------------------------------------------------------
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA 02110-1301, USA.
# -----------------------------------------------------------------------
"""
Just tell it the binaries you want, and the .cpp/.c file containing the
main functions, and it'll do the rest.

One more thing - for working out dependencies, this program needs to
know which calls are to libraries (so it won't find the source code to
those calls).

To get rid of build files, do rm .build/* in the same directory.
"""
# #################################################################### #
#                    You can edit this section.                        #
# #################################################################### #

import os

mypath = os.path.dirname(os.path.abspath( __file__ ))

from build_common import *

src_folder     = mypath + "/src/"
include_folder = mypath + "/include/"

targets = {
    "test_read_zip"     : src_folder + "test_read_zip.cpp",
    "test_generators"   : src_folder + "test_generators.cpp",
    "test_decode_check" : src_folder + "test_decode_check.cpp",
    "rlcrackzip"        : src_folder + "main.cpp",
}

compiler = ["g++", "-fopenmp", "-O2", "-flto", "-DNDEBUG", "-pipe", "-march=native", "-Wall"]
#compiler = ["g++", "-ggdb", "-Wall", "-march=native"]

libs = ["-lgcov"]

#-------------------------------------------------------------------------

import md5
m = md5.new()
m.update(" ".join(compiler))
compiler_hash = m.hexdigest()[:16] # used to group build files by compiler options

if len(sys.argv) == 1:
    chosen_targets = targets.keys()
elif len(sys.argv) == 2:
    chosen_targets = [ sys.argv[1] ]
else:
    print ("Usage: " + sys.argv[0] + " [target]")
    sys.exit(1)


print ("")
print ("Ryan's build tool!")
print ("================================================================")

run_build(targets=targets,
          chosen_targets=chosen_targets,
          compiler=compiler, 
          bin_suffix="",
          compiler_hash=compiler_hash,
          libs=libs)


