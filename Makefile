CC := gcc
RM := rm -rf
CFLAGS := -Wall -Werror -Wextra -O3
MFLAGS := -I/opt/X11/include -L/opt/X11/lib
PROJECTS := simple_window \
	event_handling \
	create_window \
	font_drawing

all: clean build

build: clean
	@$(CC) simple_window.c  $(CFLAGS) $(MFLAGS) -lX11 -o simple_window
	@$(CC) event_handling.c $(CFLAGS) $(MFLAGS) -lX11 -o event_handling
	@$(CC) create_window.c  $(CFLAGS) $(MFLAGS) -lX11 -o create_window
	@$(CC) font_drawing.c   $(CFLAGS) $(MFLAGS) -lX11 -o font_drawing

clean:
	@$(RM) $(PROJECTS)

simple_window:
	@$(RM) $@
	@$(CC) $@.c $(CFLAGS) $(MFLAGS) -lX11 -o $@
	@./$@

event_handling:
	@$(RM) $@
	@$(CC) $@.c $(CFLAGS) $(MFLAGS) -lX11 -o $@
	@./$@

create_window:
	@$(RM) $@
	@$(CC) $@.c $(CFLAGS) $(MFLAGS) -lX11 -o $@
	@./$@

font_drawing:
	@$(RM) $@
	@$(CC) $@.c $(CFLAGS) $(MFLAGS) -lX11 -o $@
	@./$@

.PHONY: all build clean $(PROJECTS)
