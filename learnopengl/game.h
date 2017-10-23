#pragma once

#include "board.h"
#include "utilities.h"

class Game
{
public:
   Game() {}
   ~Game() {}

   void init()
   { }

   void advanceStep()
   { }

private:
   Board m_board;
};