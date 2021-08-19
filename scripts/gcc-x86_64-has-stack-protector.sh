#!/bin/sh

<<<<<<< HEAD
echo "int foo(void) { char X[200]; return 3; }" | $* -S -x c -c -O0 -mcmodel=kernel -fstack-protector - -o - 2> /dev/null | grep -q "%gs"
=======
echo "int foo(void) { char X[200]; return 3; }" | $* -S -x c -c -O0 -mcmodel=kernel -fno-PIE -fstack-protector - -o - 2> /dev/null | grep -q "%gs"
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
if [ "$?" -eq "0" ] ; then
	echo y
else
	echo n
fi
