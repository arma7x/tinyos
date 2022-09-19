#ifndef TYPES_H
#define TYPES_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef void (*KeyEvent) ();

typedef struct module
{
  void (*Init) (int, ...);
  void (*Destroy) ();
  KeyEvent KeyUp;
  KeyEvent KeyDown;
  KeyEvent KeyRight;
  KeyEvent KeyLeft;
  KeyEvent KeyMid;
  KeyEvent KeySet;
  KeyEvent KeyReset;
} Module;

typedef struct menu {
  const unsigned char* icon;
  char* title;
} Menu;

#ifdef __cplusplus
}
#endif

#endif
