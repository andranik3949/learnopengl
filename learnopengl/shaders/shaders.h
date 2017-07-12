#pragma once

#include <fstream>
#include <string>

std::string getShader(std::string shadername)
{
   std::ifstream shaderStream("shaders/" + shadername + ".glsl");
   return std::string(std::istreambuf_iterator<char>(shaderStream), std::istreambuf_iterator<char>());
}