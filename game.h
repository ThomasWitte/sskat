#ifndef GAME_H
#define GAME_H

#include <allegro.h>
#include "main_dlg.h"
#include "player.h"

enum {_FARBSP, _GRAND, _NULL, _RAMSCH};
enum {KREUZ, PIK, HERZ, KARO};
enum {ASS, KOENIG, DAME, BUBE, ZEHN, NEUN, ACHT, SIEBEN};
enum {KONTRA, RE, BOCK, HIRSCH};

struct GameStruct {
	int card[3];
	Player *p[3];
	int vorhand;
    int gvorh;
	int turn;
	int tricks_played;
	int gametype;
	int trumpf;
	int player;
	int skat[2];
	int geschoben;
	int schwarz;
	int null;
	int hand;
	int schwarza;
	int schneidera;
	int kontra;
	int reizwert;
	int stufe;
	int bockramsch;
};

extern Game game;

int calc_points(Game *g);
int calc_spielwert(Game *g);
int d_game_logic(int msg, DIALOG *d, int c);
int get_card(int col, int val); //Ex.: get_card(KREUZ, BUBE);
int get_val(int card);
int get_col(int card);
int higher(Game *g, int card1, int card2); //returns 1 if card1 is higher, -1 if card2 is higher, else 0
int length_of_color(Player *p, int farbe);
int blank(Player *p, int farbe);
int playing_possible(Game *g, Player *p, int card); //returns 1 if card can be played
int player_has_card(Player *p, int card);

#endif
