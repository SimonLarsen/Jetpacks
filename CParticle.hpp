#ifndef _CPARTICLE_HPP
#define _CPARTICLE_HPP

#include "SDL/SDL.h"

class CParticle {
public:
	bool alive;
	virtual void move() = 0;
	virtual void draw(SDL_Surface* screen) = 0;
};

#endif
