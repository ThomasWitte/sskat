#ifndef RESULT_DLG_H
#define RESULT_DLG_H

#include <allegro.h>
#include <stdio.h>
#include "config.h"
#include "main_dlg.h"
#include "game.h"

extern DIALOG result_dlg[];

void init_result_dlg(Game *g);
void exit_result_dlg();

#endif
