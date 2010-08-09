#include "CPickup.hpp"

CPickup::CPickup(int _x, int _y, WEAPONTYPE wtype, SDL_Surface* sprites){
	x = _x;
	y = _y;
	w = CRATEWIDTH;
	h = CRATEHEIGHT;
	etype = EPICKUP;	
	weapon = wtype;
	sprite = sprites;	

	srcrect.x = (weapon-1)*CRATEWIDTH;
	srcrect.y = 0;
	srcrect.w = CRATEWIDTH;
	srcrect.h = CRATEWIDTH;
}

void CPickup::draw(SDL_Surface* screen){
	SDL_Rect dsrect = {(int)x, (int)y, 0, 0};
	SDL_BlitSurface(sprite, &srcrect, screen, &dsrect);
}

void CPickup::move(){
	y += PICKUPFALLSPEED;
	if(y > WINHEIGHT)
		alive = false;
}
