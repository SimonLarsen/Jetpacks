#ifndef _CPICKUP_HPP
#define _CPICKUP_HPP

#include "SDL/SDL.h"
#include "defines.hpp"
#include "CSolid.hpp"

class CPickup : public CSolid{
public:
	CPickup(int x, int y, WEAPONTYPE wtype, SDL_Surface* sprites);
	void move();
	void draw(SDL_Surface* screen);

	SDL_Rect srcrect;
	SDL_Surface* sprite;
	WEAPONTYPE weapon;
};

#endif
