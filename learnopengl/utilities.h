#pragma once

struct Coords
{
   Coords()
   { }

   Coords(int x, int y)
      :x(x), y(y)
   { }

   int x;
   int y;
};