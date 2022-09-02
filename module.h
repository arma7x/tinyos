#ifndef MODULE_H
#define MODULE_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef void (*KeyEvent) ();

typedef struct module
{
  void (*Init) (int, ...);
  KeyEvent KeyUp;
  KeyEvent KeyDown;
  KeyEvent KeyRight;
  KeyEvent KeyLeft;
  KeyEvent KeyMid;
  KeyEvent KeySet;
  KeyEvent KeyReset;
} Module;

void ModuleSwitcher(Module);
Module GetActiveModule();

#ifdef __cplusplus
}
#endif

#endif
