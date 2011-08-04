#ifndef MAIN_DLG
#define MAIN_DLG

#include <allegro.h>
#include "config.h"
#include "game.h"
#include "player.h"
#include "result_dlg.h"

#define MSG_TIMER MSG_USER
#define MSG_RESTART (MSG_USER+1)
#define D_RESTART D_USER

extern volatile int timer;
extern DIALOG main_dlg[];
extern DIALOG show_game_dlg[];
extern DIALOG ramsch_dlg[];
extern BITMAP *cards;

void init_ramsch_dlg(Game *g);
void exit_ramsch_dlg();
void init_show_game_dlg(Game *g);
void exit_show_game_dlg();
void draw_card(BITMAP *scr, BITMAP *cards, int x, int y, int w, int h, int card);

int d_bitmap(int msg, DIALOG *d, int c);
int d_human_hand(int msg, DIALOG *d, int c);
int d_npc_hand(int msg, DIALOG *d, int c);
int d_play_area(int msg, DIALOG *d, int c);
int d_timer(int msg, DIALOG *d, int c);
int d_ramsch_mgr(int msg, DIALOG *d, int c);

int buffered_do_dialog(DIALOG *d, int e);
#endif
