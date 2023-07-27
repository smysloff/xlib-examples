// ==================================================================== //
//  _   _ _    ___ ___       ____ _   _      _   _ ___  _    ____  __   //
//   \ /  |     |  |__)      |__   \ /   /\  |\ /| |  \ |    |__  (__`  //
//    X   |     |  |  \      |      X   /__\ | V | |__/ |    |       \  //
//  _/ \_ |__/ _|_ |__/      |___ _/ \_ |  | |   | |    |__/ |___ \__/  //
//                                                                      //
// ==================================================================== //
// file:    gl_keys.h                                                   //
// author:  Alexander Smyslov                                           //
// e-mail:  kokoc.smyslov@yandex.ru                                     //
// website: https://github.com/smysloff/xlib-examples                   //
// license: GPL-3.0                                                     //
// ==================================================================== //

#ifndef __GL_KEYS_H__
#define __GL_KEYS_H__

#ifdef __MACOS__
  #define KEY_ESCAPE 61
  #define KEY_Q      20
  #define KEY_W      21
  #define KEY_E      22
  #define KEY_A      8
  #define KEY_S      9
  #define KEY_D      10
  #define KEY_UP     134
  #define KEY_LEFT   131
  #define KEY_DOWN   133
  #define KEY_RIGHT  132
  #define KEY_SPACE  57
  #define KEY_RETURN 44
#else
  #define KEY_ESCAPE 9
  #define KEY_Q      24
  #define KEY_W      25
  #define KEY_E      26
  #define KEY_A      38
  #define KEY_S      39
  #define KEY_D      40
  #define KEY_UP     111
  #define KEY_LEFT   113
  #define KEY_DOWN   116
  #define KEY_RIGHT  114
  #define KEY_SPACE  65
  #define KEY_RETURN 36
#endif

#endif
