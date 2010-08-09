#include "CSolid.hpp"

CSolid::CSolid(){
	x = 0;
	y = 0;
	w = 0;
	h = 0;
	etype = ENONE;
	alive = true;
}

CSolid::CSolid(float _x, float _y, int _w, int _h){
	x = _x;
	y = _y;
	w = _w;
	h = _h;
	alive = true;
}

bool CSolid::collide(CSolid* obj1, CSolid* obj2){
	if(obj1->alive && obj2->alive){
		if(obj1->x > obj2->x+obj2->w)
			return false;
	
		if(obj1->x+obj1->w < obj2->x)
			return false;
	
		if(obj1->y > obj2->y+obj2->h)
			return false;
	
		if(obj1->y+obj1->h < obj2->y)
			return false;

		return true;
	}
	else
		return false;
}

bool CSolid::collide(CSolid* obj1, CSolid obj2){
	if(obj1->alive && obj2.alive){
		if(obj1->x > obj2.x+obj2.w)
			return false;
	
		if(obj1->x+obj1->w < obj2.x)
			return false;

		if(obj1->y > obj2.y+obj2.h)
			return false;

		if(obj1->y+obj1->h < obj2.y)
			return false;

		return true;
	}
	else
		return false;
}
