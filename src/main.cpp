#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "game/DemoGame.hpp"

int main(int ArgCount, char **Args)
{
  DemoGame game;
  game.Run();
  return 0;
}