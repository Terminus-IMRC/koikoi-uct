#ifndef MAIN_H
#define MAIN_H

#include "fudaset.h"

	extern struct fudaset fs_my_tefuda, fs_my_aifuda, fs_op_aifuda, fs_field, fs_incognita;
	extern int op_tefuda_len;

	void save_all_states();
	void restore_all_states();

#endif /* MAIN_H */
