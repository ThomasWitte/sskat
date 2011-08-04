#ifndef KI_H
#define KI_H

#include "player.h"
#include "game.h"
#include "config.h"

void cpu_schieben(Player *p, Game *g);
void cpu_kontra(Player *p, Game *g);
int cpu_choose_card(Player *p, Game *g);
void cpu_choose_game(Player *p, Game *g);
int cpu_reizen(Player *p, Game *g);

#endif
