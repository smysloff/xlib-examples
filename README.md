# XLib - First Steps

Code examples for simple applications using **Xlib** to work with windows on Unix-like operating systems with **X Window System**.


# Translations

* [русский](https://github.com/smysloff/xlib-examples/blob/master/README.ru.md)


# Description

**X Window System** is a client-server system for managing the display of windows for raster displays, widely used in Unix-like operating systems. **X** provides the basis for a graphical environment (GUI), handling the drawing and movement of windows on the screen, as well as interaction with the mouse and keyboard.

**X11** is the name of the current version of the **X Window System protocol**.

**Xlib (X Window System library)** is a library that is part of the **X Window System** and implements **X11**. It provides access to low-level graphics system functions, allowing applications to work directly with window display and screen graphics content.


#Code Examples

Here are some examples of how to use the **Xlib library**. These examples gradually reveal the basic concepts of working with a graphics server and solve typical tasks of building a graphical application.

All examples can be compiled with one command: `make`.

Each example can be compiled and run separately. The examples and how to run them are described in detail below.


* ### Simple Window

```sh
make simple_window
```

Minimalistic example showing how to connect to the graphics server and open a window using the **XCreateSimpleWindow()** function.


* ### Event Handling

```sh
make event_handling
```

Minimalistic example showing how to handle simple events. In this case, the window is closed when the *Escape* key is pressed.


* ### Create Window

```sh
make create_window
```

Simple example demonstrating creating a window using the **XCreateWindow()** function, setting the window title, drawing a square in it, and handling window closing in the window manager.


* ### Font Drawing

```sh
make font_drawing
```

Simple example demonstrating basic work with fonts.


* ### Draw Square

```sh
make draw_square
```

Example in which a rotating rectangle is drawn in a window, which rotation can be controlled using the keys *A*, *D*, *Left Arrow*, *Right Arrow*, *Space*.


## Useful links
* [Xorg Documentation](https://www.x.org/releases/current/doc/index.html)
* [Tronche's The Xlib Programming Manual](https://tronche.com/gui/x/xlib/)
* [Разработка программ для системы X Window](http://www.asvcorp.ru/tech/linux/xwinprg/index.html)
* [Hilltopia's Xlib tutorial](http://xopendisplay.hilltopia.ca/2009/Jan/Xlib-tutorial-part-1----Beginnings.html)
* [Программирование в среде X Window на основе библиотеки Xlib](https://dfe.petrsu.ru/koi/posob/X/index.html)
* [Основы программирования в системе X Window](https://www.opennet.ru/docs/RUS/xtoolkit/x-2.html)


## License

[GNU GPL-3.0](https://raw.githubusercontent.com/smysloff/xlib-examples/master/LICENSE)
