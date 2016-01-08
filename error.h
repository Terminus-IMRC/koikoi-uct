#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>

#define error(str, ...) \
	do { \
		fflush(stdout); \
		fprintf(stderr, "%s:%d: error: "str, __FILE__, __LINE__, ## __VA_ARGS__); \
	} while (0)

#define error_and_exit(str, ...) \
	do { \
		error(str, ## __VA_ARGS__); \
		exit(EXIT_FAILURE); \
	} while (0)

#define UNREACHABLE error_and_exit("Unreachable!\n")

#endif /* ERROR_H */
