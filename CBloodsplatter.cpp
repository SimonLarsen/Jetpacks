#include "CBloodsplatter.hpp"

CBloodsplatter::CBloodsplatter(int _x, int _y){
	alive = true;
	lifetime = 100;
	x = _x; y = _y;
	for(int i = 0; i < 10; i++){
		drops[i].x = x;
		drops[i].y = y;
		drops[i].xspeed = (float)(rand() % 40 - 20)/10.f;
		drops[i].yspeed = (float)(rand() & 60 - 35)/10.f;	
	}
}

void CBloodsplatter::move(){
	if(!alive)
		return;

	for(int i = 0; i < 10; i++){
		drops[i].yspeed += PLAYERGRAVITY;
		drops[i].x += drops[i].xspeed;
		drops[i].y += drops[i].yspeed;
		if(drops[i].y+4 > WINHEIGHT){
			drops[i].y = WINHEIGHT-4;
			drops[i].xspeed = 0;
		}
		if(drops[i].x < 0)
			drops[i].x = 0;
		else if(drops[i].x+4 > WINWIDTH)
			drops[i].x = WINWIDTH-4;

	}
	--lifetime;
	if(lifetime <= 0)
		alive = false;
}

void CBloodsplatter::draw(SDL_Surface* screen){
	if(!alive)
		return;
	SDL_Rect brect = {0,0,4,4};
	Uint32 color = SDL_MapRGB(screen->format, 207, 0, 0);
	for(int i = 0; i < 10; i++){
		brect.x = drops[i].x;
		brect.y = drops[i].y;
		SDL_FillRect(screen, &brect, color);
	}
}
