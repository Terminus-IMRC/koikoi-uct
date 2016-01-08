#define _BSD_SOURCE /* for random(3) */
#include <stdlib.h>
#include <sys/time.h>
#include "randrange.h"
#include "error.h"

void randrange_do_seed()
{
	struct timeval t;

	gettimeofday(&t, NULL);

	srandom(t.tv_usec);
}

int randrange(const int start, const int end)
{
	const int range = end - start;
	const int offset = start;

	if (range <= 0)
		error_and_exit("range is negative! (start = %d, end = %d)\n", start, end);

	return random() % range + offset;
}
