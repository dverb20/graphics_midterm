//
//  Asteroid.cpp
//  ASTEROIDS
//
//  Created by Danny Verb on 10/2/16.
//
//

#include "common.h"
#include "math.h"

Asteroid::Asteroid(float size, vec2 loc){
    float sX = rand()%2;if(sX == 0){sX = -1;}
    float sY = rand()%2;if(sY == 0){sY = -1;}
    state.cur_location = loc;
    state.pointing = vec2(0,0);
    state.move = vec2(10*sX,10*sY);
    state.struck = false;
    state.angle = 0;
    state.size = size;
}

bool Asteroid::hit(){
    if(state.struck == true){
        return true;
    }
    else{
        return false;
    }
}

float Asteroid::getSize(){
    return state.size;
}

vec2 Asteroid::location(){
    return state.cur_location;
}

void Asteroid::was_hit(){
    state.struck = true;
}
void Asteroid::gl_init(){
    //Asteroid
    //!!!!!!!!Asteroid
    //float oX = state.cur_location.x;
    //float oY = state.cur_location.y;
    float factor = state.size;
    vec2 asteroid_vert[8] = {
//        vec2(-15*factor,7.5*factor),vec2(-7.5*factor,15*factor),vec2(7.5*factor,15*factor),vec2(15*factor,7.5*factor),vec2(15*factor,-7.5*factor),vec2(7.5*factor,-15*factor),vec2(-7.5*factor,-15*factor),vec2(-15*factor,-7.5*factor)
        vec2(-15*factor,7.5*factor),vec2(15*factor,7.5*factor),vec2(-7.5*factor,-15*factor),vec2(-7.5*factor,15*factor),vec2(15*factor,-7.5*factor),vec2(-15*factor,-7.5*factor),vec2(7.5*factor,15*factor),vec2(7.5*factor,-15*factor)
    };
    vec3 asteroid_color[8] = {
        vec3(rand()/(float)RAND_MAX, rand()/(float)RAND_MAX, rand()/(float)RAND_MAX),
        vec3(rand()/(float)RAND_MAX, rand()/(float)RAND_MAX, rand()/(float)RAND_MAX),
        vec3(rand()/(float)RAND_MAX, rand()/(float)RAND_MAX, rand()/(float)RAND_MAX),
        vec3(rand()/(float)RAND_MAX, rand()/(float)RAND_MAX, rand()/(float)RAND_MAX),
        vec3(rand()/(float)RAND_MAX, rand()/(float)RAND_MAX, rand()/(float)RAND_MAX),
        vec3(rand()/(float)RAND_MAX, rand()/(float)RAND_MAX, rand()/(float)RAND_MAX),
        vec3(rand()/(float)RAND_MAX, rand()/(float)RAND_MAX, rand()/(float)RAND_MAX),
        vec3(rand()/(float)RAND_MAX, rand()/(float)RAND_MAX, rand()/(float)RAND_MAX)
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
    glBufferData( GL_ARRAY_BUFFER, sizeof(asteroid_vert) + sizeof(asteroid_color), NULL, GL_STATIC_DRAW );
    //First part of array holds vertices
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(asteroid_vert), asteroid_vert );
    //Second part of array hold colors (offset by sizeof(triangle))
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(asteroid_vert), sizeof(asteroid_color), asteroid_color );
    
    glEnableVertexAttribArray(GLvars.vpos_location);
    glEnableVertexAttribArray(GLvars.vcolor_location );
    
    glVertexAttribPointer( GLvars.vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    glVertexAttribPointer( GLvars.vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(asteroid_vert)) );
    
    glBindVertexArray(0);
    
}

void Asteroid::step(){//randomize initial velocity, bouce off of walls at inverse but less than velocity
    //Code changes here
    state.cur_location += state.move*.2;
    state.angle += 15*(1.0/6.0);
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

void Asteroid::change_loc(vec2 change){
    state.cur_location = change;
}

void Asteroid::draw(mat4 proj){
    
    //    float sX = rand()%2+1;
    //    float sY = rand()%2+1;
    //    if(sX == 2){sX = -1;}
    //    if(sY == 2){sY = -1;}
    //    float rX = (rand()%250+50)*sX;
    //    float rY = (rand()%250+50)*sY;
    
    glUseProgram( GLvars.program );
    glBindVertexArray( GLvars.vao );
    
    //!!!!!!!!Create a modelview matrix and pass it
    mat4 matrix = Ortho2D(-1, 1, -1, 1);
    mat4 movement= Translate(GLfloat(state.cur_location.x), GLfloat(state.cur_location.y), GLfloat(0))*Angel::RotateZ(GLfloat(state.angle));
    glUniformMatrix4fv( GLvars.M_location, 1, GL_TRUE, proj*matrix*movement);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);
    //!!!!!!!!Draw something
    glBindVertexArray(0);
    glUseProgram(0);
    
}