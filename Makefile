CC := gcc
RM := rm -rf
LIBGL := src/gl.c
CFLAGS := -Wall -Werror -Wextra -O3
MFLAGS := -I/opt/X11/include -L/opt/X11/lib
EXAMPLES := simple_window \
	event_handling \
	create_window \
	font_drawing \
	draw_square

all: clean build

build: clean
	@$(CC) src/simple_window.c $(CFLAGS) $(MFLAGS) -lX11 -o simple_window
	@$(CC) src/event_handling.c $(CFLAGS) $(MFLAGS) -lX11 -o event_handling
	@$(CC) src/create_window.c $(CFLAGS) $(MFLAGS) -lX11 -o create_window
	@$(CC) src/font_drawing.c $(CFLAGS) $(MFLAGS) -lX11 -o font_drawing
	@$(CC) src/draw_square.c $(LIBGL) $(CFLAGS) $(MFLAGS) -lX11 -o draw_square -lm

clean:
	@$(RM) $(EXAMPLES)

simple_window:
	@$(RM) $@
	@$(CC) src/$@.c $(CFLAGS) $(MFLAGS) -lX11 -o $@
	@./$@

event_handling:
	@$(RM) $@
	@$(CC) src/$@.c $(CFLAGS) $(MFLAGS) -lX11 -o $@
	@./$@

create_window:
	@$(RM) $@
	@$(CC) src/$@.c $(CFLAGS) $(MFLAGS) -lX11 -o $@
	@./$@

font_drawing:
	@$(RM) $@
	@$(CC) src/$@.c $(CFLAGS) $(MFLAGS) -lX11 -o $@
	@./$@

draw_square:
	@$(RM) $@
	@$(CC) src/$@.c $(LIBGL) $(CFLAGS) $(MFLAGS) -lX11 -o $@ -lm
	@./$@

.PHONY: all build clean $(EXAMPLES)
