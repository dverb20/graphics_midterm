//
//  Bullet.hpp
//  ASTEROIDS
//
//  Created by Danny Verb on 10/6/16.
//
//

#ifndef Bullet_hpp
#define Bullet_hpp

#include "common.h"

class Bullet{
    
    float width, height;
    
public:
    
    Bullet(vec2 place,float w, float h, vec2 pointing, vec2 velocity);
    
    //Bullet State
    struct {
        vec2 cur_location;
        vec2 pointing;
        vec2 move;
        vec3 color;
        bool fired;
        bool hit;
    } state;
    
    void update_state();
    
    bool hit_target();
    
    void set_extents(int w, int h){
        width  = w/2;
        height = h/2;
    }
    
};



#endif /* Bullet_hpp */
