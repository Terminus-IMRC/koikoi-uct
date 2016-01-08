#include "score.h"
#include <hanafuda.h>
#include "fudaset.h"

#define MAX2(a, b) ((((a)) > ((b)) ? ((a)) : ((b))))
#define MAX3(a, b, c) ((MAX2(((a)), MAX2(((b)), ((c))))))
#define MAX4(a, b, c, d) ((MAX2(MAX2(((a)), ((b))), MAX2(((c)), ((d))))))

float count_score(struct fudaset *fs_aifuda)
{
	float score_real = 0.0;
	_Bool ame = 0;

#define X(x) ((fs_aifuda -> fuda_set[LH_FUDA_ ## x]))

	if (X(11_1))
		ame = 1;

	/* n kou */
	{
		int count = 0;

		if (X(1_1))
			count ++;
		if (X(3_1))
			count ++;
		if (X(8_1))
			count ++;
		if (X(11_1))
			count ++;
		if (X(12_1))
			count ++;

		if (count == 5)
			score_real += 10;
		else if (ame) {
			if (count == 4)
				score_real += 7;
		} else {
			if (count == 4)
				score_real += 8;
			else if (count == 3)
				score_real += 5;
		}
	}

	/* hanami */
	{
		int count = 0;

		if (X(3_1))
			count ++;
		if (X(9_1))
			count ++;

		if (count == 2)
			score_real += 5;
	}

	/* tsukimi */
	{
		int count = 0;

		if (X(8_1))
			count ++;
		if (X(9_1))
			count ++;

		if (count == 2)
			score_real += 5;
	}

	/* inoshikacho */
	{
		int count = 0;

		if (X(7_1))
			count ++;
		if (X(10_1))
			count ++;
		if (X(6_1))
			count ++;

		if (count == 3)
			score_real += 5;
	}

	/* tans */
	{
		int count_red = 0, count_blue = 0, count_plain = 0;

		if (X(1_2))
			count_red ++;
		if (X(2_2))
			count_red ++;
		if (X(3_2))
			count_red ++;

		if (X(6_2))
			count_blue ++;
		if (X(9_2))
			count_blue ++;
		if (X(10_2))
			count_blue ++;

		if (X(4_2))
			count_plain ++;
		if (X(5_2))
			count_plain ++;
		if (X(7_2))
			count_plain ++;
		if (X(11_3))
			count_plain ++;

		if (count_red == 3 && count_blue == 3)
			score_real += 10 + count_plain;
		else if (count_red == 3)
			score_real += 5 + count_blue + count_plain;
		else if (count_blue == 3)
			score_real += 5 + count_red + count_plain;
		else if (count_red + count_blue + count_plain >= 5)
			score_real += 1 + (count_red + count_blue + count_plain - 5);
	}

	/* tane */
	{
		int count = 0;

		if (X(2_1))
			count ++;
		if (X(4_1))
			count ++;
		if (X(5_1))
			count ++;
		if (X(6_1))
			count ++;
		if (X(7_1))
			count ++;
		if (X(8_2))
			count ++;
		if (X(9_1))
			count ++;
		if (X(10_1))
			count ++;
		if (X(11_2))
			count ++;

		if (count >= 5)
			score_real += 1 + (count - 5);
	}

	/* kasu */
	{
		int count = 0;

		if (X(1_3))
			count ++;
		if (X(1_4))
			count ++;
		if (X(2_3))
			count ++;
		if (X(2_4))
			count ++;
		if (X(3_3))
			count ++;
		if (X(3_4))
			count ++;
		if (X(4_3))
			count ++;
		if (X(4_4))
			count ++;
		if (X(5_3))
			count ++;
		if (X(5_4))
			count ++;
		if (X(6_3))
			count ++;
		if (X(6_4))
			count ++;
		if (X(7_3))
			count ++;
		if (X(7_4))
			count ++;
		if (X(8_3))
			count ++;
		if (X(8_4))
			count ++;
		if (X(9_3))
			count ++;
		if (X(9_4))
			count ++;
		if (X(10_3))
			count ++;
		if (X(10_4))
			count ++;
		if (X(11_4))
			count ++;
		if (X(12_2))
			count ++;
		if (X(12_3))
			count ++;
		if (X(12_4))
			count ++;
		if (X(9_1))
			count ++;

		if (count >= 10)
			score_real += 1 + (count - 10);
	}

#undef X

	return score_real;
}
