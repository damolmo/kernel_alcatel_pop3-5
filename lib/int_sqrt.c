/*
 * Copyright (C) 2013 Davidlohr Bueso <davidlohr.bueso@hp.com>
 *
 *  Based on the shift-and-subtract algorithm for computing integer
 *  square root from Guy L. Steele.
 */

#include <linux/kernel.h>
#include <linux/export.h>
<<<<<<< HEAD
=======
#include <linux/bitops.h>
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

/**
 * int_sqrt - rough approximation to sqrt
 * @x: integer of which to calculate the sqrt
 *
 * A very rough approximation to the sqrt() function.
 */
unsigned long int_sqrt(unsigned long x)
{
	unsigned long b, m, y = 0;

	if (x <= 1)
		return x;

<<<<<<< HEAD
	m = 1UL << (BITS_PER_LONG - 2);
=======
	m = 1UL << (__fls(x) & ~1UL);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	while (m != 0) {
		b = y + m;
		y >>= 1;

		if (x >= b) {
			x -= b;
			y += m;
		}
		m >>= 2;
	}

	return y;
}
EXPORT_SYMBOL(int_sqrt);
