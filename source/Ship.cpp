//
//  Ship.cpp
//  Asteroids
//
//  Created by Brian Summa on 6/5/15.
//
//

#include "common.h"
#include "Bullet.hpp"
#include "Bullets.hpp"
#include "math.h"

Ship::Ship(){
  //!!!!!!!!Your initial state might be different depending on how you
  //pick your world coordinates
  state.cur_location = vec2(0.0,0.0);
  state.pointing = vec2(1.0,0.0);
  state.move = vec2(0.0,0.0);
  state.thruster_on = false;
    state.max_power = false;
  state.angle = 0.0;
};


void Ship::update_state(){

//    float xN = static_cast<float>(cos(2* 3.14159f * (state.angle / 360)));
//    float yN = static_cast<float>(sin(2* 3.14159f * (state.angle / 360)));
//    vec2 direction = vec2(xN,yN);
    if(state.thruster_on){
        if(pow(state.move.x,2) < 14*14 && pow(state.move.y,2) < 14*14){
            state.move += state.pointing*0.8;
        }
        else{
            state.move = state.pointing*14;
        }
    //!!!!!!!!Accelerate if the thruster is on
    //!!!!!!!!Clamp acceleration at some maximum value
  }
  else{
      state.move -= state.move*0.1;
  }
    
    state.cur_location += state.move;
  //!!!!!!!!Dampen the velocity at every timestep to lessen intertia
  //!!!!!!!!Move the ship location
  
  //Wrap the ship position when at the boundary
  //!!!!!!!!This will change depending on the world coordinates you are using
    if(state.cur_location.x < -width ||state.cur_location.x > width){
        state.cur_location.x = -state.cur_location.x;
    }
  if(state.cur_location.y < -height ||state.cur_location.y > height){
    state.cur_location.y = -state.cur_location.y;
  }

    
}


void Ship::gl_init(){
  //Ship
  //!!!!!!!!Populate ship_vert and ship_color
    
    vec2 ship_vert[10] = {
        vec2(16,0),vec2(0,-7.5),vec2(5,0),vec2(0,7.5),
        vec2(1.5,1),vec2(5,0),vec2(1.5,-1),vec2(-5,-1),vec2(-7.5,0),vec2(-5,1)
    };
    vec3 ship_color[10] = {
      vec3(1,1,1),vec3(1,1,1),vec3(1,1,1),vec3(1,1,1),
        vec3(1,0,0),vec3(1,0,0),vec3(1,0,0),vec3(1,0,0),vec3(1,0,0),vec3(1,0,0)
    };
  
  std::string vshader = shader_path + "vshader_Ship.glsl";
  std::string fshader = shader_path + "fshader_Ship.glsl";
  
  GLchar* vertex_shader_source = readShaderSource(vshader.c_str());
  GLchar* fragment_shader_source = readShaderSource(fshader.c_str());
  
  GLvars.vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(GLvars.vertex_shader, 1, (const GLchar**) &vertex_shader_source, NULL);
  glCompileShader(GLvars.vertex_shader);
  check_shader_compilation(vshader, GLvars.vertex_shader);
  
  GLvars.fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(GLvars.fragment_shader, 1, (const GLchar**) &fragment_shader_source, NULL);
  glCompileShader(GLvars.fragment_shader);
  check_shader_compilation(fshader, GLvars.fragment_shader);
  
  GLvars.program = glCreateProgram();
  glAttachShader(GLvars.program, GLvars.vertex_shader);
  glAttachShader(GLvars.program, GLvars.fragment_shader);
  
  glLinkProgram(GLvars.program);
  check_program_link(GLvars.program);
  
  glBindFragDataLocation(GLvars.program, 0, "fragColor");
  
  GLvars.vpos_location   = glGetAttribLocation(GLvars.program, "vPos");
  GLvars.vcolor_location = glGetAttribLocation(GLvars.program, "vColor" );
  GLvars.M_location = glGetUniformLocation(GLvars.program, "M" );
  
  // Create a vertex array object
  glGenVertexArrays( 1, &GLvars.vao );
  //Set GL state to use vertex array object
  glBindVertexArray( GLvars.vao );
  
  //Generate buffer to hold our vertex data
  glGenBuffers( 1, &GLvars.buffer );
  //Set GL state to use this buffer
  glBindBuffer( GL_ARRAY_BUFFER, GLvars.buffer );
  
  //Create GPU buffer to hold vertices and color
  glBufferData( GL_ARRAY_BUFFER, sizeof(ship_vert) + sizeof(ship_color), NULL, GL_STATIC_DRAW );
  //First part of array holds vertices
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(ship_vert), ship_vert );
  //Second part of array hold colors (offset by sizeof(triangle))
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(ship_vert), sizeof(ship_color), ship_color );
  
  glEnableVertexAttribArray(GLvars.vpos_location);
  glEnableVertexAttribArray(GLvars.vcolor_location );
  
  glVertexAttribPointer( GLvars.vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  glVertexAttribPointer( GLvars.vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(ship_vert)) );
  
  glBindVertexArray(0);

}

void Ship::fire(){
    cluster.add(state.cur_location, state.pointing, state.move);
}

vec2 Ship::get_location(){
    return state.cur_location;
}

void Ship::crash(bool crash){
    state.collision = crash;
}

bool Ship::crashed(){
    return state.collision;
}

void Ship::ship_reset(){
    state.cur_location = vec2(0,0);
    state.move = vec2(0,0);
}

void Ship::draw(mat4 proj){
  
  glUseProgram( GLvars.program );
    glBindVertexArray( GLvars.vao );
  
  //!!!!!!!!Create a modelview matrix and pass it
    mat4 matrix = Ortho2D(-1, 1, -1, 1);
    mat4 movement= Translate(GLfloat(state.cur_location.x), GLfloat(state.cur_location.y), GLfloat(0))*Angel::RotateZ(GLfloat(state.angle));
    mat4 scale = Scale(GLfloat(2),GLfloat(2),GLfloat(2));
  glUniformMatrix4fv( GLvars.M_location, 1, GL_TRUE, proj*matrix*movement*scale);
  
    glDrawArrays(GL_LINE_LOOP, 0, 4);
  //!!!!!!!!Draw something
  
  if(state.thruster_on){
      glDrawArrays(GL_LINE_LOOP, 4, 6);
    //!!!!!!!!Maybe draw something different if the thruster is on
  }
  
  glBindVertexArray(0);
  glUseProgram(0);

}
