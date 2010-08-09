#include "CApp.hpp"

CApp::CApp(){
	running = false;
	p1score = 0;
	p2score = 0;
	updates = 0;
}

bool CApp::init(){
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1)
		return false;
	std::cout << "SDL initialized" << std::endl;

	screen = SDL_SetVideoMode(WINWIDTH,WINHEIGHT,BPP,SDL_SWSURFACE);
	if(screen == NULL)
		return false;
	std::cout << WINWIDTH << "x" << WINHEIGHT << " window set" << std::endl;

	if(TTF_Init() == -1)
		return false;
	std::cout << "SDL_TTF initialized" << std::endl;

	#ifdef MIXER
	int mixflags = MIX_INIT_MP3;
	int mixreturn = Mix_Init(mixflags);
	if(mixreturn&mixflags != mixflags)
		return false;
	if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 1, 4096) == -1)
		return false;
	std::cout << "SDL_Mixer initialized" << std::endl;
	#endif

	SDL_WM_SetCaption("Jetpacks", NULL);

	srand( SDL_GetTicks() );

	return true;
}

SDL_Surface* CApp::loadImage(std::string filename){
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL;
	
	loadedImage = SDL_LoadBMP(filename.c_str());
	if(loadedImage != NULL){
		optimizedImage = SDL_DisplayFormat(loadedImage);
		SDL_FreeSurface(loadedImage);
	}
	return optimizedImage;
}

void CApp::writeText(std::string text, int x, int y){
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), twhite);
	SDL_Rect dsrect = {x,y,0,0};
	SDL_BlitSurface(surface, NULL, screen, &dsrect);
	SDL_FreeSurface(surface);
}

void CApp::writeTextOffsetRight(std::string text, int x, int y){
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), twhite);
	SDL_Rect dsrect = {WINWIDTH-x-surface->w,y,0,0};
	SDL_BlitSurface(surface, NULL, screen, &dsrect);
	SDL_FreeSurface(surface);
}

bool CApp::loadResources(){
	std::cout << "Loading resources..." << std::endl;

	sprPlayers = loadImage("gfx/players.bmp");
	if(sprPlayers == NULL)
		return false;
	SDL_SetColorKey(sprPlayers, SDL_SRCCOLORKEY, SDL_MapRGB(sprPlayers->format, 0xFF, 0, 0xFF));

	sprWeapons = loadImage("gfx/weapons.bmp");
	if(sprWeapons == NULL)
		return false;
	SDL_SetColorKey(sprWeapons, SDL_SRCCOLORKEY, SDL_MapRGB(sprWeapons->format, 0xFF, 0, 0xFF));

	sprPickups = loadImage("gfx/pickups.bmp");
	if(sprPickups == NULL)
		return false;
	SDL_SetColorKey(sprPickups, SDL_SRCCOLORKEY, SDL_MapRGB(sprPickups->format, 0xFF, 0, 0xFF));

	sprMountains = loadImage("gfx/mountains.bmp");
	if(sprMountains == NULL)
		return false;
	SDL_SetColorKey(sprMountains, SDL_SRCCOLORKEY, SDL_MapRGB(sprMountains->format, 0xFF, 0, 0xFF));
	mtrect.x = (WINWIDTH-sprMountains->w)/2;
	mtrect.y = WINHEIGHT-sprMountains->h;

	font = TTF_OpenFont("04B21.ttf", 38);
	if(font == NULL)
		return false;

	#ifdef MUSIC
	music = Mix_LoadMUS("eighteen.mp3");
	if(music == NULL)
		return false;
	#endif

	#ifdef SOUNDS
	sfxGun = Mix_LoadWAV("./sfx/gun2.wav");
	if(sfxGun == NULL)
		return false;
	#endif

	bgcolor = SDL_MapRGB(screen->format, 73, 135, 200);
	twhite.r = 0xFF; twhite.g = 0xFF; twhite.b = 0xFF;

	std::cout << "Resources loaded." << std::endl;	
	return true;
}

void CApp::cleanUp(){
	std::cout << "Cleaning up..." << std::endl;
	SDL_FreeSurface(sprPlayers);
	SDL_FreeSurface(sprWeapons);
	SDL_FreeSurface(sprPickups);
	SDL_FreeSurface(screen);
	TTF_CloseFont(font);
	TTF_Quit();
	Mix_CloseAudio();
	#ifdef MUSIC
	Mix_FreeMusic(music);
	#endif
	#ifdef SOUNDS
	Mix_FreeChunk(sfxGun);
	#endif
	Mix_Quit();
	
	delete p1;
	delete p2;

	for(int i = 0; i < particles.size(); i++){
		delete particles.at(i);
		particles.erase(particles.begin()+i);
	}
	particles.clear();
	for(int i = 0; i < pickups.size(); i++){
		delete pickups.at(i);
		pickups.erase(pickups.begin()+i);
	}
	pickups.clear();
}

void CApp::loop(){
	#ifdef MUSIC
	Mix_PlayMusic(music,-1);
	Mix_VolumeMusic(MIX_MAX_VOLUME/2);
	#endif
	running = true;
	p1 = new CPlayer( RED, 200, 200, sprPlayers, sprWeapons);
	p2 = new CPlayer( BLUE, WINWIDTH-PLAYERWIDTH-200, 200, sprPlayers, sprWeapons);
	SDLKey p1keys[] = { SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_RCTRL };
	SDLKey p2keys[] = { SDLK_w, SDLK_s, SDLK_a, SDLK_d, SDLK_LSHIFT };
	p1->setKeys(p1keys);
	p2->setKeys(p2keys);
	particles.reserve(10);
	pickups.reserve(5);
	nextPickup = updates + rand() % PICKUPSPAWNWAIT;

	while(running){	
		frameStart = SDL_GetTicks();
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				running = false;
			}
			else{
				p1->event(&event);
				p2->event(&event);
			}
		}

		// Spawn pickups
		if(nextPickup == updates){
			pickups.push_back(new CPickup(rand() % WINWIDTH-CRATEWIDTH, 0, (WEAPONTYPE)(rand() % 5 + 1), sprPickups));
			nextPickup = updates + rand() % PICKUPSPAWNWAIT;
		}
		
		// Move players
		p1->move();
		p2->move();

		// Move particles and clean up
		for(int i = 0; i < particles.size(); i++){
			if(particles.at(i)->alive)
				particles.at(i)->move();			
			else{
				delete particles.at(i);
				particles.erase(particles.begin()+i);
			}
		}

		// Move pickups and clean up
		for(int i = 0; i < pickups.size(); i++){
			if(pickups.at(i)->alive)
				pickups.at(i)->move();
			else{
				delete pickups.at(i);
				pickups.erase(pickups.begin()+i);
			}
		}

		// Check player 1 collisions
		if(p1->alive){
			if(p1->invulTime <= 0){
				for(int i = 0; i < p2->bullets.size(); ++i){
					if(CSolid::collide(p1,p2->bullets.at(i))){
						p1->handleCollision(p2->bullets.at(i)->etype);
						if(p2->bullets.at(i)->etype != EFLAME){
							new CBloodsplatter(p2->bullets.at(i)->x, p2->bullets.at(i)->y);
							particles.push_back(new CBloodsplatter(p2->bullets.at(i)->x, p2->bullets.at(i)->y));
							particles.push_back(new CBloodsplatter(p2->bullets.at(i)->x, p2->bullets.at(i)->y));
							if(p2->bullets.at(i)->etype == EROCKET){
								p2->createExplosion(p2->bullets.at(i)->x, p2->bullets.at(i)->y);
							}
							delete p2->bullets.at(i);
							p2->bullets.erase(p2->bullets.begin()+i);
						}
					}
				}
				if(p2->weapon == CHAINSAW){
					if(CSolid::collide( p1, p2->getChainsaw() ) ){
						p1->handleCollision(ECHAINSAW);
						CBloodsplatter* splat = new CBloodsplatter(p1->x+16, p1->y+24);
						particles.push_back(splat);
					}
				}
			}
			for(int i = 0; i < pickups.size(); ++i){
				if(CSolid::collide(p1, pickups.at(i))){
					p1->pickupCollision(pickups.at(i)->weapon);
					pickups.at(i)->alive = false;
				}
			}
		}
		// Check player 2 collisions
		if(p2->alive){
			if(p2->invulTime <= 0){
				for(int i = 0; i < p1->bullets.size(); ++i){
					if(CSolid::collide(p2,p1->bullets.at(i))){
						p2->handleCollision(p1->bullets.at(i)->etype);
						if(p1->bullets.at(i)->etype != EFLAME){
							particles.push_back(new CBloodsplatter(p1->bullets.at(i)->x, p1->bullets.at(i)->y));
							particles.push_back(new CBloodsplatter(p1->bullets.at(i)->x, p1->bullets.at(i)->y));
							if(p1->bullets.at(i)->etype == EROCKET){
								p1->createExplosion(p1->bullets.at(i)->x, p1->bullets.at(i)->y);
							}
							delete p1->bullets.at(i);
							p1->bullets.erase(p1->bullets.begin()+i);
						}
					}
				}
				if(p1->weapon == CHAINSAW){
					if(CSolid::collide(p2, p1->getChainsaw() ) ){
						p2->handleCollision(ECHAINSAW);	
						CBloodsplatter* splat = new CBloodsplatter(p2->x+16, p2->y+24);
						particles.push_back(splat);
					}
				}
			}
			for(int i = 0; i < pickups.size(); ++i){
				if(CSolid::collide(p2, pickups.at(i))){
					p2->pickupCollision(pickups.at(i)->weapon);
					pickups.at(i)->alive = false;
				}
			}
		}

		#ifdef BLEEDMODE		
		if(p1->health < PLAYERBLEEDPOINT && p1->alive)
			particles.push_back(new CBloodsplatter(p1->x+16, p1->y+9));
		if(p2->health < PLAYERBLEEDPOINT && p2->alive)
			particles.push_back(new CBloodsplatter(p2->x+16, p2->y+9));
		#endif

		#ifdef SOUNDS
		// Play sound effects
		if(p1->lastShot == p1->updates)
			Mix_PlayChannel(-1, sfxGun, 0);
		if(p2->lastShot == p2->updates)
			Mix_PlayChannel(-1, sfxGun, 0);
		#endif

		// Fill screen and draw mountains
		SDL_FillRect(screen, NULL, bgcolor);
		SDL_BlitSurface(sprMountains, NULL, screen, &mtrect);

		// Draw pickups
		for(int i = 0; i < pickups.size(); i++){
			pickups.at(i)->draw(screen);
		}

		// Draw players
		p1->draw(screen);
		p2->draw(screen);

		/// Draw particles
		for(int i = 0; i < particles.size(); i++){
			particles.at(i)->draw(screen);
		}

		// Write ammo
		char p1ammo[16];
		char p2ammo[16];
		sprintf(p1ammo, "AMMO: %d", p1->ammo);
		sprintf(p2ammo, "AMMO: %d", p2->ammo);
		writeText(p1ammo,10,30);
		writeTextOffsetRight(p2ammo,10,30);
		// Draw healthbars
		SDL_Rect p1underbar = {10,10,200,10};
		SDL_Rect p1bar = {10,10,2*p1->health,10};
		Uint32 p1barcolor = SDL_MapRGB(screen->format, (float)(100-p1->health)*2.55, (float)p1->health*2.55, 0);

		SDL_Rect p2underbar = {WINWIDTH-10-200, 10, 200, 10};
		SDL_Rect p2bar = {WINWIDTH-10-2*p2->health, 10, 2*p2->health, 10};
		Uint32 p2barcolor = SDL_MapRGB(screen->format, (float)(100-p2->health)*2.55, (float)p2->health*2.55, 0);

		SDL_FillRect(screen, &p1underbar, 0x000000);
		SDL_FillRect(screen, &p1bar, p1barcolor);
		SDL_FillRect(screen, &p2underbar, 0x000000);	
		SDL_FillRect(screen, &p2bar, p2barcolor);	

		// Draw deathcount
		char p1deaths[10];
		char p2deaths[10];
		sprintf(p1deaths, "%d KILLS", p2->deaths);
		sprintf(p2deaths, "%d KILLS", p1->deaths);
		writeText(p1deaths, 10, WINHEIGHT-40);
		writeTextOffsetRight(p2deaths, 10, WINHEIGHT-40);
		// Update and wait
		SDL_UpdateRect(screen, 0, 0, 0, 0);

		frameTime = SDL_GetTicks() - frameStart;
		if(frameTime < 15){
			SDL_Delay(15 - frameTime);
		}
		++updates;
	}
}

int CApp::execute(){
	if(!init())
		return 1;

	if(!loadResources())
		return 1;

	loop();

	cleanUp();

	SDL_Quit();

	return 0;
}

int main(int argc, char** argv){
	CApp theApp;
	return theApp.execute();
}
