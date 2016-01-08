#include "player.h"
#include "error.h"

enum player player_flip(const enum player player_arg)
{
	enum player ret = PLAYER_INVALID;

	switch (player_arg) {
		case PLAYER_ME:
			ret = PLAYER_OP;
			break;
		case PLAYER_OP:
			ret = PLAYER_ME;
			break;
		default:
			error_and_exit("invalid player: %d\n", player_arg);
	}

	return ret;
}
