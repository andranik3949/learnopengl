#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader\shader.h";


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

   unsigned int VAO;
   glGenVertexArrays(1, &VAO);
   glBindVertexArray(VAO);

   unsigned int VBO;
   glGenBuffers(1, &VBO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);

   float vertices[] = {
       0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
      -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
       0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
       0.5f, -0.5f, 0.0f,  0.0f, 0.5f, 1.0f
   };
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   unsigned int EBO;
   glGenBuffers(1, &EBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

   unsigned int indices[] = {
      0, 1, 2,
      1, 2, 3
   };
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);

   glBindVertexArray(0);

   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   float clearR = 0.2f, clearG = 0.3f, clearB = 0.3f;

   Shader shader("shader/basic.slvs", "shader/basic.slfs");
   while( !glfwWindowShouldClose(window) )
   { 
      processInput(window);

      glClearColor(clearR, clearG, clearB, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

	  shader.setUniform("offset", 0.0f);
	  shader.use();
	  
      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

      glfwPollEvents();
      glfwSwapBuffers(window);
   }

   glfwTerminate();
   return 0;
}