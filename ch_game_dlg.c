#include "ch_game_dlg.h"

DIALOG ch_game_dlg[] = {
	//proc,				x,				y,				w,				h,				fg,		bg,		key,	flags,	d1,		d2,		dp,		dp2,	dp3
	{d_bitmap,			0,				0,				SCRW,			SCRH,			0,		0,		0,		0,		0,		0,		NULL,	"images/background.tga",	NULL},
	{d_bitmap,			2*SCRW/3,		SCRH/2,			SCRW,			SCRH,			0,		0,		0,		0,		0,		0,		NULL,	"images/note.tga",	NULL},
	{d_skat,			SCRW/2-CARDW*0.375, SCRH/2-CARDH*0.375, CARDH*0.75,	CARDH*0.75,	0,		0,		0,		0,		0,		0,		&game,	&human,	NULL},
	{d_npc_hand,		0,				0,				SCRW/2,			SCRW/3,			0,		0,		0,		0,		1,		0,		&cpu1,	"images/face_Robert.tga",	NULL},
	{d_npc_hand,		SCRW/2,			0,				SCRW/2,			SCRW/3,			0,		0,		0,		0,		0,		0,		&cpu2,	"images/face_Thomas.tga",	NULL},
	{d_human_hand,		0,				2*SCRH/3,		2*SCRW/3,		SCRH/3,			0,		0,		0,		0,		0,		0,		&human,	NULL,	NULL},
	{d_text_proc,		3*SCRW/4,		SCRH/2+80,		0,				0,				0,		-1,		0,		0,		0,		0,		NULL,	NULL,	NULL},
	{d_radio_cb,		3*SCRW/4,		SCRH/2+100,		100,			10,				0,		-1,		0,		0,		1,		0,		"Null",	&game, game_ch_handler},
	{d_radio_cb,		3*SCRW/4,		SCRH/2+120,		100,			10,				0,		-1,		0,		D_SELECTED, 1,	0,		"Grand",	&game, game_ch_handler},
	{d_radio_cb,		3*SCRW/4,		SCRH/2+140,		100,			10,				0,		-1,		0,		0,		1,		0,		"Kreuz",	&game, game_ch_handler},
	{d_radio_cb,		3*SCRW/4,		SCRH/2+160,		100,			10,				0,		-1,		0,		0,		1,		0,		"Pik",	&game, game_ch_handler},
	{d_radio_cb,		3*SCRW/4,		SCRH/2+180,		100,			10,				0,		-1,		0,		0,		1,		0,		"Herz",	&game, game_ch_handler},
	{d_radio_cb,		3*SCRW/4,		SCRH/2+200,		100,			10,				0,		-1,		0,		0,		1,		0,		"Karo",	&game, game_ch_handler},
	{d_check_cb,		3*SCRW/4,		SCRH/2+240,		100,			10,				0,		-1,		0,		0,		0,		0,		"Schneider",	&game, game_ch_handler},
	{d_check_cb,		3*SCRW/4,		SCRH/2+260,		100,			10,				0,		-1,		0,		0,		0,		0,		"Ouvert",	&game, game_ch_handler},
	{d_check_cb,		3*SCRW/4,		SCRH/2+280,		100,			10,				0,		-1,		0,		0,		0,		0,		"Schwarz",	&game, game_ch_handler},
	{d_button_proc,		SCRW-130,		SCRH-80,		100,			50,				0,		-1,		0,		D_EXIT,	0,		0,		"Los gehts!",	NULL,	NULL},
	{NULL,				0,				0,				0,				0,				0,		0,		0,		0,		0,		0,		NULL,	NULL,	NULL}
};

void init_ch_game_dlg(Game *g) {
	ch_game_dlg[6].dp = malloc(50);
	sprintf(ch_game_dlg[6].dp, "Gereizt bis %i", g->reizwert);
	set_control(ch_game_dlg, "Schwarz", D_DISABLED);
	set_control(ch_game_dlg, "Ouvert", D_DISABLED);
	set_control(ch_game_dlg, "Schneider", 0);
	set_control(ch_game_dlg, "Null", 0);
	set_control(ch_game_dlg, "Grand", D_SELECTED);
	set_control(ch_game_dlg, "Kreuz", 0);
	set_control(ch_game_dlg, "Pik", 0);
	set_control(ch_game_dlg, "Herz", 0);
	set_control(ch_game_dlg, "Karo", 0);
}

void exit_ch_game_dlg() {
	free(ch_game_dlg[6].dp);
}

void set_control(DIALOG *dlg, char *element, int nflag) {
	for(; dlg->proc; ++dlg) {
		if(dlg->dp)
			if(strcmp(dlg->dp, element) == 0) {
				dlg->flags = nflag;
				return;
			}
	}
}

int d_radio_cb(int msg, DIALOG *d, int c) {
	switch(msg) {
		case MSG_KEY:
		case MSG_LRELEASE:
		if(d->dp3 && !(d->flags & D_DISABLED))
			((void(*)(DIALOG*, Game*))d->dp3)(d, d->dp2);
		return d_radio_proc(msg, d, c) | D_REDRAW;
	}
	return d_radio_proc(msg, d, c);
}

int d_check_cb(int msg, DIALOG *d, int c) {
	switch(msg) {
		case MSG_KEY:
		case MSG_LRELEASE:
		if(d->dp3)
			((void(*)(DIALOG*, Game*))d->dp3)(d, d->dp2);
		return d_check_proc(msg, d, c) | D_REDRAW;
	}
	return d_check_proc(msg, d, c);
}

void game_ch_handler(DIALOG *d, Game *g) {
	char *str = (char*)d->dp;
	if(strcmp(str, "Null") == 0) {
		g->gametype = _NULL;
		g->trumpf = -1;
		g->schwarza = 0;
		g->schneidera = 0;
		g->p[g->player]->ouvert = 0;
		set_control(ch_game_dlg, "Schwarz", D_DISABLED);
		set_control(ch_game_dlg, "Schneider", D_DISABLED);
		set_control(ch_game_dlg, "Ouvert", 0);
	} else if(strcmp(str, "Grand") == 0) {
		g->gametype = _GRAND;
		g->trumpf = -1;
		g->schwarza = 0;
		g->schneidera = 0;
		g->p[g->player]->ouvert = 0;
		set_control(ch_game_dlg, "Schwarz", D_DISABLED);
		set_control(ch_game_dlg, "Ouvert", D_DISABLED);
		set_control(ch_game_dlg, "Schneider", (g->hand ? 0 : D_DISABLED));
	} else if(strcmp(str, "Kreuz") == 0) {
		g->gametype = _FARBSP;
		g->trumpf = KREUZ;
		g->schwarza = 0;
		g->schneidera = 0;
		g->p[g->player]->ouvert = 0;
		set_control(ch_game_dlg, "Schwarz", D_DISABLED);
		set_control(ch_game_dlg, "Ouvert", D_DISABLED);
		set_control(ch_game_dlg, "Schneider", (g->hand ? 0 : D_DISABLED));
	} else if(strcmp(str, "Pik") == 0) {
		g->gametype = _FARBSP;
		g->trumpf = PIK;
		g->schwarza = 0;
		g->schneidera = 0;
		g->p[g->player]->ouvert = 0;
		set_control(ch_game_dlg, "Schwarz", D_DISABLED);
		set_control(ch_game_dlg, "Ouvert", D_DISABLED);
		set_control(ch_game_dlg, "Schneider", (g->hand ? 0 : D_DISABLED));
	} else if(strcmp(str, "Herz") == 0) {
		g->gametype = _FARBSP;
		g->trumpf = HERZ;
		g->schwarza = 0;
		g->schneidera = 0;
		g->p[g->player]->ouvert = 0;
		set_control(ch_game_dlg, "Schwarz", D_DISABLED);
		set_control(ch_game_dlg, "Ouvert", D_DISABLED);
		set_control(ch_game_dlg, "Schneider", (g->hand ? 0 : D_DISABLED));
	} else if(strcmp(str, "Karo") == 0) {
		g->gametype = _FARBSP;
		g->trumpf = KARO;
		g->schwarza = 0;
		g->schneidera = 0;
		g->p[g->player]->ouvert = 0;
		set_control(ch_game_dlg, "Schwarz", D_DISABLED);
		set_control(ch_game_dlg, "Ouvert", D_DISABLED);
		set_control(ch_game_dlg, "Schneider", (g->hand ? 0 : D_DISABLED));
	} else if(strcmp(str, "Schneider") == 0) {
		if(g->schneidera == 0) {
			g->schneidera = 1;
			set_control(ch_game_dlg, "Schwarz", 0);
		} else {
			g->schwarza = 0;
			g->schneidera = 0;
			g->p[g->player]->ouvert = 0;
			set_control(ch_game_dlg, "Schwarz", D_DISABLED);
			set_control(ch_game_dlg, "Ouvert", D_DISABLED);
		}
	} else if(strcmp(str, "Schwarz") == 0) {
		if(g->schwarza == 0) {
			g->schwarza = 1;
			set_control(ch_game_dlg, "Ouvert", 0);
		} else {
			g->schwarza = 0;
			g->p[g->player]->ouvert = 0;
			set_control(ch_game_dlg, "Ouvert", D_DISABLED);
		}
	} else if(strcmp(str, "Ouvert") == 0) {
		g->p[g->player]->ouvert = (g->p[g->player]->ouvert+1)%2;
	}
	sort_hand(g, g->p[g->player]);
}

int d_skat(int msg, DIALOG *d, int c) {
	Game *g = (Game*)d->dp;
	Player *p = (Player*)d->dp2;
	switch(msg) {
		case MSG_START:
		d->d1 = 0;
		d->d2 = 0;
		break;
		
		case MSG_LRELEASE:
		if(d->d1 == 0) {
			d->d1 = 1;
			g->hand = 0;
			set_control(ch_game_dlg, "Schneider", D_DISABLED);
			set_control(ch_game_dlg, "Schwarz", D_DISABLED);
			if(g->gametype != _NULL)
				set_control(ch_game_dlg, "Ouvert", D_DISABLED);
			p->clicked_card = -1;
			return D_REDRAWME;
		}
		break;
		
		case MSG_DRAW:
		if(d->d1 == 0) {
			draw_card(gui_get_screen(), cards, d->x, d->y, CARDW*0.75, CARDH*0.75, 0);
			draw_card(gui_get_screen(), cards, d->x+d->w-CARDW*0.75, d->y, CARDW*0.75, CARDH*0.75, 0);
		} else {
			draw_card(gui_get_screen(), cards, d->x, d->y, CARDW*0.75, CARDH*0.75, g->skat[0]);
			draw_card(gui_get_screen(), cards, d->x+d->w-CARDW*0.75, d->y, CARDW*0.75, CARDH*0.75, g->skat[1]);
		}
		break;
		
		case MSG_IDLE:
		{
			if(p->clicked_card >= 0 && d->d1 == 1) {
				int t = p->hand[p->clicked_card];
				p->hand[p->clicked_card] = g->skat[d->d2];
				g->skat[d->d2] = t;
				sort_hand(g, p);
				p->clicked_card = -1;
				d->d2 = (d->d2+1)%2;
				return D_REDRAW;
			}
		}
		break;
	}
	return D_O_K;
}
