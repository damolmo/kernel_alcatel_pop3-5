#!/usr/bin/awk -f
# extract linker version number from stdin and turn into single number
	{
<<<<<<< HEAD
	gsub(".*)", "");
=======
	gsub(".*\\)", "");
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	split($1,a, ".");
	print a[1]*10000000 + a[2]*100000 + a[3]*10000 + a[4]*100 + a[5];
	exit
	}
