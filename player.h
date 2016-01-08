#ifndef PLAYER_H
#define PLAYER_H

	enum player {
		PLAYER_INVALID,
		PLAYER_ME,
		PLAYER_OP
	};

	enum player player_flip(const enum player player_arg);

#endif /* PLAYER_H */
