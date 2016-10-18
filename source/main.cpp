#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "common.h"
#include "Asteroid.h"
#include "shader.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <chrono>
#include <thread>

using namespace Angel;

Asteroids asteroids;
Ship ship;
int lives = 3;

static void error_callback(int error, const char* description)
{
  fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        ship.rotateLeft();
    }
  if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    ship.rotateRight();
  if (key == GLFW_KEY_SPACE){
    if(action == GLFW_PRESS){
      ship.start_thruster();
    }
    if(action == GLFW_RELEASE){
      ship.stop_thruster();
    }
  }
  if (key == GLFW_KEY_Z && action == GLFW_PRESS){
      ship.fire();
      //asteroids.change();
  }
}

void init(){
  
    
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
  glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);
  ship.gl_init();
    ship.cluster.gl_init();
  asteroids.gl_init();
    asteroids.start();

}

void animate(){
  if(glfwGetTime() > 0.033){
    glfwSetTime(0.0);
    ship.update_state();
    asteroids.update_state();
    if(ship.cluster.bullets.size() != 0){
        if(asteroids.bullet_hit(ship.cluster.return_first()) == true){
            ship.cluster.delete_first();
        };
    }
    ship.cluster.step();
    ship.crash(asteroids.ship_crash(ship.get_location()));
  }
}

int main(void)
{
  GLFWwindow* window;
  
  glfwSetErrorCallback(error_callback);
  
  if (!glfwInit())
    exit(EXIT_FAILURE);
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  glfwWindowHint(GLFW_SAMPLES, 10);
  
  
  window = glfwCreateWindow(1024, 768, "Asteroids!", NULL, NULL);
  if (!window){
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  
  glfwSetKeyCallback(window, key_callback);
  
  glfwMakeContextCurrent(window);
  gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
  glfwSwapInterval(1);
  
  init();
  
  while (!glfwWindowShouldClose(window)){
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    //!!!!!!!!Pick a projection that makes the most sense to you
    ship.set_extents(width, height);
    asteroids.set_extents(width, height);
      ship.cluster.set_extents(width, height);
    mat4 proj = Ortho2D(-width/2, width/2, height/2, -height/2);
    animate();
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    ship.draw(proj);
      if(asteroids.asteroids.size() == 0){
          std::cout << "Great Job! You Win!" << std::endl;
      }
    asteroids.draw(proj);
      if(ship.cluster.active() == true){
          ship.cluster.update();
          ship.cluster.draw(proj);
      }
      
      if(ship.crashed() == true){ 
          std::cout << "crash!" << std::endl;
          if(lives > 1){
              lives--;
              //Do animation
              ship.ship_reset();
              ship.draw(proj);
              std::this_thread::sleep_for(std::chrono::seconds(1));
          }
          else{
              std::cout << "Game Over" << std::endl;
              break;
          }
      }
    glfwSwapBuffers(window);
    glfwPollEvents();
    
  }
  
  glfwDestroyWindow(window);
  
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
