#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "texture.h"
#include "shader\shader.h";


int window_width = 800;
int window_height = 600;

float gamma = 0.0f;

glm::vec3 camPos(0.0f, 0.0f, 3.0f);
glm::vec3 camFront(0.0f, 0.0f, -1.0f);
glm::vec3 camUp(0.0f, 1.0f, 0.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
   window_width = width;
   window_height = height;
   glViewport(0, 0, window_width, window_height);
}

void processInput(GLFWwindow* window, Shader& program)
{
   if( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS )
   {
      glfwSetWindowShouldClose(window, true);
   }

   float cameraSpeed = 0.05f; // adjust accordingly
   if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      camPos += cameraSpeed * camFront;
   if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      camPos -= cameraSpeed * camFront;
   if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      camPos -= glm::normalize(glm::cross(camFront, camUp)) * cameraSpeed;
   if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      camPos += glm::normalize(glm::cross(camFront, camUp)) * cameraSpeed;
   
   if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
   {
      gamma += 0.05f;
      /*if (gamma > 1.0f)
      {
         gamma = 1.0f;
      }*/
   }
   if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
   {
      gamma -= 0.05f;
      /*if (gamma < 0.0f)
      {
         gamma = 0.0f;
      }*/
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
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
   };
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   glm::vec3 cubePositions[] = {
      glm::vec3(0.0f,  0.0f,  0.0f),
      glm::vec3(2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),
      glm::vec3(1.5f,  2.0f, -2.5f),
      glm::vec3(1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
   };

   /*
   unsigned int EBO;
   glGenBuffers(1, &EBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

   unsigned int indices[] = {
      0, 1, 2,
      1, 2, 3
   };
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
   */

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);

   Texture containter("resources/container.jpg", GL_RGB);
   Texture awsomeface("resources/awesomeface.png", GL_RGBA);

   glBindVertexArray(0);

   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   float clearR = 0.2f, clearG = 0.3f, clearB = 0.3f;

   Shader shader("shader/basic.slvs", "shader/basic.slfs");
   glEnable(GL_DEPTH_TEST);

   while( !glfwWindowShouldClose(window) )
   { 
      processInput(window, shader);

      glClearColor(clearR, clearG, clearB, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	   shader.use();
	   shader.setUniform("ourTexture1", 0);
	   shader.setUniform("ourTexture2", 1);
      shader.setUniform("gamma", gamma);

      glBindVertexArray(VAO);
	   containter.bind(GL_TEXTURE0);
	   awsomeface.bind(GL_TEXTURE1);


      glm::mat4 projection;
      projection = glm::perspective(glm::radians(70.0f), (float)window_width / window_height, 0.05f, 100.0f);
      glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
     
      glm::mat4 view;
      view = glm::lookAt(camPos, camPos + camFront, camUp);
      glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "view"), 1, GL_FALSE, glm::value_ptr(view));

      for (int i = 0; i < 10; ++i)
      {
         glm::mat4 model;
         model = glm::translate(model, cubePositions[i]);
         model = glm::rotate(model, i % 3 * (float)glfwGetTime(), glm::vec3(i / 10.0f, 1 - i/10.0f, 0.0f));
         glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
         
         glDrawArrays(GL_TRIANGLES, 0, 36);
      }
      //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      glfwPollEvents();
      glfwSwapBuffers(window);
   }

   glfwTerminate();
   return 0;
}