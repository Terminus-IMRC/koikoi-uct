#include "fudaset.h"

void fudaset_clean(struct fudaset *fs)
{
	int i;

	fs -> fuda_len = 0;
	for (i = 0; i < LH_MONTHS * LH_DAYS; i ++)
		fs -> fuda_set[i] = 0;
	for (i = 0; i < LH_MONTHS; i ++)
		fs -> num_by_month[i] = 0;
}

void fudaset_add_fuda(struct fudaset *fs, const LH_FUDA_T fuda)
{
	const int month = lh_month(fuda);

	fs -> fuda[fs -> fuda_len] = fuda;
	fs -> fuda_len ++;
	fs -> fuda_set[fuda] = 1;
	fs -> num_by_month[month] ++;
}

LH_FUDA_T fudaset_del_fuda(struct fudaset *fs, const int elem)
{
	const LH_FUDA_T fuda = fs -> fuda[elem];
	const int month = lh_month(fuda);

	fs -> fuda[elem] = fs -> fuda[fs -> fuda_len - 1];
	fs -> fuda_len --;
	fs -> fuda_set[fuda] = 0;
	fs -> num_by_month[month] --;

	return fuda;
}

void fudaset_merge(struct fudaset *dest, const struct fudaset *src1, const struct fudaset *src2)
{
	int i;

	for (i = 0; i < src1->fuda_len; i ++)
		fudaset_add_fuda(dest, src1->fuda[i]);
	for (i = 0; i < src2->fuda_len; i ++)
		fudaset_add_fuda(dest, src2->fuda[i]);
}
