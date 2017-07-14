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
   glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

   if (!success)
   {
      char info[512];
      glGetShaderInfoLog(shader, 512, NULL, info);
      std::cout << "Error: Shader did not compile sucessfully. InfoLog = " << info << std::endl;
   }

   return shader;
}

unsigned int getProgram( std::string vertexShaderName, std::string fragmenShadername)
{
   unsigned int VertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderName);
   unsigned int FragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmenShadername);

   unsigned int program = glCreateProgram();
   glAttachShader(program, VertexShader);
   glAttachShader(program, FragmentShader);
   glLinkProgram(program);

   int success;
   glGetProgramiv(program, GL_LINK_STATUS, &success);
   
   if (!success)
   {
      char info[512];
      glGetProgramInfoLog(program, 512, NULL, info);
      std::cout << "Error: Program did not link successfully. InfoLog = " << info << std::endl;
   }

   glDeleteShader(VertexShader);
   glDeleteShader(FragmentShader);

   return program;
}