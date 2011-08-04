CC = gcc
CFLAGS = -march=native -pipe -std=c99 -fgnu89-inline
LDFLAGS	= `allegro-config --libs`
DEBUGFLAGS = -g3 -Wall
RELEASEFLAGS = -O3

OBJ =   skat.o ki.o main_dlg.o result_dlg.o player.o game.o ch_game_dlg.o reizen_dlg.o
BIN =   sskat

release: CFLAGS += $(RELEASEFLAGS)
release: all

all: $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) $(LDFLAGS)

%.o: src/%.cc
	$(CC) $(CFLAGS) -c $<

debug: CFLAGS += $(DEBUGFLAGS)
debug: all

clean:
	rm -rf $(BIN) $(OBJ) *~

windows: LDFLAGS=-lalleg44
windows: CC=i486-mingw32-gcc
windows: all
