#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <hanafuda.h>
#include "fudaset.h"
#include "randrange.h"
#include "playout.h"
#include "uct.h"
#include "dbg.h"
#include "error.h"

char *progname = NULL;

struct fudaset fs_my_tefuda, fs_my_aifuda, fs_op_aifuda, fs_field, fs_incognita;
struct fudaset fs_my_tefuda_back, fs_my_aifuda_back, fs_op_aifuda_back, fs_field_back, fs_incognita_back;

static void main_mode_test();
static void init_newgame();

static void main_mode_interactive();
static void input_fuda(const char *name, struct fudaset *fsp);

void usage()
{
	printf("Usage: %s [mode]\n", progname);
	printf("mode:\n");
	printf("    0 = test\n");
	printf("    1 = interactive\n");
}

int main(int argc, char *argv[])
{
	int mode;

	progname = argv[0];

	if (argc != 2) {
		error("invalid argument\n");
		usage();
		return 1;
	}

	randrange_do_seed();

	mode = atoi(argv[1]);
	switch (mode) {
		case 0:
			main_mode_test();
			break;
		case 1:
			main_mode_interactive();
			break;
		default:
			error("invalid mode: %d\n", mode);
			return 1;
	}

	return 0;
}

void main_mode_test()
{
	int e;

	init_newgame();

	print_fs_fuda_info(fs_my_tefuda);
	print_fs_fuda_info(fs_field);
	e = select_best_uct();
	printf("e = %d\n", e);
	print_fs_fuda_info(fs_my_tefuda);
	print_fs_fuda_info(fs_field);
}

/* New Game! */
void init_newgame()
{
	int i;

	for (i = 0; i < LH_MONTHS * LH_DAYS; i ++)
		fs_incognita.fuda[i] = lh_int_to_fuda_t(i);
	fs_incognita.fuda_len = LH_MONTHS * LH_DAYS;
	for (i = 0; i < LH_MONTHS; i ++)
		fs_incognita.num_by_month[i] = LH_DAYS;

	/* Note: Temporary shuffle implementation. */
	/* Will use libhanafuda's communication system. */
	fudaset_clean(&fs_my_tefuda);
	for (i = 0; i < 8; i ++) {
		int r;
		LH_FUDA_T f;
		r = randrange(0, fs_incognita.fuda_len);
		f = fudaset_del_fuda(&fs_incognita, r);
		fudaset_add_fuda(&fs_my_tefuda, f);
	}

	fudaset_clean(&fs_field);
	for (i = 0; i < 8; i ++) {
		int r;
		LH_FUDA_T f;
		r = randrange(0, fs_incognita.fuda_len);
		f = fudaset_del_fuda(&fs_incognita, r);
		fudaset_add_fuda(&fs_field, f);
	}
}

void main_mode_interactive()
{
	int e;

	input_fuda("field", &fs_field);
	input_fuda("my_tefuda", &fs_my_tefuda);
	input_fuda("my_aifuda", &fs_my_aifuda);
	input_fuda("op_aifuda", &fs_op_aifuda);

	{
		int i;
		struct fudaset fs1, fs2, fs;
		_Bool used[LH_MONTHS * LH_DAYS];

		fudaset_clean(&fs1);
		fudaset_clean(&fs2);
		fudaset_clean(&fs);
		fudaset_merge(&fs1, &fs_field, &fs_my_tefuda);
		fudaset_merge(&fs2, &fs_my_aifuda, &fs_op_aifuda);
		fudaset_merge(&fs, &fs1, &fs2);

		memset(used, 0, (LH_MONTHS * LH_DAYS) * sizeof(_Bool));
		for (i = 0; i < fs.fuda_len; i ++)
			used[fs.fuda[i]] = 1;
		fudaset_clean(&fs_incognita);
		for (i = 0; i < LH_MONTHS * LH_DAYS; i ++)
			if (!used[i])
				fudaset_add_fuda(&fs_incognita, lh_int_to_fuda_t(i));
	}

	print_fs_fuda_info(fs_field);
	print_fs_fuda_info(fs_my_tefuda);
	print_fs_fuda_info(fs_my_aifuda);
	print_fs_fuda_info(fs_op_aifuda);
	print_fs_fuda_info(fs_incognita);

	e = select_best_uct();
	printf("e = %d\n", e);

	print_fs_fuda_info(fs_field);
	print_fs_fuda_info(fs_my_tefuda);
	print_fs_fuda_info(fs_my_aifuda);
	print_fs_fuda_info(fs_op_aifuda);
	print_fs_fuda_info(fs_incognita);
}

void input_fuda(const char *name, struct fudaset *fsp)
{
	char buf[0x10000];
	char *nptr, *endptr;
	int i;

	fudaset_clean(fsp);

	printf("%s? ", name);
	fgets(buf, sizeof(buf), stdin);

	i = 0;
	nptr = buf;
	endptr = NULL;
	for (; ; ) {
		long val;

		errno = 0;
		val = strtol(nptr, &endptr, 10);
		if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (errno != 0 && val == 0))
			error_and_exit("strtol: %s\n", strerror(errno));

		/* There are no digits at all. */
		if (endptr == nptr)
			break;

		if (i >= LH_MONTHS * LH_DAYS)
			error_and_exit("too many input fudas\n");

		if ((val < 0) || (val >= LH_MONTHS * LH_DAYS))
			error_and_exit("input fuda out-of-range: #%d: %ld\n", i, val);

		fudaset_add_fuda(fsp, (int) val);
		i ++;

		if (*endptr == '\0')
			break;

		nptr = endptr;
	}
}

void save_all_states()
{
	memcpy(&fs_my_tefuda_back, &fs_my_tefuda, sizeof(struct fudaset));
	memcpy(&fs_my_aifuda_back, &fs_my_aifuda, sizeof(struct fudaset));
	memcpy(&fs_op_aifuda_back, &fs_op_aifuda, sizeof(struct fudaset));
	memcpy(&fs_field_back, &fs_field, sizeof(struct fudaset));
	memcpy(&fs_incognita_back, &fs_incognita, sizeof(struct fudaset));
}

void restore_all_states()
{
	memcpy(&fs_my_tefuda, &fs_my_tefuda_back, sizeof(struct fudaset));
	memcpy(&fs_my_aifuda, &fs_my_aifuda_back, sizeof(struct fudaset));
	memcpy(&fs_op_aifuda, &fs_op_aifuda_back, sizeof(struct fudaset));
	memcpy(&fs_field, &fs_field_back, sizeof(struct fudaset));
	memcpy(&fs_incognita, &fs_incognita_back, sizeof(struct fudaset));
}
