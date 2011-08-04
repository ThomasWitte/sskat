#ifndef CH_GAME_DLG_H
#define CH_GAME_DLG_H

#include <allegro.h>
#include "main_dlg.h"
#include "game.h"

//typedef struct GameStruct Game;

extern DIALOG ch_game_dlg[];

void init_ch_game_dlg(Game *g);
void exit_ch_game_dlg();
int d_skat(int msg, DIALOG *d, int c);
int d_check_cb(int msg, DIALOG *d, int c);
int d_radio_cb(int msg, DIALOG *d, int c);
void game_ch_handler(DIALOG *d, Game *g);
void set_control(DIALOG *dlg, char *element, int nflag);

#endif
