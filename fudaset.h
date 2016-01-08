#ifndef FUDASET_H
#define FUDASET_H

#include <hanafuda.h>

	struct fudaset {
		LH_FUDA_T fuda[LH_MONTHS * LH_DAYS];
		int fuda_len;
		_Bool fuda_set[LH_MONTHS * LH_DAYS];
		int num_by_month[LH_MONTHS];
	};

	void fudaset_clean(struct fudaset *fs);
	void fudaset_add_fuda(struct fudaset *fs, const LH_FUDA_T fuda);
	LH_FUDA_T fudaset_del_fuda(struct fudaset *fs, const int elem);
	void fudaset_merge(struct fudaset *dest, const struct fudaset *src1, const struct fudaset *src2);

#endif /* FUDASET_H */
