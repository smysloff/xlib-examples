CC := gcc
RM := rm -rf
LIBGL := src/gl.c
CFLAGS := -Wall -Werror -Wextra -O3
MFLAGS := -I/opt/X11/include -L/opt/X11/lib
EXAMPLES := simple_window \
	event_handling \
	create_window \
	font_drawing \
	draw_rect

all: clean build

build: clean
	@$(CC) src/simple_window.c $(CFLAGS) -o simple_window $(MFLAGS) -lX11 
	@$(CC) src/event_handling.c $(CFLAGS) -o event_handling $(MFLAGS) -lX11 
	@$(CC) src/create_window.c $(CFLAGS) -o create_window $(MFLAGS) -lX11
	@$(CC) src/font_drawing.c $(CFLAGS) -o font_drawing $(MFLAGS) -lX11 
	@$(CC) src/draw_rect.c $(LIBGL) $(CFLAGS) -o draw_rect $(MFLAGS) -lX11 -lm

clean:
	@$(RM) $(EXAMPLES)

simple_window:
	@$(RM) $@
	@$(CC) src/$@.c $(CFLAGS) -o $@ $(MFLAGS) -lX11
	@./$@

event_handling:
	@$(RM) $@
	@$(CC) src/$@.c $(CFLAGS) -o $@ $(MFLAGS) -lX11
	@./$@

create_window:
	@$(RM) $@
	@$(CC) src/$@.c $(CFLAGS) -o $@ $(MFLAGS) -lX11
	@./$@

font_drawing:
	@$(RM) $@
	@$(CC) src/$@.c $(CFLAGS) -o $@ $(MFLAGS) -lX11
	@./$@

draw_rect:
	@$(RM) $@
	@$(CC) src/$@.c $(LIBGL) $(CFLAGS) -o draw_rect $(MFLAGS) -lX11 -lm
	@./$@

.PHONY: all build clean $(EXAMPLES)
