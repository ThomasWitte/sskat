#include "player.h"

Player human, cpu1, cpu2;

void sort_hand(Game *g, Player *p) {
	for(int i = 1; i < p->hcardcount; ++i) {
		for(int j = i; j >= 1; j--) {
			switch(higher(g, p->hand[j], p->hand[j-1])) {
				case 0:
				{
					int card1 = p->hand[j];
					int card2 = p->hand[j-1];
					if(get_val(card1) == ASS) card1 -= 1;
					else if(get_val(card1) == ZEHN) card1 -= 4;
					if(get_val(card2) == ASS) card2 -= 1;
					else if(get_val(card2) == ZEHN) card2 -= 4;
					if(card1 > card2)
						break;
				}

				case 1:
				{
					int t = p->hand[j];
					p->hand[j] = p->hand[j-1];
					p->hand[j-1] = t;
				}
				break;
								
				case -1:
				j = 0;
				break;
			}
		}
	}
}

int d_player(int msg, DIALOG *d, int c) {
	Game *g = (Game*)d->dp2;
	Player *pl = (Player*)d->dp;
	switch(msg) {
		case MSG_START:
		case MSG_RESTART:
		if(!d->dp) return D_EXIT;
		pl->clicked_card = -1;
		pl->cur_game_points = 0;
		sort_hand(g, pl);
		break;
		
		case MSG_IDLE:
		if(pl->your_turn) {
			int cc = pl->choose_card(pl, g);
			if(cc >= 0) {
				--pl->hcardcount;
				g->card[g->turn] = pl->hand[cc];
				for(int i = cc; i < pl->hcardcount; ++i) {
					pl->hand[i] = pl->hand[i+1];
				}
				pl->your_turn = 0;
				return D_REDRAW;
			}
		}
		break;
	}
	return D_O_K;
}

int human_choose_card(Player *p, Game *g) {
	int ret = -1;
	if(p->clicked_card >= 0 && p->hcardcount > 0 && playing_possible(g, p, p->hand[p->clicked_card])) {
		ret = p->clicked_card;
	}
	p->clicked_card = -1;
	return ret;
}

void human_choose_game(Player *p, Game *g) {
	init_ch_game_dlg(g);
	buffered_do_dialog(ch_game_dlg, -1);
	exit_ch_game_dlg();
}

int human_reizen(Player *p, Game *g) {
	int grw = g->reizwert;
	init_reizen_dlg();
	buffered_do_dialog(reizen_dlg, -1);
	exit_reizen_dlg();
	int rw = g->reizwert;
	g->reizwert = grw;
	return rw;
}

void human_schieben(Player *p, Game *g) {
	init_ramsch_dlg(g);
	buffered_do_dialog(ramsch_dlg, -1);
	exit_ramsch_dlg();
	if(g->hand == 1)
		g->geschoben++;
}

void human_kontra(Player *p, Game *g) {
	init_show_game_dlg(g);
	int res = buffered_do_dialog(show_game_dlg, -1);
	if(res == 5)
		g->kontra++;
	exit_show_game_dlg();
}
