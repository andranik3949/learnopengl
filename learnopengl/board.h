#pragma once

#include <iostream>

#include <vector>
#include <algorithm>

#include <glm/glm.hpp>

#include <cstdlib>
#include <ctime>

#include "utilities.h"

class Board
{
public:
   Board(int height, int width)
      :m_board(height, std::vector<int>(width))
   {
      srand(time(0));
      for(int i = 0; i < m_board.size(); ++i)
      {
         for (int j = 0; j < m_board.size(); ++j)
         {
            m_board[i][j] = rand() % 5 + 1;
         }
      }
   }

   ~Board()
   { }

   Coords findBall( glm::vec3 position )
   {

   }

   bool switchBalls( Coords from, Coords to )
   {
      std::swap(m_board[from.x][from.y], m_board[to.x][to.y]);
      if (!isCombination(to) && !isCombination(from))
      {
         std::swap(m_board[from.x][from.y], m_board[to.x][to.y]);
         return false;
      }

      std::cout << "Switched!" << std::endl;
      return true;
   }

   void print()
   {
      for (int i = 0; i < m_board.size(); ++i)
      {
         for (int j = 0; j < m_board.size(); ++j)
         {
            std::cout << m_board[i][j] << ' ';
         }
         std::cout << std::endl;
      }
   }

   const std::vector<std::vector<int>>& getTable()
   {
      return m_board;
   }

private:
   bool isCombination(const Coords& pos)
   {
      std::cout << "Processing <" << pos.x << ", " << pos.y << ">" << std::endl;

      int vertical = 0, current = 0;
      for (int i = std::max(pos.x - 2, 0); i <= std::min(pos.x + 2, (int)m_board.size() - 1); ++i)
      {
         std::cout << "Checking <" << i << ", " << pos.y << "> Value: <" << m_board[i][pos.y] << ">" << std::endl;
         if (m_board[i][pos.y] == m_board[pos.x][pos.y])
         {
            ++current;
            if (current > vertical)
            {
               vertical = current;
            }
         }
         else
         {
            current = 0;
         }
      }
      std::cout << "Vertical <" << vertical << ">" << std::endl;

      int horizontal = 0; 
      current = 0;
      for (int i = std::max(pos.y - 2, 0); i <= std::min(pos.y + 2, (int)m_board[pos.x].size() - 1); ++i)
      {
         std::cout << "Checking <" << pos.x << ", " << i << "> Value: <" << m_board[pos.x][i] << ">" << std::endl;
         if (m_board[pos.x][i] == m_board[pos.x][pos.y])
         {
            ++current;
            if (current > horizontal)
            {
               horizontal = current;
            }
         }
         else
         {
            current = 0;
         }
      } 
      std::cout << "Horizontal <" << horizontal << ">" << std::endl;

      return vertical >= 3 || horizontal >= 3;
   }

   std::vector<std::vector<int>> m_board;
};