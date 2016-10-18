//
//  Bullets.hpp
//  ASTEROIDS
//
//  Created by Danny Verb on 10/6/16.
//
//

#ifndef Bullets_hpp
#define Bullets_hpp


#include "common.h"
#include "Bullet.hpp"

class Bullets{
    
    
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
    
    std::vector < Bullet * > bullets;
    
    float width, height;
    
    Bullets(){};
    ~Bullets(){
        for(unsigned int i=0; i < bullets.size(); i++)
            delete bullets[i];
        bullets.clear();
    }
    
    bool active();
    
    void gl_init();
    
    void add(vec2 loc, vec2 point, vec2 velocity);
    
    void update();
    
    void step();
    
    Bullet return_first();
    
    void delete_first();
    
    void draw(mat4 proj);
    
    void set_extents(int w, int h){
        width  = w/2;
        height = h/2;
    }
};

#endif /* Bullets_hpp */
