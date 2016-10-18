//
//  Asteroid.h
//  ASTEROIDS
//
//  Created by Danny Verb on 10/2/16.
//
//

#ifndef Asteroid_h
#define Asteroid_h

#include "common.h"
#include <stdio.h>

class Asteroid{
    
    float width, height;
    vec2 asteroid_vert[4];
    vec3 asteroid_color[4];
    
    //Asteroid State
     struct {
        vec2 cur_location;
        vec2 pointing;
        vec2 move;
        bool struck;
        float angle;
        float size;
    } state;
    
    //OpenGL vars
    struct {
        GLuint vao;
        GLuint program;
        GLuint buffer;
        GLuint vertex_shader, fragment_shader;
        GLint vpos_location, vcolor_location;
        GLint M_location;
    } GLvars;
    
public:
    
    Asteroid(float size, vec2 loc);
    
    void gl_init();
    
    void change_loc(vec2 change);
    
    bool hit();
    
    vec2 location();
    
    float getSize();
    
    void was_hit();
    
    void step();

    void draw(mat4 proj);
    
    void set_extents(int w, int h){
        width  = w/2;
        height = h/2;
    }
};



#endif /* Asteroid_h */
