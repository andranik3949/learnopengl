#pragma once

#include <fstream>
#include <string>

unsigned int compileShader(GLenum type, std::string shadername)
{
   unsigned int shader;
   shader = glCreateShader(type);

   std::ifstream shaderStream("shaders/" + shadername + ".glsl");
   std::string shaderCode = std::string(std::istreambuf_iterator<char>(shaderStream), std::istreambuf_iterator<char>());
   const char* shaderCodePtr = shaderCode.c_str();

   glShaderSource(shader, 1, &shaderCodePtr, NULL);
   glCompileShader(shader);

   int success;
   char buf[512];
   glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

   if (!success)
   {
      glGetShaderInfoLog(shader, 512, NULL, buf);
      std::cout << "Error: Shader did not compile sucessfully, InfoLog = " << buf << std::endl;
   }

   return shader;
}