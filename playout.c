#include "playout.h"
#include <hanafuda.h>
#include "main.h"
#include "randrange.h"
#include "player.h"
#include "score.h"
#include "fudaset.h"
#include "error.h"

int select_randomly(const struct fudaset *fsp_from);
void take_randomly(struct fudaset *fsp_aifuda, struct fudaset *fsp_field, struct fudaset *fsp_tefuda, const int idx);
void take_manually(	struct fudaset *fsp_aifuda,
	struct fudaset *fsp_field, const LH_FUDA_T fuda_field_selected, const int fuda_field_selected_idx,
	struct fudaset *fsp_tefuda, const LH_FUDA_T fuda_tefuda_selected, const int fuda_tefuda_selected_idx);

int select_randomly(const struct fudaset *fsp_from)
{
	int r;

	r = randrange(0, fsp_from->fuda_len);

	return r;
}

void take_randomly(struct fudaset *fsp_aifuda, struct fudaset *fsp_field, struct fudaset *fsp_tefuda, const int idx)
{
	int i;
	LH_FUDA_T fuda_tefuda_selected;
	LH_FUDA_T fuda_field_selected;
	int fuda_field_selected_idx;
	int month, nbm, nth, count;

	fuda_tefuda_selected = fsp_tefuda->fuda[idx];
	month = lh_month(fuda_tefuda_selected);
	nbm = fsp_field->num_by_month[month];

	if (nbm == 0) {
		/* Just throw away the card from tefuda to the field */
		fudaset_add_fuda(fsp_field, fuda_tefuda_selected);
		fudaset_del_fuda(fsp_tefuda, idx);
		return;
	}

	nth = randrange(0, nbm);

	count = 0;
	fuda_field_selected_idx = -1;
	for (i = 0; i < fsp_field->fuda_len; i ++) {
		int month_cur = lh_month(fsp_field->fuda[i]);
		if (month_cur == month) {
			if (count == nth) {
				fuda_field_selected_idx = i;
				break;
			}
			count ++;
		}
	}
	if (fuda_field_selected_idx == -1)
		error_and_exit("fsp_field->num_by_month told a lie\n");

	fuda_field_selected = fsp_field->fuda[fuda_field_selected_idx];

	take_manually(fsp_aifuda, fsp_field, fuda_field_selected, fuda_field_selected_idx, fsp_tefuda, fuda_tefuda_selected, idx);
}

void take_manually(	struct fudaset *fsp_aifuda,
	struct fudaset *fsp_field, const LH_FUDA_T fuda_field_selected, const int fuda_field_selected_idx,
	struct fudaset *fsp_tefuda, const LH_FUDA_T fuda_tefuda_selected, const int fuda_tefuda_selected_idx)
{
	fudaset_add_fuda(fsp_aifuda, fuda_tefuda_selected);
	fudaset_del_fuda(fsp_tefuda, fuda_tefuda_selected_idx);

	fudaset_add_fuda(fsp_aifuda, fuda_field_selected);
	fudaset_del_fuda(fsp_field, fuda_field_selected_idx);
}

float playout(const enum player player_arg)
{
	int s;
	enum player player;

	switch (player_arg) {
		case PLAYER_ME:
		case PLAYER_OP:
			break;
		default:
			error_and_exit("invalid player_arg: %d\n", player_arg);
	}

	player = player_arg;

	while (
		(fs_my_tefuda.fuda_len != 0)
		|| (fs_incognita.fuda_len != ((LH_MONTHS * LH_DAYS) - 8 * 3 - 8 * 2))
	){
		switch (player) {
			case PLAYER_ME:
				/* ME: From my tefuda */
				s = select_randomly(&fs_my_tefuda);
				take_randomly(&fs_my_aifuda, &fs_field, &fs_my_tefuda, s);
				/* ME: From the deck */
				s = select_randomly(&fs_incognita);
				take_randomly(&fs_my_aifuda, &fs_field, &fs_incognita, s);

				break;

			case PLAYER_OP:
				/* OP: From OP tefuda */
				s = select_randomly(&fs_incognita);
				take_randomly(&fs_op_aifuda, &fs_field, &fs_incognita, s);
				/* OP: From the deck */
				s = select_randomly(&fs_incognita);
				take_randomly(&fs_op_aifuda, &fs_field, &fs_incognita, s);

				break;

			default:
				UNREACHABLE;
		}
		player = player_flip(player);
	}

	switch (player_arg) {
		case PLAYER_ME:
			return count_score(&fs_my_aifuda) - count_score(&fs_op_aifuda);
		case PLAYER_OP:
			return count_score(&fs_op_aifuda) - count_score(&fs_my_aifuda);
		default:
			UNREACHABLE;
	}

	UNREACHABLE;
}
