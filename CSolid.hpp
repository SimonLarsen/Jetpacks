#ifndef _CSOLID_HPP
#define _CSOLID_HPP

#include <iostream>
#include "defines.hpp"

class CSolid{
public:
	CSolid();
	CSolid(float _x, float _y, int _w, int _h);
	float x, y;
	int w, h;
	static bool collide(CSolid* obj1, CSolid* obj2);
	static bool collide(CSolid* obj1, CSolid obj2);
	bool alive;
	ENTITY etype;
};

#endif
