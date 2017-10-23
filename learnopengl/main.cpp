#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "texture.h"
#include "shader/shader.h";

#include "board.h";

namespace
{
   int window_width = 1920;
   int window_height = 1080;

   float gamma = 0.0f;
   float clearR = 0.2f, clearG = 0.3f, clearB = 0.3f;

   float lastFrame = 0.0f;
   float deltaTime = 0.0f;

   bool cursorFirstEntry = true;

   float camFOV = 70.0f;

   float camPitch = 0.0f;
   float camYaw = -90.0f;

   float camLastX = window_width / 2;
   float camLastY = window_height / 2;

   glm::vec3 camPos(0.0f, 0.0f, 3.0f);
   glm::vec3 camFront(0.0f, 0.0f, -1.0f);
   glm::vec3 camUp(0.0f, 1.0f, 0.0f);

   glm::mat4 projection;
   glm::mat4 view;

   Board testBoard(8, 8);

   bool isSelected = false;
   int selectedX = -1;
   int selectedY = -1;

   int highlightedX = -1;
   int highlightedY = -1;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
   window_width = width;
   window_height = height;
   glViewport(0, 0, window_width, window_height);
}

void mouse_leftclick_callback(GLFWwindow* window, int button, int mode, int flags)
{
   if (mode == GLFW_RELEASE)
   {
      std::cout << "CamPos   <" << camPos.x << ',' << camPos.y << ',' << camPos.z << ">" << std::endl;
      std::cout << "CamFront <" << camFront.x << ',' << camFront.y << ',' << camFront.z << ">" << std::endl;
      
      if (isSelected)
      {
         int toX = camPos.x + camFront.x * (-0.5f - camPos.z) / camFront.z + 0.5f;
         int toY = camPos.y + camFront.y * (-0.5f - camPos.z) / camFront.z - 0.5f;

         std::cout << "To rectangle <" << -toY << ", " << toX << "> " << std::endl;

         testBoard.switchBalls(Coords(-selectedY, selectedX), Coords(-toY, toX));

         isSelected = false;
         return;
      }
      
      

      //----------Initial------Normal-------------------Distance----------Offset--
      selectedX = camPos.x + camFront.x * (-0.5f - camPos.z) / camFront.z + 0.5f;
      selectedY = camPos.y + camFront.y * (-0.5f - camPos.z) / camFront.z - 0.5f;

      isSelected = true;
      std::cout << "Selected recatngle <" << -selectedY << ", " << selectedX << "> " << std::endl;
   }
}

void mouse_movement_callback(GLFWwindow* window, double xPos, double yPos)
{
   if (cursorFirstEntry)
   {
      camLastX = xPos;
      camLastY = yPos;

      cursorFirstEntry = false;
   }

   float sensitivity = 0.05f;
   
   camPitch += (yPos - camLastY) * sensitivity;
   camLastY = yPos;
   
   camYaw += (xPos - camLastX) * sensitivity;
   camLastX = xPos;

   if (camPitch > 89.9f)
      camPitch = 89.9f;
   if (camPitch < -89.9f)
      camPitch = -89.9f;

   glm::vec3 front;
   front.x = cos(glm::radians(camYaw)) * cos(glm::radians(camPitch));
   front.y = -sin(glm::radians(camPitch));
   front.z = sin(glm::radians(camYaw)) * cos(glm::radians(camPitch));

   camFront = glm::normalize(front);

   highlightedX = camPos.x + camFront.x * (-0.5f - camPos.z) / camFront.z + 0.5f;
   highlightedY = camPos.y + camFront.y * (-0.5f - camPos.z) / camFront.z - 0.5f;
}

void mouse_wheel_callback(GLFWwindow* window, double offsetX, double offsetY)
{
   if (camFOV >= 1.0f && camFOV <= 70.0f)
   {
      camFOV -= offsetY;
   }
   if (camFOV < 1.0f)
   {
      camFOV = 1.0f;
   }
   if (camFOV > 70.0f)
   {
      camFOV = 70.0f;
   }
}

void processInput(GLFWwindow* window, Shader& program)
{
   if( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS )
   {
      glfwSetWindowShouldClose(window, true);
   }

   float cameraSpeed = deltaTime * 1.5f;
   glm::vec3 cam(camFront.x, camFront.y ,camFront.z);
   if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
   {
      camPos += cameraSpeed * cam;
      mouse_movement_callback(window, camLastX, camLastY);
   }
   if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
   {
      camPos -= cameraSpeed * cam;
      mouse_movement_callback(window, camLastX, camLastY);
   }
   if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
   {
      camPos -= glm::normalize(glm::cross(cam, camUp)) * cameraSpeed;
      mouse_movement_callback(window, camLastX, camLastY);
   }
   if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
   {
      camPos += glm::normalize(glm::cross(cam, camUp)) * cameraSpeed;
      mouse_movement_callback(window, camLastX, camLastY);
   }

   /*
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
   */
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

   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   glfwSetCursorPosCallback(window, mouse_movement_callback);

   glfwSetScrollCallback(window, mouse_wheel_callback);

   glfwSetMouseButtonCallback(window, mouse_leftclick_callback);

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

   glm::vec3 colors[] = 
   {
      glm::vec3(0.24f,  0.37f,  0.59f),
      glm::vec3(0.85f,  0.62f,  0.03f),
      glm::vec3(0.49f,  0.71f,  0.07f),
      glm::vec3(0.49f,  0.09f,  0.57f),
      glm::vec3(0.57f,  0.09f,  0.09f)
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

   Shader shader("shader/basic.slvs", "shader/basic.slfs");
   glEnable(GL_DEPTH_TEST);

   //testBoard.test();

   while (!glfwWindowShouldClose(window))
   {
      float currentTime = glfwGetTime();
      deltaTime = currentTime - lastFrame;
      lastFrame = currentTime;

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
      
      //glm::mat4 ortho = glm::ortho(0.0f, (float)window_width, 0.0f, (float)window_height, 0.1f, 100.0f);
      projection = glm::perspective(glm::radians(camFOV), (float)window_width / window_height, 0.05f, 100.0f);
      glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

      view = glm::lookAt(camPos, camPos + camFront, camUp);
      glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "view"), 1, GL_FALSE, glm::value_ptr(view));

      std::vector<std::vector<int>> table = testBoard.getTable();

      glm::mat4 rectacleModel;
      shader.setUniform("color", glm::vec3(0.5f, 0.5f, 0.5f));
      rectacleModel = glm::translate(rectacleModel, camPos + camFront * 0.1f);
      rectacleModel = glm::scale(rectacleModel, glm::vec3(0.001f, 0.001f, 0.001f));
         
      glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "model"), 1, GL_FALSE, glm::value_ptr(rectacleModel));
      glDrawArrays(GL_TRIANGLES, 0, 36);

      for(int i = 0; i < table.size(); ++i)
      {
         for (int j = 0; j < table[i].size(); ++j)
         {
            glm::mat4 model;
            model = glm::translate(model, glm::vec3(j, -i, -1.0f));
            if (isSelected && j == selectedX && i == -selectedY)
            {
               shader.setUniform("color", glm::vec3(1.0f, 0.0f, 0.0f));
            }
            else if(j == highlightedX && i == -highlightedY)
            {
               shader.setUniform("color", glm::mix(colors[table[i][j]], glm::vec3(1.0f, 1.0f, 1.0f), 0.5f));
            }
            else
            {
               shader.setUniform("color", colors[table[i][j] - 1]);
            }
            //model = glm::rotate(model, i % 3 * (float)glfwGetTime(), glm::vec3(i / 10.0f, 1 - i / 10.0f, 0.0f));
            glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
         }
      }
      //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      glfwPollEvents();
      glfwSwapBuffers(window);
   }


   glfwTerminate();
   return 0;
}