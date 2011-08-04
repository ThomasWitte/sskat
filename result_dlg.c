#include "result_dlg.h"

DIALOG result_dlg[] = {
	//proc,				x,				y,				w,				h,				fg,		bg,		key,	flags,	d1,		d2,		dp,		dp2,	dp3
	{d_bitmap,			0,				0,				SCRW,			SCRH,			0,		0,		0,		0,		0,		0,		NULL,	"images/background.tga",	NULL},
	{d_bitmap,			SCRW/4,			0,				SCRW,			SCRH,			0,		0,		0,		0,		0,		0,		NULL,	"images/note.tga",	NULL},
	{d_text_proc,		SCRW/3,			SCRH/4,			0,				0,				0,		-1,		0,		0,		0,		0,		NULL,	NULL,	NULL},
	{d_text_proc,		SCRW/3,			SCRH/4+20,		0,				0,				0,		-1,		0,		0,		0,		0,		NULL,	NULL,	NULL},
	{d_text_proc,		SCRW/3,			SCRH/4+40,		0,				0,				0,		-1,		0,		0,		0,		0,		NULL,	NULL,	NULL},
	{d_button_proc,		30,				SCRH-100,		120,			70,				0,		-1,		0,		D_EXIT,	0,		0,		"weiter", NULL, NULL},
	{NULL,				0,				0,				0,				0,				0,		0,		0,		0,		0,		0,		NULL,	NULL,	NULL}
};

void init_result_dlg(Game *g) {
	if(g->gametype == _RAMSCH) {
		g->player = 0;
		for(int i = 1; i < 3; i++) {
			if(g->p[i]->cur_game_points > g->p[g->player]->cur_game_points)
				g->player = i;
		}
	}
	
	int points = calc_points(g);
	int spw = calc_spielwert(g);
	if(g->gametype == _RAMSCH) {
		for(int i = 0; i < 3; ++i)
			if(g->p[i]->cur_game_points == g->p[g->player]->cur_game_points) {
				g->p[i]->total_points += spw;
			}
	}
	else
		g->p[g->player]->total_points += spw;
	 
	result_dlg[2].dp = malloc(100);
	result_dlg[3].dp = malloc(100);
	result_dlg[4].dp = malloc(100);
	switch(g->gametype) {
		case _FARBSP:
		case _GRAND:
		sprintf(result_dlg[2].dp, "%s %s das Spiel mit %i zu %i Punkten.",
			g->p[g->player]->name,
			(spw > 0 ? "gewinnt" : "verliert"),
			points,
			120-points);
		break;
		
		case _RAMSCH:
		sprintf(result_dlg[2].dp, "%s %s den Ramsch.",
			g->p[g->player]->name,
			(spw > 0 ? "gewinnt" : "verliert"));
		break;
		
		case _NULL:
		sprintf(result_dlg[2].dp, "%s %s das Nullspiel.",
			g->p[g->player]->name,
			(spw > 0 ? "gewinnt" : "verliert"));
		break;
	}
	sprintf(result_dlg[3].dp, "Der Spielwert ist %i.", spw);
	sprintf(result_dlg[4].dp, "Punkte: %s %i, %s %i, %s %i",
		g->p[0]->name, g->p[0]->total_points,
		g->p[1]->name, g->p[1]->total_points,
		g->p[2]->name, g->p[2]->total_points);
}

void exit_result_dlg() {
	free(result_dlg[2].dp);
	free(result_dlg[3].dp);
	free(result_dlg[4].dp);
}
