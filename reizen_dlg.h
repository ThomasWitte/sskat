#ifndef REIZEN_DLG_H
#define REIZEN_DLG_H

#include <allegro.h>
#include "main_dlg.h"
#include "player.h"
#include "game.h"

extern DIALOG reizen_dlg[];

void init_reizen_dlg();
void exit_reizen_dlg();

int d_reizen_mgr(int msg, DIALOG *d, int c);

#endif
