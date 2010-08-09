#ifndef _CPLAYER_HPP
#define _CPLAYER_HPP

#include <vector>
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "defines.hpp"
#include "CSolid.hpp"
#include "CBullet.hpp"

class CPlayer : public CSolid{
public:
	float xspeed, yspeed;
	CPlayer(PLAYERS _ID, int _x, int _y, SDL_Surface* _sprite, SDL_Surface* _wpsprite);
	~CPlayer();
	void setKeys(SDLKey* keyset);
	void event(SDL_Event* event);
	void draw(SDL_Surface* screen);
	void move();
	void handleCollision(ENTITY entity);
	void pickupCollision(WEAPONTYPE wp);
	void shoot();
	void createExplosion(int _x, int _y);
	CSolid getChainsaw();

	std::vector<CBullet*> bullets;	
	WEAPONTYPE weapon;
	DIRECTION dir;
	int ammo;
	int deaths;
	int invulTime;
	bool blink;
	float health;
	PLAYERS ID;
	Uint32 lastShot, updates;

private:
	SDL_Surface* sprite;
	SDL_Surface* wpsprites;
	SDL_Rect dsrect, srcrect, wpdsrect, wpsrcrect;
	SDLKey keys[5];
	bool keystates[5];
};

#endif
