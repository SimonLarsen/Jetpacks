#ifndef _CAPP_CPP
#define _CAPP_CPP

#include <iostream>
#include <string>
#include <vector>
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "defines.hpp"
#include "CPlayer.hpp"
#include "CParticle.hpp"
#include "CBloodsplatter.hpp"
#include "CPickup.hpp"

class CApp{
private:
	bool init();
	SDL_Surface* loadImage(std::string filename);
	bool loadResources();
	void writeText(std::string text, int x, int y);
	void writeTextOffsetRight(std::string text, int x, int y);
	void loop();
	void cleanUp();

	CPlayer* p1;
	CPlayer* p2;
	Uint32 bgcolor;
	Uint32 frameStart, frameTime;
	int updates, nextPickup;
	SDL_Color twhite;
	bool running;

public:
	CApp();
	int execute();
	SDL_Surface* screen;
	SDL_Surface* sprPlayers;
	SDL_Surface* sprWeapons;
	SDL_Surface* sprPickups;
	SDL_Surface* sprMountains;
	SDL_Rect mtrect;
	Mix_Music* music;
	Mix_Chunk* sfxGun;
	TTF_Font* font;
	SDL_Event event;
	std::vector<CParticle*> particles;
	std::vector<CPickup*> pickups;
	int p1score, p2score;
};

#endif
