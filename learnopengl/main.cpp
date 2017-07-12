#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaders\shaders.h";


const int window_width = 800;
const int window_height = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
   glViewport(10, 10, width - 10, height - 10);
}

void processInput(GLFWwindow* window)
{
   if( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS )
   {
      glfwSetWindowShouldClose(window, true);
   }
}

int main()
{
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


   GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Test", NULL, NULL);
   if( window == NULL )
   {
      std::cout << "GLFW: Failed to create a window" << std::endl;
      glfwTerminate();
      return -1;
   }
   glfwMakeContextCurrent(window);
   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


   if( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) )
   {
      std::cout << "GLAD: Failed to load procedures" << std::endl;
      glfwTerminate();
      return -1;
   }



   unsigned int vertexShader;
   vertexShader = glCreateShader(GL_VERTEX_SHADER);

   std::string shader = getShader("basic_vertex");
   const char* shader_code = shader.c_str();
   glShaderSource(vertexShader, 1, &shader_code, NULL);
   glCompileShader(vertexShader);

   int success;
   char buf[512];
   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

   if( !success )
   {
      glGetShaderInfoLog(vertexShader, 512, NULL, buf);
      std::cout << "Error: Shader did not compile sucessfully, InfoLog = " << buf << std::endl;
   }

   while( !glfwWindowShouldClose(window) )
   { 
      processInput(window);

      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glfwPollEvents();
      glfwSwapBuffers(window);
   }

   glfwTerminate();
   return 0;
}