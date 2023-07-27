#include <X11/Xlib.h>
#include <X11/Xutil.h> // XSizeHints
#include <stdio.h>     // fprintf() snprintf()
#include <string.h>    // strlen()

#define WINDOW_TITLE  "X11 Window Example"
#define FONTNAME      "12x24"
#define WINDOW_WIDTH  720
#define WINDOW_HEIGHT 480
#define BUFFER_SIZE   32

#ifdef __MACOS__
#define KEY_ESCAPE 61
#else
#define KEY_ESCAPE 9
#endif

static Display* display;
static int screen;
static Window root;
static Window window;
static GC gc;
static XFontStruct* font;
static unsigned long black_color;
static unsigned long white_color;
static XEvent event;
static Atom wm_delete_window;

void get_window_size(unsigned short* width, unsigned short* height)
{
  XWindowAttributes attributes;
  XGetWindowAttributes(display, window, &attributes);
  *width = attributes.width;
  *height = attributes.height;
}

void get_text_center_pos(int* x, int* y, const char* restrict text)
{
  unsigned short width;
  unsigned short height;
  get_window_size(&width, &height);

  int font_direction, font_ascent, font_descent;
  XCharStruct overall;
  XTextExtents(font,
    text, strlen(text),
    &font_direction,
    &font_ascent, &font_descent,
    &overall);
  *x = (width - overall.width) / 2;
  *y = (height - (font_ascent + font_descent)) / 2;
}

void draw_text(const char* restrict text)
{
  int x, y;
  get_text_center_pos(&x, &y, text);
  XClearWindow(display, window);
  XDrawString(display, window, gc,
    x, y, text, strlen(text));
}

int main(void)
{
  // init basics
  display = XOpenDisplay(NULL);

  if (!display)
  {
    fprintf(stderr, "error:"
      "	Can't open connection to display server."
      " Probably X server is not started.\n");
    return 1;
  }

  screen = DefaultScreen(display);
  black_color = BlackPixel(display, screen);
  white_color = WhitePixel(display, screen);
  root = RootWindow(display, screen);

  // create window
  window = XCreateSimpleWindow(display, root,
    0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
    0, black_color,
    black_color);

  XSelectInput(display, window,
    ExposureMask | KeyPressMask | ButtonPressMask);

  // set window title
  XStoreName(display, window, WINDOW_TITLE);

  // load font
  font = XLoadQueryFont(display, FONTNAME);
  XGCValues gcv = {
    .foreground = white_color,
    .background = white_color,
    .font       = font->fid
	};

  // create graphic context
  gc = XCreateGC(display, window,
    GCForeground | GCBackground | GCFont,
    &gcv);

  // add close event listener
  wm_delete_window = XInternAtom(display,
    "WM_DELETE_WINDOW", False);
  XSetWMProtocols(display, window, &wm_delete_window, 1);

  // set window fixed
  XSizeHints hints;
  XWindowAttributes attributes;

  XGetWindowAttributes(display, window, &attributes);

  hints.flags = PMinSize | PMaxSize;
  hints.min_width = hints.max_width = attributes.width;
  hints.min_height = hints.max_height = attributes.height;
  XSetWMNormalHints(display, window, &hints);

  // make window visible
  XMapWindow(display, window);


  // event loop

  char text[BUFFER_SIZE];

  for (int loop = 1; loop; )
  {
    XNextEvent(display, &event);

    switch (event.type)
    {
      case Expose:
      {
        snprintf(text, BUFFER_SIZE, "%s", "PRESS ANY KEY");
        draw_text(text);
        break;
      }

      case KeyPress:
      {
        snprintf(text, BUFFER_SIZE, "KEY %d", event.xkey.keycode);
        draw_text(text);
        if (event.xkey.keycode == KEY_ESCAPE)
          loop = 0;
        break;
      }

      case ButtonPress:
      {
        snprintf(text, BUFFER_SIZE, "BUTTON %d (%d %d)",
          event.xbutton.button, event.xbutton.x, event.xbutton.y);
        draw_text(text);
        if (event.xkey.keycode == KEY_ESCAPE)
          loop = 0;
        break;
      }

      case ClientMessage:
      {
        if ((Atom) event.xclient.data.l[0] == wm_delete_window)
          loop = 0;
        break;
      }
    }

  }


  // cleanup

  XFreeGC(display, gc);
  XFreeFont(display, font);
  XUnmapWindow(display, window);
  XDestroyWindow(display, window);
  XCloseDisplay(display);
}
