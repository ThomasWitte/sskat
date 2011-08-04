#include "ki.h"

int choose_card_null(Player *p, Game *g);
int null_spielen(Player *p, Game *g);
void exchange_cards_null(Player *p, Game *g);

int cpu_choose_card(Player *p, Game *g) {
	//Entscheidung, welche Karte ein Computerspieler in einen Stich spielen soll
	int c;
	switch(g->gametype) {
		case _NULL:
			c = choose_card_null(p, g);
		break;

		default:
		do {
			c = rand()%p->hcardcount; //Zufällige Karte wählen
		} while(!playing_possible(g, p, p->hand[c])); //Falls die Karte nicht gespielt werden darf, neue Karte wählen
		break;
	}
	return c;
}

void cpu_choose_game(Player *p, Game *g) {
	//Entscheidung, welches Spiel angesagt wird, default ist Grand Hand
	
	int nullp = null_spielen(p,g);
	if(nullp) {
		g->gametype = _NULL;
		g->hand = 0;
		if(nullp == 46 || nullp == 59) p->ouvert = 1;
		if(nullp == 35 || nullp == 59) g->hand = 1;
		//Karten wählen zum Tauschen mit dem Skat
		else exchange_cards_null(p, g);
	}
}

int cpu_reizen(Player *p, Game *g) {
	//Maximaler Reizwert mit dem Aktuellen Blatt
	//Da die Funktion mehrmals aufgerufen wird muss sie deterministisch sein
	int ret = 0;
	
	int i = null_spielen(p,g);
	if(i > ret) ret = i;
	
	return ret;
}

void cpu_schieben(Player *p, Game *g) {
	//Enscheidung, welche Karten im Ramsch weitergeschoben werden
	g->geschoben++;
}

void cpu_kontra(Player *p, Game *g) {
	//Entscheidung ob ein Computerspieler Kontra ansagt
}

int null_spielen(Player *p, Game *g) {
	//Null (selbst):
	//-zwingend: auf jeder farbe die 7 (oder blank, notfalls auch eine blanke 8/9) (wenn nur eine farbe nicht passt -> auf skat reizen?)
	//-wenn auf jeder farbe 7,9,B,K (oder besser, oder blank): dann null hand/ouvert
	//-7,9,D,K,A auf einer Farbe auch okay (also wenn farbe sehr lang, dann können auch tiefe karten fehlen)
	int ret = 47;
	int risk = 0;
	for(int i = 0; i < 4; i++) {
		if(!player_has_card(p, get_card(i, SIEBEN)) && !blank(p, i) && !(length_of_color(p, i) == 1 && (player_has_card(p, get_card(i, NEUN)) || player_has_card(p, get_card(i, ACHT)))))
			risk += length_of_color(p, i);
		int val = 0;
		for(int j = 0; j < p->hcardcount; j++)
			if(get_col(p->hand[j]) == i)
				val += get_val(p->hand[j]);
		int min_val[] = {0, 7, 12, 15, 16, 15, 18, 22, 0};
		if(val < min_val[length_of_color(p,i)])
			ret = 23;
	}
	
	if(risk == 0) { //keine unsicheren Karten -> Hand
		ret += 12;
	}
	if(risk < 3) return ret; //max 2 unsichere Karten
	return 0;
}

int choose_card_null(Player *p, Game *g) {
	int c = 0;
	//Wenn Vorhand, spiele niedrigste Karte
	if(g->p[g->vorhand] == p) {
		for(int i = 0; i < p->hcardcount; i++) {
			if(get_val(p->hand[c]) < get_val(p->hand[i]))
				c = i;
		}
		return c;
	}
	
	//Wenn Farbe bekannt werden muss, wähle größtmögliche Karte ohne den Stich zu bekommen oder niedrigste Karte
	c = -1;
	for(int i = 0; i < p->hcardcount; i++) {
		if(get_col(p->hand[i]) == get_col(g->card[g->vorhand])) {
			if(c == -1) c = i;
			else {
				switch(higher(g, p->hand[i], p->hand[c])) {
					case 1:
					if(higher(g, p->hand[i], g->card[g->vorhand]) != 1 || (p != g->p[(g->vorhand+1)%3] && higher(g, p->hand[i], g->card[(g->vorhand+1)%3]) != 1))
						c = i;
					break;
					
					case -1:
					if(higher(g, p->hand[c], g->card[g->vorhand]) == 1 && (p == g->p[(g->vorhand+1)%3] || higher(g, p->hand[c], g->card[(g->vorhand+1)%3]) == 1))
						c = i;
					break;
				}
			}
		}
	}
	if(c >= 0) return c;
	
	//Sonst: wähle höchste farbfremde Karte
	c = 0;
	for(int i = 1; i < p->hcardcount; i++) {
		if(get_val(p->hand[i]) < get_val(p->hand[c]))
			c = i;
	}
	
	return c;
}

void exchange_cards_null(Player *p, Game *g) {
	//skat aufnehmen
	p->hand[10] = g->skat[0];
	p->hand[11] = g->skat[1];
	p->hcardcount = 12;
	for(; p->hcardcount > 10; --p->hcardcount) {
		int c = -1;

		//Risikofarbe suchen
		//dabei wird der wert jeder Karte (ass = 0 … sieben = 7) quadriert
		//und durch die anzahl der Karten der Farbe +1 geteilt
		//die Farbe mit dem geringsten Wert ist ein Risiko
		int col_risk = 0;
		float val_risk = 100.0;
		for(int col = 0; col < 4; ++col) {
			int val = 0;
			for(int j = 0; j < p->hcardcount; j++)
				if(get_col(p->hand[j]) == col)
					val += get_val(p->hand[j]);
			float risk = (float)(val*val+1)/((float)length_of_color(p, col)+1.0);
			if(val_risk > risk && risk != 0) {
				val_risk = risk;
				col_risk = col;
			}
		}
		
		//höchste Karte der Risikofarbe in den Skat
		for(int i = 0; i < p->hcardcount; ++i) {
			if(get_col(p->hand[i]) == col_risk) {
				if(c == -1) c = i;
				else if(get_val(p->hand[i]) < get_val(p->hand[c]))
					c = i;
			}
		}
		
		g->skat[p->hcardcount-10] = p->hand[c];
		p->hand[c] = p->hand[p->hcardcount-1];
	}
}
