#define _BSD_SOURCE /* for logf(3) */
#include <math.h>
#include "main.h"
#include "randrange.h"
#include "player.h"
#include "fudaset.h"
#include "playout.h"
#include "error.h"

#define NODE_MAX 10000
#define K 1000
#define uct_loops 10000

struct child {
	int fuda_idx;
	int next;
	int num_searched;
	float score;
	float win_rate;
};

struct node {
	int num_child;
	int child_num_searched_sum;
	struct child child[8];
};

static struct node node[NODE_MAX];
static int node_num = 0;

static const int NODE_EMPTY = -1;

void add_child(struct node *np, const int fuda_idx)
{
	int num = np->num_child;

	np->child[num].fuda_idx = fuda_idx;
	np->child[num].next = NODE_EMPTY;
	np->child[num].num_searched = 0;
	np->child[num].score = 0.0;
	np->child[num].win_rate = 0.0;
	np->num_child ++;
}

int create_node()
{
	int i;
	struct node *np;

	if (node_num >= NODE_MAX)
		error_and_exit("node full use\n");

	np = &node[node_num];
	np->num_child = 0;
	np->child_num_searched_sum = 0;

	for (i = 0; i < fs_my_tefuda.fuda_len; i ++)
		add_child(np, i);

	node_num ++;

	return node_num - 1;
}

int select_best_ucb(const int node_n)
{
	int i;
	struct node *np = &node[node_n];
	int select = -1;
	float ucb, ucb_max = -9999;

	for (i = 0; i < np->num_child; i ++) {
		struct child *cp = &np->child[i];

		if (cp->num_searched == 0)
			ucb = 999999999 + randrange(0, 1000);
		else
			ucb = cp->win_rate + K * sqrt(2 * logf(np->child_num_searched_sum) / cp->num_searched);

		if (ucb > ucb_max) {
			ucb_max = ucb;
			select = i;
		}
	}
	if (select == -1)
		error_and_exit("invalid select (node_n = %d, np->num_child = %d)\n", node_n, np -> num_child);

	return select;
}

float search_uct(int node_n)
{
	struct node *np = &node[node_n];
	struct child *cp = NULL;
	int select, s;
	float score;

	select = select_best_ucb(node_n);
	cp = &np->child[select];

	/* ME: From my tefuda */
	take_randomly(&fs_my_aifuda, &fs_field, &fs_my_tefuda, cp->fuda_idx);
	/* ME: From the deck */
	s = select_randomly(&fs_incognita);
	take_randomly(&fs_my_aifuda, &fs_field, &fs_incognita, s);

	if (cp->num_searched <= 10)
		score = -playout(PLAYER_OP);
	else {
		/* OP: From OP tefuda */
		s = select_randomly(&fs_incognita);
		take_randomly(&fs_op_aifuda, &fs_field, &fs_incognita, s);
		/* OP: From the deck */
		s = select_randomly(&fs_incognita);
		take_randomly(&fs_op_aifuda, &fs_field, &fs_incognita, s);

		if (cp->next == NODE_EMPTY)
			cp->next = create_node();
		score = search_uct(cp->next);
	}

	cp->score += score;
	cp->num_searched ++;
	cp->win_rate = cp->score / cp->num_searched;
	np->child_num_searched_sum ++;

	return score;
}

int select_best_uct()
{
	int i, i_best, max;
	int next;
	struct node *np;

	node_num = 0;
	next = create_node();

	for (i = 0; i < uct_loops; i ++) {
		save_all_states();
		search_uct(next);
		restore_all_states();
	}

	i_best = -1;
	max = -1;
	np = &node[next];
	printf("Top nodes:\n");
	for (i = 0; i < np -> num_child; i ++) {
		struct child *cp = &np -> child[i];
		printf("i = %d, num_searched = %6d, score = %12.6f, win_rate = %9.6f\n", i, cp->num_searched, cp->score, cp->win_rate);
		if (cp -> num_searched > max) {
			i_best = i;
			max = cp -> num_searched;
		}
	}

	printf("%s:%d:%s: i_best = %d, max = %d\n", __FILE__, __LINE__, __func__, i_best, max);
	return np->child[i_best].fuda_idx;
}
