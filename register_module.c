#include <stdarg.h>
#include "register_module.h"

#ifdef __cplusplus
 extern "C" {
#endif


static Module ActiveModule;

void ModuleSwitcher(Module m ) {
  ActiveModule = m;
}

Module GetActiveModule() {
  return ActiveModule;
}


#ifdef __cplusplus
}
#endif
