# filename with space.
set -e 
mkdir tests || true
cat <<EOF > "tests/filename with space.c"
/*BINFMTC:
exit 1
 */
#include <stdio.h>

int main(int argc, char** argv)
{
  printf("Hello world\n");
  return 0;
}
EOF
chmod 700 "tests/filename with space.c"
"tests/filename with space.c"
rm "tests/filename with space.c" 
