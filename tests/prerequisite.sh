# check for prerequisite status, such as binfmt_misc being enabled.
# This test failing is a sign that some of the things are missing from
# the system configuration. Further tests will have more likelihood of
# failing.

set -e 

grep "^enabled$" /proc/sys/fs/binfmt_misc/ccompile
grep "^enabled$" /proc/sys/fs/binfmt_misc/cxxcompile

# check that some essential binaries exist, and some non-trivial pieces
which sudo

# check that sudo functions.
sudo true 
