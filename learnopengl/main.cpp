#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "texture.h"
#include "shader\shader.h";


const int window_width = 800;
const int window_height = 600;

float gamma = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
   glViewport(10, 10, width - 10, height - 10);
}

void processInput(GLFWwindow* window, Shader& program)
{
   if( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS )
   {
      glfwSetWindowShouldClose(window, true);
   }
   if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
   {
      gamma += 0.05f;
      if (gamma > 1.0f)
      {
         gamma = 1.0f;
      }
   }
   if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
   {
      gamma -= 0.05f;
      if (gamma < 0.0f)
      {
         gamma = 0.0f;
      }
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
      -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
      -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
       0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
       0.5f,  0.5f, 0.0f,  0.0f, 0.5f, 1.0f,  1.0f, 1.0f
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

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
   glEnableVertexAttribArray(2);


   Texture containter("resources/container.jpg", GL_RGB);
   Texture awsomeface("resources/awesomeface.png", GL_RGBA);

   glBindVertexArray(0);

   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   float clearR = 0.2f, clearG = 0.3f, clearB = 0.3f;

   Shader shader("shader/basic.slvs", "shader/basic.slfs");
   
   while( !glfwWindowShouldClose(window) )
   { 
      processInput(window, shader);

      glClearColor(clearR, clearG, clearB, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

	   shader.use();
	   shader.setUniform("ourTexture1", 0);
	   shader.setUniform("ourTexture2", 1);
      shader.setUniform("gamma", gamma);

      glBindVertexArray(VAO);
	   containter.bind(GL_TEXTURE0);
	   awsomeface.bind(GL_TEXTURE1);

      float time = glfwGetTime();

      glm::mat4 trans;
      trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
      trans = glm::rotate(trans, (float)time, glm::vec3(0.0f, 0.6f, 0.8f));
      trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 1.0f));
      glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "transform"), 1, GL_FALSE, glm::value_ptr(trans));

      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      glm::mat4 trans1;
      trans1 = glm::translate(trans1, glm::vec3(-0.5f, 0.5f, 0.0f));
      trans1 = glm::rotate(trans1, (float)time, glm::vec3(0.8f, 0.0f, 0.6f));
      float movement = std::sin(time) / 4.0f + 0.5f;
      trans1 = glm::scale(trans1, glm::vec3(movement, movement, 1.0f));
      glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "transform"), 1, GL_FALSE, glm::value_ptr(trans1));

      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      glfwPollEvents();
      glfwSwapBuffers(window);
   }

   glfwTerminate();
   return 0;
}