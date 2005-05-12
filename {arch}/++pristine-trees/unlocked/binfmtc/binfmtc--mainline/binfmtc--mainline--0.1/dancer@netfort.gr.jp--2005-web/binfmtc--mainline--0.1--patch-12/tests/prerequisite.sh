# check for prerequisite status, such as binfmt_misc being enabled

set -e 

grep "^enabled$" /proc/sys/fs/binfmt_misc/ccompile
grep "^enabled$" /proc/sys/fs/binfmt_misc/cxxcompile
