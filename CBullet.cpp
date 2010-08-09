#include "CBullet.hpp"

CBullet::CBullet(BULLETTYPE _type, int _x, int _y, DIRECTION _dir){
	x = _x;
	y = _y;
	type = _type;	
	dir = _dir;
	sprite = NULL;
	yspeed = 0;

	if(type == BULLET){
		w = 8; h = 4; dsrect.w = 8; dsrect.h = 4; xspeed = 15; etype = EBULLET; 
	}
	else if(type == ROCKET){
		w = 16; h = 8; dsrect.w = 12; dsrect.h = 8; xspeed = 10; etype = EROCKET;
	}
	else if(type == FLAME){
		alive = false;
	}
	else if(type == SHELL){
		w = 8; h = 4; dsrect.w = 8; dsrect.h = 4; xspeed = 12; yspeed = (float)(-20 + rand() % 50)/10.f; etype = EBULLET; 
	}
}

CBullet::CBullet(BULLETTYPE _type, int _x, int _y, DIRECTION _dir, SDL_Surface* _sprite){
	x = _x;
	y = _y;
	type = _type;	
	dir = _dir;
	alive = true;
	sprite = _sprite;

	if(type == BULLET){
		w = 8; h = 4; dsrect.w = 8; dsrect.h = 4; xspeed = 12; yspeed = 0; etype = EBULLET; 
	}
	else if(type == ROCKET){
		w = 16; h = 8; dsrect.w = 12; dsrect.h = 8; xspeed = 18; yspeed = 0; etype = EROCKET;
	}
	else if(type == FLAME){
		w = 28; h = 28; dsrect.w = 28; dsrect.h = 28; xspeed = 10; yspeed = 0; etype = EFLAME;
	}
	else if(type == SHELL){
		w = 8; h = 4; dsrect.w = 8; dsrect.h = 4; xspeed = 12; yspeed = -3 + rand() % 3; etype = EBULLET; 
	}
}

void CBullet::draw(SDL_Surface* screen){
	dsrect.x = (int)x;
	dsrect.y = (int)y;
	
	switch(type){
		case BULLET: SDL_FillRect(screen, &dsrect, SDL_MapRGB(screen->format, 36, 36, 36)); break;
		case ROCKET: SDL_FillRect(screen, &dsrect, SDL_MapRGB(screen->format, 36, 73, 0)); break;
		case SHELL: SDL_FillRect(screen, &dsrect, SDL_MapRGB(screen->format, 36, 36, 36)); break;
		case FLAME: SDL_Rect srcrect = {0,72,28,28}; SDL_BlitSurface(sprite,&srcrect,screen,&dsrect); break;
	}
}

void CBullet::move(){
	switch(dir){
		case LEFT: x -= xspeed; break;
		case RIGHT: x += xspeed; break;
		case DNONE: x += 0; break;
	}
	y += yspeed;
	if(type == FLAME){
		y += rand() % 6 - 2;
		xspeed *= 0.95;
		if(xspeed < 1)
			alive = false;
	}

	if(x+w < 0)
		alive = false;
	else if(x > WINWIDTH)
		alive = false;
}
