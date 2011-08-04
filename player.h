#ifndef PLAYER_H
#define PLAYER_H

#include <allegro.h>
#include "game.h"
#include "ch_game_dlg.h"
#include "reizen_dlg.h"
#include "ki.h"

struct PlayerStruct {
	int hand[12];
	int hcardcount;
	int clicked_card;
	int ouvert;
	int cur_game_points;
	int total_points;
	int your_turn;
	int (*choose_card)(Player*, Game*);
	void (*choose_game)(Player*, Game*);
	int (*reizen)(Player*, Game*);
	void (*schieben)(Player*, Game*);
	void (*kontra)(Player*, Game*);
	char *name;
	char *rtext;
};

extern Player human, cpu1, cpu2;

int d_player(int msg, DIALOG *d, int c);

void human_schieben(Player *p, Game *g);
void human_kontra(Player *p, Game *g);
int human_choose_card(Player *p, Game *g);
void human_choose_game(Player *p, Game *g);
int human_reizen(Player *p, Game *g);
void sort_hand(Game *g, Player *p);

#endif
