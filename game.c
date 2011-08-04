#include "game.h"

Game game;

void game_init(Game *game) {
	for(int i = 0; i < 3; ++i) {
		game->card[i] = -1;
		game->p[i]->your_turn = 0;
		game->p[i]->ouvert = 0;
	}
	
	game->player = -1;
	game->gvorh = (game->gvorh+1)%3;
	game->vorhand = game->gvorh;
	game->turn = game->vorhand;
	game->tricks_played = 0;
	game->trumpf = -1;
	game->gametype = _GRAND;
	game->stufe = 0;
	game->geschoben = 0;
	game->schwarz = 1;
	game->null = 1;
	game->hand = 1;
	game->schwarza = 0;
	game->schneidera = 0;
	game->kontra = 0;

	//karten verteilen
	int dealt_cards[32];
	for(int i = 0; i < 32; i++)
		dealt_cards[i] = 0;
	for(int p = 0; p < 3; p++) {
		game->p[p]->hcardcount = 10;
		for(int i = 0; i < 10; i++) {
			int card = rand()%32;
			while(dealt_cards[card])
				card = rand()%32;
			dealt_cards[card] = 1;
			game->p[p]->hand[i] = card+1;
		}
	}
	for(int i = 0; i < 2; ++i)
		for(int j = 0; j < 32; ++j)
			if(dealt_cards[j] == 0) {
				game->skat[i] = j+1;
				dealt_cards[j] = 1;
				break;
			}

	//Reizen
	sort_hand(game, game->p[0]);
	game->reizwert = 0;
	for(int i = 4; i >= 2; --i) {
		int rw = game->p[(game->vorhand+i)%3]->reizen(game->p[(game->vorhand+i)%3], game);
		if(rw >= game->reizwert) {
			game->reizwert = rw;
			game->player = (game->vorhand+i)%3;
		}
	}
	game->turn = game->vorhand;
			
	//Spielstufe berechnen
	if(game->reizwert > 0) {
		game->stufe = 2;
		if(player_has_card(game->p[game->player], get_card(KREUZ, BUBE)) || game->skat[0] == get_card(KREUZ, BUBE) || game->skat[1] == get_card(KREUZ, BUBE))
			for(int i = 1; i < 4; i++)
				if(player_has_card(game->p[game->player], get_card(i, BUBE)) || game->skat[0] == get_card(i, BUBE) || game->skat[1] == get_card(i, BUBE))
					game->stufe++;
				else
					break;
		else
			for(int i = 1; i < 4; i++)
				if(!player_has_card(game->p[game->player], get_card(i, BUBE)) && game->skat[0] != get_card(i, BUBE) && game->skat[1] != get_card(i, BUBE))
					game->stufe++;
				else
					break;
		//Spiel wählen
		game->p[game->player]->choose_game(game->p[game->player], game);
	} else {
		game->player = -1;
		game->gametype = _RAMSCH;
		for(int i = 0; i < 3; i++)
			game->p[(game->vorhand+i)%3]->schieben(game->p[(game->vorhand+i)%3], game);
	}

	//spieltyp anzeigen:
	int k = 0;
	do {
	k = game->kontra;
	for(int i = 2; i >= 0; --i)
		game->p[i]->kontra(game->p[i], game);
	} while(k != game->kontra);

	for(int i = 0; i < 3; ++i) {
		game->card[i] = -1;
		game->p[i]->your_turn = 0;
	}

	game->p[game->vorhand]->your_turn = 1;
}

int get_trick_winner(Game *g) {
	int winner = g->vorhand;
	for(int i = 1; i < 3; i++) {
		if(higher(g, g->card[(g->vorhand+i)%3], g->card[winner]) == 1)
			winner = (g->vorhand+i)%3;
	}
	return winner;
}

int get_trick_points(Game *game) {
	int values[] = {11, 4, 3, 2, 10, 0, 0, 0};
	return values[get_val(game->card[0])]
		+  values[get_val(game->card[1])]
		+  values[get_val(game->card[2])];
}

int next_trick(Game *game) {
	game->tricks_played++;
	game->vorhand = get_trick_winner(game);
	if(game->vorhand != game->player)
		game->schwarz = 0;
	else {
		game->null = 0;
		if(game->gametype == _NULL)
			game->tricks_played = 10;
	}
	game->p[game->vorhand]->cur_game_points += get_trick_points(game);

	for(int i = 0; i < 3; ++i) {
		game->card[i] = -1;
	}

	if(game->tricks_played < 10) {
		game->p[game->vorhand]->your_turn = 1;
		game->turn = game->vorhand;
	} else {
		init_result_dlg(game);
		buffered_do_dialog(result_dlg, -1);
		exit_result_dlg();
		return 2;
	}
	return 1;
}

int set_update_timer(Game *game) {
	if(game->card[game->turn] > 0) {
		return 1;
	}
	return 0;
}

int update_game(Game *game) {
	game->turn = (game->turn+1)%3;
	if(game->turn == game->vorhand) {
		return next_trick(game);
	} else {
		game->p[game->turn]->your_turn = 1;
	}
	return 0;
}

int d_game_logic(int msg, DIALOG *d, int c) {
	switch(msg) {
		case MSG_START:
		((Game*)d->dp)->gvorh = 2;
		((Game*)d->dp)->bockramsch = 0;
		case MSG_RESTART:
		game_init((Game*)d->dp);
		d->fg = 0;
		break;

		case MSG_IDLE:
		if(set_update_timer((Game*)d->dp) && d->fg == 0) {
			d->fg = 5;
		}
		break;
		
		case MSG_TIMER:
		if(d->fg > 0)
			--d->fg;
		if(d->fg == 1) {
			switch(update_game((Game*)d->dp)) {
				case 1:
				return D_REDRAW;
				
				case 2:
				return D_RESTART;
			}
		}
		break;
	}
	return D_O_K;
}

int get_card(int col, int val) {
	return 1 + 8*col + val; 
}

int get_col(int card) {
	return (card-1)/8;
}

int get_val(int card) {
	return (card-1)%8;
}

int higher(Game *g, int card1, int card2) {
	switch(g->gametype) {
		case _NULL:
		if(get_col(card1) == get_col(card2)) {
			if(card1 > card2) return -1;
			else return 1;
		}
		break;
		
		case _FARBSP:
		case _GRAND:
		case _RAMSCH:
		//Buben prüfen
		if(get_val(card1) == BUBE) {
			if(get_val(card2) == BUBE) {
				if(card1 < card2) return 1;
				return -1;
			}
			return 1;
		}
		if(get_val(card2) == BUBE)
			return -1;
		//Trumpf prüfen
		if(get_col(card1) != get_col(card2)) {
			if(get_col(card1) == g->trumpf)
				return 1;
			if(get_col(card2) == g->trumpf)
				return -1;
		}
		//Farbe prüfen
		else {
			if(get_val(card1) == ASS) card1 -= 1;
			else if(get_val(card1) == ZEHN) card1 -= 4;
			if(get_val(card2) == ASS) card2 -= 1;
			else if(get_val(card2) == ZEHN) card2 -= 4;
			if(card1 < card2) return 1;
			return -1;
		}
		break;
	}
	return 0;
}

int playing_possible(Game *g, Player *p, int card) {
	if(p == g->p[g->vorhand])
		return 1;

	int first_card = g->card[g->vorhand];
		
	if(g->gametype == _NULL) {
		if(get_col(card) == get_col(first_card))
			return 1;
		for(int i = 0; i < p->hcardcount; ++i)
			if(get_col(p->hand[i]) == get_col(first_card))
				return 0;
		return 1;
	} else {
		if(get_col(first_card) == g->trumpf || get_val(first_card) == BUBE) {
			//trumpf gespielt
			if(get_col(card) == g->trumpf || get_val(card) == BUBE)
				return 1;
			for(int i = 0; i < p->hcardcount; ++i)
				if(get_col(p->hand[i]) == g->trumpf || get_val(p->hand[i]) == BUBE)
					return 0;
			return 1;
		} else {
			//andere farbe angespielt
			if(get_col(card) == get_col(first_card) && get_val(card) != BUBE)
				return 1;
			for(int i = 0; i < p->hcardcount; ++i)
				if(get_col(p->hand[i]) == get_col(first_card) && get_val(p->hand[i]) != BUBE)
					return 0;
			return 1;
		}
	}
	return 0;
}

int calc_spielwert(Game *g) {
	int wert = 0;
	
	if(g->hand)
		g->stufe++;
	if(g->schneidera)
		g->stufe++;
	if(g->p[g->player]->ouvert)
		g->stufe++;
	if(g->schwarza)
		g->stufe++;
	if(g->schwarz)
		g->stufe++;
	if(calc_points(g) > 90 || calc_points(g) <= 30)
		g->stufe++;

	switch(g->gametype) {
		case _GRAND:
		case _FARBSP:
		{
			int gw[] = {24, 12, 11, 10, 9};
			wert = g->stufe*gw[g->trumpf+1];
			if(calc_points(g) <= 60 || (calc_points(g) <= 90 && g->schneidera) || (!g->schwarz && g->schwarza))
				wert *= -2;
				
		}
		break;
		
		case _RAMSCH:
		wert = -calc_points(g);
		if(g->schwarz) {
			wert *= -1;
			break;
		}
		while(g->geschoben > 0) {
			wert *= 2;
			g->geschoben--;
		}
		for(int i = 0; i < 3; i++) {
			if(g->p[i]->cur_game_points == 0)
				wert *= 2;
		}
		break;
		
		case _NULL:
		if(g->hand && g->p[g->player]->ouvert)
			wert = 59;
		else if(g->p[g->player]->ouvert)
			wert = 46;
		else if(g->hand)
			wert = 35;
		else
			wert = 23;
		if(!g->null)
			wert *= -2;
		break;
	}
	while(g->kontra > 0) {
		wert *= 2;
		g->kontra--;
	}
	
	if(wert > 0 && wert < g->reizwert)
		wert = g->reizwert * -2;
	
	return wert;
}

int calc_points(Game *g) {
	int values[] = {11, 4, 3, 2, 10, 0, 0, 0};
	return g->p[g->player]->cur_game_points + values[get_val(g->skat[0])] + values[get_val(g->skat[1])];
}

int player_has_card(Player *p, int card) {
	for(int i = 0; i < p->hcardcount; ++i)
		if(p->hand[i] == card)
			return 1;
	return 0;
}

int length_of_color(Player *p, int farbe) {
	int len = 0;
	for(int i = 0; i < p->hcardcount; ++i)
		if(get_col(p->hand[i]) == farbe)
			++len;
	return len;
}

int blank(Player *p, int farbe) {
	return length_of_color(p, farbe) == 0;
}
