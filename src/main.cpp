#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "engine_core/XJZoomEngine.h"

int main(int ArgCount, char **Args)
{
  XJZoomEngine engine;
  engine.Init();
  engine.Run();
  return 0;
}