//
//  Bullets.cpp
//  ASTEROIDS
//
//  Created by Danny Verb on 10/6/16.
//
//
#include "common.h"
#include "Bullets.hpp"

void Bullets::gl_init(){
    
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
    
    glEnableVertexAttribArray(GLvars.vpos_location);
    glEnableVertexAttribArray(GLvars.vcolor_location );
    
    glBindVertexArray(0);
    
}

void Bullets::update(){
    vec2 *pos = new vec2[bullets.size()];
    vec3 *col = new vec3[bullets.size()];
    
    for(unsigned int i=0; i < bullets.size(); i++){
        pos[i] = bullets[i]->state.cur_location;
        col[i] = bullets[i]->state.color;
    }

    
    glBindVertexArray( GLvars.vao );
    glBindBuffer( GL_ARRAY_BUFFER, GLvars.buffer );

    
    //Create GPU buffer to hold vertices and color
    glBufferData( GL_ARRAY_BUFFER, bullets.size()*sizeof(vec2) + bullets.size() *sizeof(vec3), NULL, GL_STATIC_DRAW );
    //First part of array holds vertices
    glBufferSubData( GL_ARRAY_BUFFER, 0, bullets.size()*sizeof(vec2), pos );
    //Second part of array hold colors (offset by sizeof(triangle))
    glBufferSubData( GL_ARRAY_BUFFER, bullets.size()*sizeof(vec2), bullets.size()*sizeof(vec3), col );
    
    glVertexAttribPointer( GLvars.vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    glVertexAttribPointer( GLvars.vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(bullets.size()*sizeof(vec2)) );
    
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    
    glBindVertexArray( 0);

    delete[] pos;
    delete[] col;
}

void Bullets::step(){
    for (std::vector< Bullet * >::iterator it = bullets.begin() ; it != bullets.end(); ++it){
        if((*it)->hit_target() == false){
            (*it)->update_state();
        }
        else{
            delete *it;
            bullets.erase(it);
            break;
        }
    }
}

void Bullets::add(vec2 loc, vec2 point, vec2 velocity){
    bullets.push_back(new Bullet(loc, width, height, point, velocity));
}

Bullet Bullets::return_first(){
    return *bullets[0];
}

void Bullets::delete_first(){
    for (std::vector< Bullet * >::iterator it = bullets.begin() ; it != bullets.end(); ++it){
        delete *it;
        bullets.erase(it);
        break;
    }
}

bool Bullets::active(){
    if(bullets.size() > 0){
        return true;
    }
    else{
        return false;
    }
}

void Bullets::draw(mat4 proj){
    
    glUseProgram( GLvars.program );
    glBindVertexArray( GLvars.vao );

    
    //!!!!!!!!Create a modelview matrix and pass it
    //mat4 matrix = Ortho2D(-1, 1, -1, 1);
    glUniformMatrix4fv( GLvars.M_location, 1, GL_TRUE, proj);
    
    glPointSize(4.0);
    glDrawArrays(GL_POINTS, 0, bullets.size());
    //!!!!!!!!Draw something
    glPointSize(1.0);
    glBindVertexArray(0);
    glUseProgram(0);
    
}