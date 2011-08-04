#include <allegro.h>
#include <time.h>

#include "config.h"
#include "main_dlg.h"

volatile int timer;
void timerupdate() {
	++timer;
}
END_OF_FUNCTION(timerupdate)

void close_handler() {
	//send some esc to the dialogs
	simulate_ukeypress(27, 0);
	simulate_ukeypress(27, 0);
	simulate_ukeypress(27, 0);
	simulate_ukeypress(27, 0);
	simulate_ukeypress(27, 0);
}
END_OF_FUNCTION(close_handler)

int main(int argc, char **argv) {
	allegro_init();
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCRW, SCRH, 0, 0);
	install_keyboard();
	install_mouse();
	install_timer();
	
	LOCK_VARIABLE(timer);
	LOCK_FUNCTION(timerupdate);
	install_int_ex(timerupdate, BPS_TO_TIMER(TIMER_BPS));

	srand((unsigned)time(NULL));
	set_window_title("Skat");
	
	LOCK_FUNCTION(close_handler);
	set_close_button_callback(close_handler);

	cards = load_bitmap("images/cards.tga", NULL);
	game.p[0] = &human;
	game.p[1] = &cpu1;
	game.p[2] = &cpu2;
	human.choose_card = human_choose_card;
	cpu1.choose_card = cpu_choose_card;
	cpu2.choose_card = cpu_choose_card;
	human.choose_game = human_choose_game;
	cpu1.choose_game = cpu_choose_game;
	cpu2.choose_game = cpu_choose_game;
	human.reizen = human_reizen;
	cpu1.reizen = cpu_reizen;
	cpu2.reizen = cpu_reizen;
	human.schieben = human_schieben;
	cpu1.schieben = cpu_schieben;
	cpu2.schieben = cpu_schieben;
	human.kontra = human_kontra;
	cpu1.kontra = cpu_kontra;
	cpu2.kontra = cpu_kontra;
	human.name = "Stefan";
	cpu1.name = "Robert";
	cpu2.name = "Thomas";
	human.rtext = cpu1.rtext = cpu2.rtext = NULL;
	human.total_points = 0;
	cpu1.total_points = 0;
	cpu2.total_points = 0;

	enable_hardware_cursor();
	show_mouse(screen);

	buffered_do_dialog(main_dlg, -1);
	
	destroy_bitmap(cards);
}
END_OF_MAIN()
