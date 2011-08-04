#include "main_dlg.h"

BITMAP *cards = NULL;

DIALOG main_dlg[] = {
	//proc,				x,				y,				w,				h,				fg,		bg,		key,	flags,	d1,		d2,		dp,		dp2,	dp3
	{d_game_logic,		0,				0,				0,				0,				0,		0,		0,		0,		0,		0,		&game,	NULL,	NULL},
	{d_timer,			0,				0,				0,				0,				0,		0,		0,		0,		0,		0,		NULL,	NULL,	NULL},
	{d_player,			0,				0,				0,				0,				0,		0,		0,		0,		0,		0,		&human,	&game,	NULL},
	{d_player,			0,				0,				0,				0,				0,		0,		0,		0,		0,		0,		&cpu1,	&game,	NULL},
	{d_player,			0,				0,				0,				0,				0,		0,		0,		0,		0,		0,		&cpu2,	&game,	NULL},
	{d_bitmap,			0,				0,				SCRW,			SCRH,			0,		0,		0,		0,		0,		0,		NULL,	"images/background.tga",	NULL},
	{d_play_area,		SCRW/3,			SCRH/3,			SCRW/3,			SCRH/3,			0,		0,		0,		0,		0,		0,		&game,	NULL,	NULL},
	{d_npc_hand,		0,				0,				SCRW/2,			SCRW/3,			0,		0,		0,		0,		1,		1,		&cpu1,	"images/face_Robert.tga",	NULL},
	{d_npc_hand,		SCRW/2,			0,				SCRW/2,			SCRW/3,			0,		0,		0,		0,		0,		2,		&cpu2,	"images/face_Thomas.tga",	NULL},
	{d_human_hand,		SCRW/6,			2*SCRH/3,		2*SCRW/3,		SCRH/3,			0,		0,		0,		0,		0,		0,		&human,	NULL,	NULL},
	{NULL,				0,				0,				0,				0,				0,		0,		0,		0,		0,		0,		NULL,	NULL,	NULL}
};

DIALOG show_game_dlg[] = {
	//proc,				x,				y,				w,				h,				fg,		bg,		key,	flags,	d1,		d2,		dp,		dp2,	dp3
	{d_bitmap,			0,				0,				SCRW,			SCRH,			0,		0,		0,		0,		0,		0,		NULL,	"images/background.tga",	NULL},
	{d_button_proc,		SCRW-130,		SCRH-80,		100,			50,				0,		-1,		0,		D_EXIT,	0,		0,		"Aha",	NULL,	NULL},
	{d_bitmap,			250,			50,				0,				0,				0,		0,		0,		0,		0,		0,		NULL,	"images/ramsch.tga",	NULL},
	{d_bitmap,			300,			100,			0,				0,				0,		0,		0,		0,		0,		0,		NULL,	NULL,	NULL},
	{d_bitmap,			20,				20,				0,				0,				0,		0,		0,		0,		0,		0,		NULL,	"images/face_Robert.tga",	NULL},
	{d_button_proc,		SCRW-130,		SCRH-160,		100,			50,				0,		-1,		0,		D_EXIT,	0,		0,		NULL,	NULL,	NULL},
	{d_human_hand,		SCRW/6,			2*SCRH/3,		2*SCRW/3,		SCRH/3,			0,		0,		0,		0,		0,		0,		&human,	NULL,	NULL},
	{NULL,				0,				0,				0,				0,				0,		0,		0,		0,		0,		0,		NULL,	NULL,	NULL}
};

DIALOG ramsch_dlg[] = {
	{d_bitmap,			0,				0,				SCRW,			SCRH,			0,		0,		0,		0,		0,		0,		NULL,	"images/background.tga",	NULL},
	{d_skat,			SCRW/2-CARDW*0.375, SCRH/2-CARDH*0.375, CARDH*0.75,	CARDH*0.75,	0,		0,		0,		0,		0,		0,		&game,	&human,	NULL},
	{d_npc_hand,		0,				0,				SCRW/2,			SCRW/3,			0,		0,		0,		0,		1,		0,		&cpu1,	"images/face_Robert.tga",	NULL},
	{d_npc_hand,		SCRW/2,			0,				SCRW/2,			SCRW/3,			0,		0,		0,		0,		0,		0,		&cpu2,	"images/face_Thomas.tga",	NULL},
	{d_human_hand,		SCRW/6,			2*SCRH/3,		2*SCRW/3,		SCRH/3,			0,		0,		0,		0,		0,		0,		&human,	NULL,	NULL},
	{d_button_proc,		SCRW-130,		SCRH-80,		100,			50,				0,		-1,		0,		D_EXIT,	0,		0,		"Schieben",	NULL,	NULL},
	{d_ramsch_mgr,		0,				0,				0,				0,				0,		0,		0,		0,		5,		0,		&game,	NULL,	NULL},
	{NULL,				0,				0,				0,				0,				0,		0,		0,		0,		0,		0,		NULL,	NULL,	NULL}
};

void init_show_game_dlg(Game *g) {
	show_game_dlg[1].flags = D_EXIT;
	show_game_dlg[5].flags = D_EXIT;
	show_game_dlg[2].dp2 = malloc(100);
	show_game_dlg[4].dp2 = malloc(100);
	show_game_dlg[3].dp2 = malloc(100);
	show_game_dlg[5].dp = malloc(50);
	show_game_dlg[4].proc = d_bitmap;
	show_game_dlg[5].proc = d_button_proc;
	switch(g->kontra) {
		case KONTRA:
		sprintf(show_game_dlg[5].dp, "Kontra!");
		if(g->player == 0) {
			show_game_dlg[5].proc = NULL;
		}
		break;
		
		case RE:
		sprintf(show_game_dlg[5].dp, "Re!!");
		if(g->player != 0) {
			show_game_dlg[5].proc = NULL;
		}
		sprintf(show_game_dlg[3].dp2, "images/kontra.tga");
		break;
		
		case BOCK:
		sprintf(show_game_dlg[5].dp, "Bock!!!");
		if(g->player == 0) {
			show_game_dlg[5].proc = NULL;
		}
		break;
		
		case HIRSCH:
		sprintf(show_game_dlg[5].dp, "Hirsch!!!!");
		if(g->player != 0) {
			show_game_dlg[5].proc = NULL;
		}
		break;
		
		default:
		show_game_dlg[5].proc = NULL;
		show_game_dlg[5].flags |= D_DISABLED;
		break;
	}
	
	switch(g->player) {
		case 0:
		case 1:
		case 2:
		sprintf(show_game_dlg[4].dp2, "images/face_%s.tga", g->p[g->player]->name);
		break;
		
		default:
		show_game_dlg[4].proc = NULL;
	}
	switch(g->gametype) {
		case _NULL:
		sprintf(show_game_dlg[2].dp2, "images/null.tga");
		break;

		case _GRAND:
		sprintf(show_game_dlg[2].dp2, "images/grand.tga");
		break;

		case _RAMSCH:
		sprintf(show_game_dlg[2].dp2, "images/ramsch.tga");
		break;
		
		case _FARBSP:
		switch(g->trumpf) {
			case KREUZ:
			sprintf(show_game_dlg[2].dp2, "images/kreuz.tga");
			break;
			
			case PIK:
			sprintf(show_game_dlg[2].dp2, "images/pik.tga");
			break;
			
			case HERZ:
			sprintf(show_game_dlg[2].dp2, "images/herz.tga");
			break;
			
			case KARO:
			sprintf(show_game_dlg[2].dp2, "images/karo.tga");
			break;
		}
		break;
	}
}

void exit_show_game_dlg() {
	free(show_game_dlg[2].dp2);
	free(show_game_dlg[3].dp2);
	free(show_game_dlg[4].dp2);
	free(show_game_dlg[5].dp);
}

void init_ramsch_dlg(Game *g) {
}

void exit_ramsch_dlg() {
}

void draw_card(BITMAP *scr, BITMAP *cards, int x, int y, int w, int h, int card) {
	masked_stretch_blit(cards, scr, card*CARDW, 0, CARDW, CARDH, x, y, w, h);
}

int d_bitmap(int msg, DIALOG *d, int c) {
	switch(msg) {
		case MSG_START:
		if(d->dp2)
			d->dp = load_bitmap((char*)d->dp2, NULL);
		break;
		
		case MSG_DRAW:
		if(d->dp)
			masked_blit((BITMAP*)d->dp, gui_get_screen(), 0, 0, d->x, d->y, ((BITMAP*)d->dp)->w, ((BITMAP*)d->dp)->h);
		break;
		
		case MSG_END:
		if(d->dp)
			destroy_bitmap(d->dp);
		break;
	}
	return D_O_K;
}

int d_human_hand(int msg, DIALOG *d, int c) {
	switch(msg) {
		case MSG_RESTART:
		case MSG_START:
		if(!d->dp) return D_EXIT;
		d->d1 = -1;
		d->d2 = 0;
		break;
		
		case MSG_DRAW:
		{
//			rect(screen, d->x, d->y, d->x+d->w, d->y+d->h, makecol(255,255,255));
			int width = (d->w-CARDW*0.75) / (((Player*)d->dp)->hcardcount-1);
			for(int i = 0; i < ((Player*)d->dp)->hcardcount; ++i) {
				draw_card(gui_get_screen(), cards, d->x+i*width,
					d->y+(i-((Player*)d->dp)->hcardcount/2)*(i-((Player*)d->dp)->hcardcount/2),
					0.75*CARDW, 0.75*CARDH, ((Player*)d->dp)->hand[i]);
			}
			if(d->d1 >= 0)
				draw_card(gui_get_screen(), cards, d->x+d->d1*width,
					d->y+d->h-CARDH, CARDW, CARDH, ((Player*)d->dp)->hand[d->d1]);
		}
		break;
		
		case MSG_WANTFOCUS:
		return D_WANTFOCUS;
		
		case MSG_LOSTMOUSE:
		d->d1 = -1;
		d->d2 = 0;
		return D_REDRAW;
		
		case MSG_GOTMOUSE:
		d->d2 = 1;
		break;

		case MSG_LRELEASE: //MSG_CLICK seems to be broken
		((Player*)d->dp)->clicked_card = d->d1;
		break;
		
		case MSG_IDLE:
		if(d->d2 == 1) {
			int curcard = (gui_mouse_x()-d->x)*(((Player*)d->dp)->hcardcount-1)/(d->w-CARDW*0.75);
			if(curcard >= ((Player*)d->dp)->hcardcount)
				curcard = ((Player*)d->dp)->hcardcount-1;
			if(d->d1 != curcard) {
				d->d1 = curcard;
				return D_REDRAW;
			}
		}
		break;
	}
	return D_O_K;
}

int d_npc_hand(int msg, DIALOG *d, int c) {
	switch(msg) {
		case MSG_START:
			d->dp3 = malloc(3*sizeof(BITMAP*));
			((BITMAP**)d->dp3)[0] = load_bitmap((char*)d->dp2, NULL);
			((BITMAP**)d->dp3)[1] = load_bitmap("images/bubble.tga", NULL);
			switch(game.gametype) {
				case _FARBSP:
				switch(game.trumpf) {
					case KREUZ:
					((BITMAP**)d->dp3)[2] = load_bitmap("images/kreuz_s.tga", NULL);
					break;
					
					case KARO:
					((BITMAP**)d->dp3)[2] = load_bitmap("images/karo_s.tga", NULL);
					break;
					
					case PIK:
					((BITMAP**)d->dp3)[2] = load_bitmap("images/pik_s.tga", NULL);
					break;
					
					case HERZ:
					((BITMAP**)d->dp3)[2] = load_bitmap("images/herz_s.tga", NULL);
					break;
				}
				break;
				
				case _NULL:
				((BITMAP**)d->dp3)[2] = load_bitmap("images/null_s.tga", NULL);
				break;
				
				case _GRAND:
				((BITMAP**)d->dp3)[2] = load_bitmap("images/grand_s.tga", NULL);
				break;
				
				default:
				((BITMAP**)d->dp3)[2] = NULL;
			}
		break;
		
		case MSG_DRAW:
//		rect(screen, d->x, d->y, d->x+d->w, d->y+d->h, makecol(255,255,255));
		masked_blit(((BITMAP**)d->dp3)[0], gui_get_screen(), 0, 0, (d->d1 == 1 ? d->x : d->x+d->w-((BITMAP**)d->dp3)[0]->w), d->y, ((BITMAP**)d->dp3)[0]->w, ((BITMAP**)d->dp3)[0]->h);
		for(int i = 0; i < ((Player*)d->dp)->hcardcount; ++i) {
			draw_card(gui_get_screen(), cards, d->x + (d->d1 == 1 ? i*d->w/12 : d->w-CARDW/2-i*d->w/12),
				d->y+d->h-CARDH/2-i*i*2, 
				CARDW/2, CARDH/2, (((Player*)d->dp)->ouvert ? ((Player*)d->dp)->hand[i] : 0));
		}
		if(((Player*)d->dp)->rtext && strcmp(((Player*)d->dp)->rtext, "") != 0) {
			masked_blit(((BITMAP**)d->dp3)[1], gui_get_screen(), 0, 0, (d->d1 == 1 ? d->x+30 : d->x+d->w-((BITMAP**)d->dp3)[1]->w-45), d->y+d->h-((BITMAP**)d->dp3)[1]->h-100, ((BITMAP**)d->dp3)[1]->w, ((BITMAP**)d->dp3)[1]->h);
			gui_textout_ex(gui_get_screen(), ((Player*)d->dp)->rtext, (d->d1 == 1 ? d->x+45 : d->x+d->w-((BITMAP**)d->dp3)[1]->w-30), d->y+d->h-((BITMAP**)d->dp3)[1]->h-50, 0, -1, 0);
		}
		if(d->d2 > 0 && game.player == d->d2)
			masked_blit(((BITMAP**)d->dp3)[2], gui_get_screen(), 0, 0, (d->d1 == 1 ? d->x+250 : d->x+d->w-((BITMAP**)d->dp3)[2]->w-250), d->y+20, ((BITMAP**)d->dp3)[2]->w, ((BITMAP**)d->dp3)[2]->h);
		break;
		
		case MSG_END:
			if(d->dp3) {
				destroy_bitmap(((BITMAP**)d->dp3)[0]);
				destroy_bitmap(((BITMAP**)d->dp3)[1]);
				if(((BITMAP**)d->dp3)[2])
					destroy_bitmap(((BITMAP**)d->dp3)[2]);
				free(d->dp3);
			}
		break;
	}
	return D_O_K;
}

int d_play_area(int msg, DIALOG *d, int c) {
	switch(msg) {
		case MSG_DRAW:
		{
//		rect(screen, d->x, d->y, d->x+d->w, d->y+d->h, makecol(255,255,255));
		int midx = (d->x+d->w/2)-CARDW*0.375;
		int midy = (d->y+d->h/2)-CARDH*0.375;
		for(int i = ((Game*)d->dp)->vorhand; i < ((Game*)d->dp)->vorhand+3; ++i)
			switch(i%3) {
				case 0:
				if(((Game*)d->dp)->card[0] >= 0)
					draw_card(gui_get_screen(), cards,	midx, midy+10, CARDW*0.75, CARDH*0.75, ((Game*)d->dp)->card[0]);
				break;
				
				case 1:
				if(((Game*)d->dp)->card[1] >= 0)
					draw_card(gui_get_screen(), cards,	midx-40, midy-20, CARDW*0.75, CARDH*0.75, ((Game*)d->dp)->card[1]);
				break;
				
				case 2:
				if(((Game*)d->dp)->card[2] >= 0)
					draw_card(gui_get_screen(), cards,	midx+40, midy-20, CARDW*0.75, CARDH*0.75, ((Game*)d->dp)->card[2]);
				break;
			}
		}
		break;
	}
	return D_O_K;
}

int d_timer(int msg, DIALOG *d, int c) {
	switch(msg) {
		case MSG_IDLE:
		while(timer > 0) {
			--timer;
			int ret = broadcast_dialog_message(MSG_TIMER, 0);
			if(ret & D_RESTART) {
				broadcast_dialog_message(MSG_RESTART, 0);
				broadcast_dialog_message(MSG_DRAW, 0);
			}
			return ret;
		}
		break;
		
		case MSG_RESTART:
		case MSG_START:
		timer = 0;
		break;
	}
	return D_O_K;
}

int d_ramsch_mgr(int msg, DIALOG *d, int c) {
	Game *g = (Game*)d->dp;
	if(g->hand == 0) {
		if((get_val(g->skat[0]) == BUBE || get_val(g->skat[1]) == BUBE) && !(ramsch_dlg[d->d1].flags & D_DISABLED)) {
			ramsch_dlg[d->d1].flags |= D_DISABLED;
			return D_REDRAW;
		}
		if((get_val(g->skat[0]) != BUBE && get_val(g->skat[1]) != BUBE) && (ramsch_dlg[d->d1].flags & D_DISABLED)) {
			ramsch_dlg[d->d1].flags &= ~D_DISABLED;
			return D_REDRAW;
		}
	}
	return D_O_K;
}

int buffered_do_dialog(DIALOG *d, int e) {
	DIALOG_PLAYER *player = init_dialog(d, e);
	BITMAP *buffer = create_bitmap(SCRW, SCRH);
	BITMAP *scr = gui_get_screen();
	gui_set_screen(buffer);

	//unschön, dass der buffer dauernd geblittet wird, aber auf die schnelle fällt mir nichts besseres ein
	while(update_dialog(player)) {
		blit(buffer, screen, 0, 0, 0, 0, SCRW, SCRH);
	}
	
	gui_set_screen(scr);
	destroy_bitmap(buffer);
	return shutdown_dialog(player);
}
