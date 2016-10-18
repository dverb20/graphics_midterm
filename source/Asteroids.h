//
//  Asteroids.h
//  ASTEROIDS
//
//  Created by Danny Verb on 10/2/16.
//
//

#ifndef Asteroids_h
#define Asteroids_h

#include "common.h"
#include "Asteroid.h"
#include "Bullet.hpp"

class Asteroids{
    
    float width, height;
    
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
    
    std::vector < Asteroid * > asteroids;
    
    Asteroids();
    ~Asteroids(){
        for(unsigned int i=0; i < asteroids.size(); i++)
            delete asteroids[i];
        asteroids.clear();
    }
    
    void update_state();
    
    void gl_init();
    
    void start();
    
    void step();
    
    bool ship_crash(vec2 ship_loc);
    
    bool bullet_hit(Bullet b);
    
    void split(vec2 loc, float size);
    
    void change();
    
    void draw(mat4 proj);
    
    void set_extents(int w, int h);
};


#endif /* Asteroids_h */
