#include "reizen_dlg.h"

DIALOG reizen_dlg[] = {
	//proc,				x,				y,				w,				h,				fg,		bg,		key,	flags,	d1,		d2,		dp,		dp2,	dp3
	{d_reizen_mgr,		0,				0,				0,				0,				0,		0,		0,		0,		0,		0,		&game,	NULL,	NULL},
	{d_bitmap,			0,				0,				SCRW,			SCRH,			0,		0,		0,		0,		0,		0,		NULL,	"images/background.tga",	NULL},
	{d_bitmap,			2*SCRW/3,		SCRH/2,			SCRW,			SCRH,			0,		0,		0,		0,		0,		0,		NULL,	"images/note.tga",	NULL},
	{d_npc_hand,		0,				0,				SCRW/2,			SCRW/3,			0,		0,		0,		0,		1,		0,		&cpu1,	"images/face_Robert.tga",	NULL},
	{d_npc_hand,		SCRW/2,			0,				SCRW/2,			SCRW/3,			0,		0,		0,		0,		0,		0,		&cpu2,	"images/face_Thomas.tga",	NULL},
	{d_human_hand,		0,				2*SCRH/3,		2*SCRW/3,		SCRH/3,			0,		0,		0,		0,		0,		0,		&human,	NULL,	NULL},
	{d_button_proc,		SCRW-130,		SCRH-80,		100,			50,				0,		-1,		0,		0,		0,		0,		"weg",	NULL,	NULL},
	{d_button_proc,		SCRW-130,		SCRH-160,		100,			50,				0,		-1,		0,		0,		0,		0,		NULL,	NULL,	NULL},
	{d_timer,			0,				0,				0,				0,				0,		0,		0,		0,		0,		0,		NULL,	NULL,	NULL},
	{NULL,				0,				0,				0,				0,				0,		0,		0,		0,		0,		0,		NULL,	NULL,	NULL}
};

void init_reizen_dlg() {
	game.reizwert = 0;
	reizen_dlg[6].flags = reizen_dlg[7].flags = 0;
	reizen_dlg[7].dp = malloc(50);
	sprintf(reizen_dlg[7].dp, "ja");
}

void exit_reizen_dlg() {
	free(reizen_dlg[7].dp);
}

int set_update_timer_rz(Game *g, DIALOG *d) {
	if(g->p[d->x]->your_turn == 0 && g->p[d->y]->your_turn == 0)
		return 1;
	return 0;
}

int naechste_stufe(int i) {
	int stufen[] = {18, 20, 22, 23, 24, 27, 30, 33, 35, 36, 40, 44, 45, 46, 48, 50, 54, 55, 59, 60, 66, 72, 96, 120, 144, 168, 192};
	int ret = 0;
	for(; stufen[ret] <= i; ++ret);
	return stufen[ret];
}

int reizen_react(DIALOG *d, Game *g) {
	if(g->p[0]->your_turn) {
		if(g->p[0]->your_turn == 1) {
			int nstufe = naechste_stufe(g->reizwert);
			sprintf(reizen_dlg[7].dp, "%i", nstufe);
		} else if(g->p[0]->your_turn == -1) {
			sprintf(reizen_dlg[7].dp, "ja");
		}
		if(reizen_dlg[6].flags & D_SELECTED) {
			//weg geklickt
			reizen_dlg[6].flags &= ~D_SELECTED;
			g->card[0] = 0;
			g->p[0]->your_turn = 0;
		} else if(reizen_dlg[7].flags & D_SELECTED) {
			//ja/… geklickt
			g->card[0] = -2;
			reizen_dlg[7].flags &= ~D_SELECTED;
			if(g->p[0]->your_turn == 1)
				g->reizwert = naechste_stufe(g->reizwert);
			g->p[0]->your_turn = 0;
		}
	}
		
	if(g->p[1]->your_turn) {
		if(g->p[1]->your_turn == 1) {
			if(d->d1 > g->reizwert) {
				int nstufe = naechste_stufe(g->reizwert);
				g->reizwert = nstufe;
				sprintf(g->p[1]->rtext, "%i", nstufe);
				sprintf(g->p[2]->rtext, "%s", "");
			} else {
				sprintf(g->p[1]->rtext, "weg");
				sprintf(g->p[2]->rtext, "%s", "");
				g->card[1] = 0;
			}
		} else if(g->p[1]->your_turn == -1) {
			if(d->d1 >= g->reizwert && d->d1 > 0) {
				sprintf(g->p[1]->rtext, "ja");
				sprintf(g->p[2]->rtext, "%s", "");
			} else {
				sprintf(g->p[1]->rtext, "weg");
				sprintf(g->p[2]->rtext, "%s", "");
				g->card[1] = 0;
			}
		}
		g->p[1]->your_turn = 0;
		return D_REDRAW;
	}

	if(g->p[2]->your_turn) {
		if(g->p[2]->your_turn == 1) {
			if(d->d2 > g->reizwert) {
				int nstufe = naechste_stufe(g->reizwert);
				g->reizwert = nstufe;
				sprintf(g->p[2]->rtext, "%i", nstufe);
				sprintf(g->p[1]->rtext, "%s", "");
			} else {
				sprintf(g->p[2]->rtext, "weg");
				sprintf(g->p[1]->rtext, "%s", "");
				g->card[2] = 0;
			}
		} else if(g->p[2]->your_turn == -1) {
			if(d->d2 >= g->reizwert && d->d2 > 0) {
				sprintf(g->p[2]->rtext, "ja");
				sprintf(g->p[1]->rtext, "%s", "");
			} else {
				sprintf(g->p[2]->rtext, "weg");
				sprintf(g->p[1]->rtext, "%s", "");
				g->card[2] = 0;
			}
		}
		g->p[2]->your_turn = 0;
		return D_REDRAW;
	}
	return D_O_K;
}

int d_reizen_mgr(int msg, DIALOG *d, int c) {
	Game *g = (Game*)d->dp;
	switch(msg) {
		case MSG_START:
		g->p[1]->rtext = malloc(50);
		g->p[2]->rtext = malloc(50);
		sprintf(g->p[1]->rtext, "%s", "");
		sprintf(g->p[2]->rtext, "%s", "");
		d->d1 = g->p[1]->reizen(g->p[1], g);
		d->d2 = g->p[2]->reizen(g->p[2], g);
		d->fg = 2;
		d->x = (g->vorhand+1)%3;
		d->y = g->vorhand;
		d->bg = 0;
		for(int i = 0; i < 3; i++)
			g->card[i] = -1;
		break;

		case MSG_IDLE:
		if(set_update_timer_rz(g, d) && d->fg == 0) {
			d->fg = 10;
		}
		return reizen_react(d, g);
		
		case MSG_TIMER:
		if(d->fg > 0)
			--d->fg;
		if(d->fg == 1) {
			do {
			if(d->y == d->x) {
				d->y = (d->y+2)%3;
				if(d->y == (g->vorhand+1)%3) {
					if(g->card[0] != -1) return D_EXIT;
					else goto out;
				}
			}
			
			while(g->card[d->x] == 0) {
				d->x = (d->x+2)%3;
				d->bg = (d->bg+1)%2;
				if(d->x == (g->vorhand+1)%3) return D_EXIT;
			}
			while(g->card[d->y] == 0) {
				d->y = (d->y+2)%3;
				d->bg = (d->bg+1)%2;
				if(d->y == (g->vorhand+1)%3) {
					if(g->card[0] != -1) return D_EXIT;
					else goto out;
				}
			}
			} while(d->x == d->y);

			out:
			if(d->bg == 0 ) {//sagen
				d->bg = 1;
				g->p[d->x]->your_turn = 1;
			} else if(d->bg == 1) {//hören
				d->bg = 0;
				g->p[d->y]->your_turn = -1;
			}
			
			return D_REDRAW;
		}
		break;
			
		case MSG_END:
		if(g->p[1]->rtext)
			free(g->p[1]->rtext);
		g->p[1]->rtext = NULL;
		if(g->p[2]->rtext)
			free(g->p[2]->rtext);
		g->p[2]->rtext = NULL;

		if(g->card[0] == 0)
			g->reizwert = 0;
		for(int i = 0; i < 3; i++)
			g->card[i] = -1;
		break;
	}
	return D_O_K;
}
