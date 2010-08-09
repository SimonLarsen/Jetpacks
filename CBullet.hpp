#ifndef _CBULLET_HPP
#define _CBULLET_HPP

#include "SDL/SDL.h"
#include "defines.hpp"
#include "CSolid.hpp"

class CBullet : public CSolid{
public:
	CBullet(BULLETTYPE _type, int _x, int _y, DIRECTION _dir);
	CBullet(BULLETTYPE _type, int _x, int _y, DIRECTION _dir, SDL_Surface* _sprite);
	void draw(SDL_Surface* screen);
	void move();
	
	BULLETTYPE type;
	SDL_Rect dsrect;
	SDL_Surface* sprite;
	float xspeed, yspeed;
	DIRECTION dir;
};

#endif
