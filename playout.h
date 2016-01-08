#ifndef PLAYOUT_H
#define PLAYOUT_H

#include "player.h"
#include "fudaset.h"

	//void select_and_take_fuda_on_field(const int r, const int nf, const int n, struct fudaset *fs_from, struct fudaset *fs_aifuda);
	int select_randomly(const struct fudaset *fsp_from);
	void take_randomly(struct fudaset *fsp_aifuda, struct fudaset *fsp_field, struct fudaset *fsp_tefuda, const int idx);
	void take_manually(	struct fudaset *fsp_aifuda,
		struct fudaset *fsp_field, const LH_FUDA_T fuda_field_selected, const int fuda_field_selected_idx,
		struct fudaset *fsp_tefuda, const LH_FUDA_T fuda_tefuda_selected, const int fuda_tefuda_selected_idx);
	float playout(const enum player player_arg);

#endif /* PLAYOUT_H */
