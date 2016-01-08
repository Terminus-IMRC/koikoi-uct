#include <stdio.h>
#include "fudaset.h"
#include "dbg.h"

void print_fs_fuda_info_core(const char *name, const struct fudaset fs)
{
	int i;

	printf("%s: %2d:", name, fs.fuda_len);
	for (i = 0; i < fs.fuda_len; i ++)
		printf(" %2d", fs.fuda[i]);
	printf("\n");
}
