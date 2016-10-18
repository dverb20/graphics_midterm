//
//  Bullet.cpp
//  ASTEROIDS
//
//  Created by Danny Verb on 10/6/16.
//
//

#include "common.h"

Bullet::Bullet(vec2 place,float w,float h, vec2 pointing, vec2 velocity){
    state.cur_location = place+pointing*27;
    state.pointing = vec2(1.0,0.0);
    state.color = vec3(1,1,1);
    state.move = pointing+velocity*.3;
    state.fired = false;
    state.hit = false;
    width = w;
    height = h;
    
}

void Bullet::update_state(){
    state.cur_location += state.move*10;
    //!!!!!!!!Dampen the velocity at every timestep to lessen intertia
    //!!!!!!!!Move the ship location
    
    //Wrap the ship position when at the boundary
    //!!!!!!!!This will change depending on the world coordinates you are using
    if(state.cur_location.x < -width ||state.cur_location.x > width){
        state.hit = true; //delete the bullet here
    }
    if(state.cur_location.y < -height ||state.cur_location.y > height){
        state.hit = true; //delete the bullet here
    }
    
}

bool Bullet::hit_target(){
    if (state.hit == true){
        return true;
    }
    else{return false;}
}

