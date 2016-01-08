#ifndef DBG_H
#define DBG_H

#define _STR(x) #x
#define STR(x) _STR(x)

#include "fudaset.h"

#define print_fs_fuda_info(fs) print_fs_fuda_info_core(STR(fs), fs)

	void print_fs_fuda_info_core(const char *name, const struct fudaset fs);

#endif /* DBG_H */
