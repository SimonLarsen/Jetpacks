#ifndef _CBLOODSPLATTER_HPP
#define _CBLOODSPLATTER_HPP

#include "SDL/SDL.h"
#include "defines.hpp"
#include "CParticle.hpp"

typedef struct {
	float x, y, xspeed, yspeed;
} drop;

class CBloodsplatter : public CParticle {
public:
	int x, y;
	CBloodsplatter(int _x, int _y);
	int lifetime;
	void move();
	void draw(SDL_Surface* screen);
	drop drops[10];
};

#endif
