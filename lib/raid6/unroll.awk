
# This filter requires one command line option of form -vN=n
# where n must be a decimal number.
#
# Repeat each input line containing $$ n times, replacing $$ with 0...n-1.
# Replace each $# with n, and each $* with a single $.

BEGIN {
	n = N + 0
}
{
	if (/\$\$/) { rep = n } else { rep = 1 }
	for (i = 0; i < rep; ++i) {
		tmp = $0
		gsub(/\$\$/, i, tmp)
<<<<<<< HEAD
		gsub(/\$\#/, n, tmp)
=======
		gsub(/\$#/, n, tmp)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		gsub(/\$\*/, "$", tmp)
		print tmp
	}
}
