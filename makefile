CC = gcc
CFLAGS = -I $(IDIR) `pkg-config --libs allegro-5 allegro_image-5 allegro_font-5 allegro_ttf-5`

IDIR = ./include/
SRCDIR = ./src/
#pkg-config --cflags -libs allegro-5.1 allegro_acodec-5.1 allegro_audio-5.1 \ allegro_color-5.1 allegro_dialog-5.1 allegro_font-5.1 allegro image-5.1 allegro_main-5.1\ allegro_memfile-5.1 allegro_physfs-5.1 allegro_primitives-5.1 allegro_ttf-5.1
SOURCES = $(SRCDIR)*.c

all: Pokemon #run #clean

Pokemon: $(SOURCES)
		$(CC) -g $(SOURCES) $(CFLAGS) -o ./Pokemon

run : 
		./Pokemon
clean:
		rm ./Pokemon
		clear