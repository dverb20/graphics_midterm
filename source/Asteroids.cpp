//
//  Asteroids.cpp
//  ASTEROIDS
//
//  Created by Danny Verb on 10/2/16.
//
//

#include <stdio.h>
#include "common.h"
#include "math.h"

//Asteroid a;
//Asteroid b;
//Asteroid c;

Asteroids::Asteroids(){
    for(int i=0; i<4;i++){
        asteroids.push_back(new Asteroid(2,vec2(0,0)));
    }
}

void Asteroids::gl_init(){
    //Asteroids
    //!!!!!!!!Asteroids
    for (std::vector< Asteroid * >::iterator it = asteroids.begin() ; it != asteroids.end(); ++it){
        (*it)->gl_init();
    }
    
    
}

//create game start function

void Asteroids::start(){
//    a.change_loc(vec2(200,-30));
//    b.change_loc(vec2(-200,-30));
//    c.change_loc(vec2(100,-30));
    
    for (std::vector< Asteroid * >::iterator it = asteroids.begin() ; it != asteroids.end(); ++it){
        float sX = rand()%2+1;
        float sY = rand()%2+1;
        if(sX == 2){sX = -1;}
        if(sY == 2){sY = -1;}
        float rX = (rand()%250+50)*sX;
        float rY = (rand()%250+50)*sY;
        (*it)->change_loc(vec2(rX,rY));
    }
    
}

void Asteroids::update_state(){
    //Updates the state of each asteroid in array
    
//    for (std::vector< Asteroid * >::iterator it = asteroids.begin() ; it != asteroids.end(); ++it){
//        if((*it)->hit() == true){
//            if((*it)->getSize() == 2){
//                std::cout << (*it) << std::endl;
//                vec2 loc = (*it)->location();
//                delete (*it);
//                asteroids.erase(it);
//                //it;
//                split(loc);
//                std::cout << "split" << std::endl;
//            }
////            else if((*it)->getSize() == 1){
////                Asteroid *uno = new Asteroid(0.5);
////                Asteroid *dos = new Asteroid(0.5);
////                uno->change_loc(vec2((*it)->location()));
////                dos->change_loc(vec2((*it)->location()));
////                asteroids.push_back(uno);
////                asteroids.push_back(dos);
////                //delete[] *it;
////                //asteroids.erase(it);
////                //--it;
////            }
//        }
//        else{
//            (*it)->step();
//        }
//        
//    }
    std::vector< Asteroid * >::iterator it = asteroids.begin();
    int i = 0;
    while(i < asteroids.size()){
        (*it)->step();
        ++it;
        i++;
        
    }
};

//create iterator
//make while loop going from it to end of array
//check space -> delete it and erase it
//if it isn't erased move arry forward '

void Asteroids::change(){
    for (std::vector< Asteroid * >::iterator it = asteroids.begin() ; it != asteroids.end(); ++it){
        (*it)->was_hit();
        vec2 loc = (*it)->location();
        float size = (*it)->getSize();
        delete (*it);
        asteroids.erase(it);
        if(size >= 1){
            split(loc,(size/2));
        }
        break;
    }
}

void Asteroids::split(vec2 loc, float size){
    Asteroid *uno = new Asteroid(size*1.2,loc);
    Asteroid *dos = new Asteroid(size*1.2,loc);
    uno->gl_init();
    dos->gl_init();
    asteroids.push_back(uno);
    asteroids.push_back(dos);
}

bool Asteroids::ship_crash(vec2 ship_crash){
    for (std::vector< Asteroid * >::iterator it = asteroids.begin() ; it != asteroids.end(); ++it){
        vec2 loc = (*it)->location();
        vec2 difference = loc - ship_crash;
        float size = (*it)->getSize();
        float edge = (225*size*size)+100;
        if(difference.x*difference.x < edge && difference.y*difference.y < edge ){
            return true;
        }
    }
    return false;
}

bool Asteroids::bullet_hit(Bullet b){
    vec2 bul = b.state.cur_location;
    for (std::vector< Asteroid * >::iterator it = asteroids.begin() ; it != asteroids.end(); ++it){
        vec2 loc = (*it)->location();
        vec2 difference = loc - bul;
        float size = (*it)->getSize();
        float edge = (225*size*size)+100;
        if(difference.x*difference.x < edge && difference.y*difference.y < edge ){
            std::cout << (difference.x*difference.x) << std::endl;
            std::cout << (difference.y*difference.y) << std::endl;
            delete (*it);
            asteroids.erase(it);
            if(size >= 1){
                split(loc,(size/2));
            }
            return true;
        }
    }
    return false;
}

void Asteroids::draw(mat4 proj){
    for (std::vector< Asteroid * >::iterator it = asteroids.begin() ; it != asteroids.end(); ++it){
        (*it)->draw(proj);
    }
}

void Asteroids::set_extents(int w, int h){
    for (std::vector< Asteroid * >::iterator it = asteroids.begin() ; it != asteroids.end(); ++it){
        (*it)->set_extents(w, h);
    }
}



