#include "CPlayer.hpp"

CPlayer::CPlayer(PLAYERS _ID, int _x, int _y, SDL_Surface* _sprite, SDL_Surface* _wpsprite){
	ID = _ID;
	x = _x;
	y = _y;
	xspeed = 0;
	yspeed = 0;
	w = PLAYERWIDTH;
	h = PLAYERHEIGHT;
	weapon = SMG;
	ammo = 20;
	health = 100;
	etype = EPLAYER;
	srcrect.w = PLAYERWIDTH;
	srcrect.h = PLAYERHEIGHT;
	bullets.reserve(10);
	lastShot = 0;
	updates = 0;
	deaths = 0;
	invulTime = INVULNERABILITYTIME;
	blink = true;

	wpsrcrect.w = 56;
	wpsrcrect.h = 36;
	wpsrcrect.x = 0;
	wpsrcrect.y = 0;
	sprite = _sprite;
	wpsprites = _wpsprite;
	for(int i = 0; i < 5; i++)
		keystates[i] = false;
	dir = RIGHT;
}

void CPlayer::setKeys(SDLKey* keyset){
	for(int i = 0; i < 5; i++){
		keys[i] = keyset[i];
	}
}

void CPlayer::event(SDL_Event* event){
	if(event->type == SDL_KEYDOWN){
		for(int i = 0; i < 5; i++){
			if(event->key.keysym.sym == keys[i]){
				keystates[i] = true; break;
			}
		}
	}
	else if(event->type == SDL_KEYUP){
		for(int i = 0; i < 5; i++){
			if(event->key.keysym.sym == keys[i]){
				keystates[i] = false; break;
			}
		}
	}
}

void CPlayer::draw(SDL_Surface* screen){
	if(!alive)
		return;

	// Draw bullets
	for(int i = 0; i < bullets.size(); ++i){
		if(bullets.at(i)->alive){
			bullets.at(i)->draw(screen);
		}
	}

	if(invulTime > 0 && blink == false)
		return;

	// Draw player
	dsrect.x = (int)x;
	dsrect.y = (int)y;
	if(dir == LEFT)
		srcrect.x = 0;
	else
		srcrect.x = PLAYERWIDTH;
	srcrect.y = ID*PLAYERHEIGHT;
	SDL_BlitSurface(sprite, &srcrect, screen, &dsrect);

	// Draw flames if upkey is down
	if(keystates[KUP]){
		SDL_Rect flsrc = {0,96,12,12};
		SDL_Rect flds;
		if(dir == LEFT){
			flds.x = x+24;
			flds.y = y+32;
		}
		else{
			flsrc.x = 16;
			flds.x = x-4;
			flds.y = y+32;
		}
		SDL_BlitSurface(sprite,&flsrc,screen,&flds);
	}

	// Draw weapon if any
	if(weapon > 0){
		wpsrcrect.x = (weapon-1)*56;
		if(dir == LEFT){
			wpsrcrect.y = 0;
			wpdsrect.x = x-24;
		}
		else{
			wpsrcrect.y = 36;
			wpdsrect.x = x;
		}
		wpdsrect.y = y;
		SDL_BlitSurface(wpsprites, &wpsrcrect, screen, &wpdsrect);
	}
}

void CPlayer::move(){
	updates++;

	if(!alive)
		return;

	// Accelerate up
	if(keystates[KUP])
		yspeed -= PLAYERJETPACKPOWER;

	// Move left or right
	if(keystates[KLEFT]){
		xspeed -= PLAYERSPEEDINCREASE;
		dir = LEFT;
	}
	if(keystates[KRIGHT]){
		xspeed += PLAYERSPEEDINCREASE;
		dir = RIGHT;
	}
	// Shoot button
	if(keystates[KSHOOT])
		shoot();

	// Add gravity
	yspeed += PLAYERGRAVITY;

	// Cap horizontal and vertical speed
	if(yspeed > MAXSPEED)
		yspeed = MAXSPEED;
	else if(yspeed < -MAXSPEED)
		yspeed = -MAXSPEED;

	// Add speeds to x- and y-coords
	x += xspeed;
	y += yspeed;

	// Check collision with window borders
	if(x < 0){
		x = 0;
		xspeed = WALLBOUNCEABSORB*-xspeed;
	}
	else if(x+w > WINWIDTH){
		x = WINWIDTH-PLAYERWIDTH;
		xspeed = WALLBOUNCEABSORB*-xspeed;
	}
	if(y < 0){
		y = 0;
		yspeed = WALLBOUNCEABSORB*-yspeed;
	}
	else if(y+h > WINHEIGHT){
		y = WINHEIGHT-h;
		yspeed = WALLBOUNCEABSORB*-yspeed;
	}

	// If invulnerable: Decrease time and checke blinkframe
	if(invulTime > 0){
		--invulTime;
		blink = !blink;
	}
	
	// Move bullets and delete dead bullets
	for(int i = 0; i < bullets.size(); ++i){
		if(bullets.at(i)->alive){
			bullets.at(i)->move();
		}
		else{
			delete bullets.at(i);
			bullets.erase(bullets.begin()+i);
		}
	}
}

void CPlayer::shoot(){
// Check if player has a weapon and any ammo
	if(weapon > 0 && ammo > 0){
		if(weapon == SMG){
			if(updates - lastShot > SMGWAIT){
				int bx;
				if(dir == LEFT)
					bx = x;
				else
					bx = x+w;
				bullets.push_back(new CBullet(BULLET, bx, y+24, dir));
				--ammo;
				lastShot = updates;
			}
		}
		else if(weapon == BAZOOKA){
			if(updates - lastShot > ROCKETWAIT){ 
				int bx;
				if(dir == LEFT)
					bx = x - 8;
				else
					bx = x+w;
				bullets.push_back(new CBullet(ROCKET, bx, y+20, dir));
				--ammo;
				lastShot = updates;
			}
		}
		else if(weapon == FLAMETHROWER){
			if(updates - lastShot > FLAMEWAIT){
				int bx;
				if(dir == LEFT)
					bx = x - 26;
				else
					bx = x+w;
				bullets.push_back(new CBullet(FLAME, bx, y+14, dir, wpsprites));
				--ammo;
				lastShot = updates;
			}
		}
		else if(weapon == SHOTGUN){
			if(updates - lastShot > SHOTGUNWAIT){
				int bx;
				if(dir == LEFT)
					bx = x;
				else
					bx = x+w;
				bullets.push_back(new CBullet(SHELL, bx, y+24, dir));
				bullets.push_back(new CBullet(SHELL, bx, y+24, dir));
				bullets.push_back(new CBullet(SHELL, bx, y+24, dir));
				bullets.push_back(new CBullet(SHELL, bx, y+24, dir));
				--ammo;
				lastShot = updates;
			}
		}
		if(ammo <= 0)
			weapon = NONE;
	}
}

void CPlayer::handleCollision(ENTITY entity){
	// Only take damage if invulnerability time is over
	if(invulTime <= 0){
		switch(entity){
			case EBULLET: health -= BULLETDAMAGE; break;
			case EROCKET: health -= ROCKETDAMAGE; break;
			case ECHAINSAW: health -= CHAINSAWDAMAGE; break;
			case EFLAME: health -= FLAMEDAMAGE; break;
		}

		// If player has dies: Restore health, ammo, SMG and add 
		if(health <= 0){
			deaths++;
			std::cout << "Player died " << deaths << "times" << std::endl;
			health = 100;
			weapon = SMG;
			ammo = 20;
			invulTime = INVULNERABILITYTIME;
		}
	}
}

void CPlayer::pickupCollision(WEAPONTYPE wp){
	switch(wp){
		case CHAINSAW: weapon = CHAINSAW; ammo = 0; break;
		case SMG: weapon = SMG; ammo = SMGAMMO; break;
		case BAZOOKA: weapon = BAZOOKA; ammo = ROCKETAMMO; break;
		case FLAMETHROWER: weapon = FLAMETHROWER; ammo = FLAMEAMMO; break;
		case SHOTGUN: weapon = SHOTGUN; ammo = SHOTGUNAMMO; break;
	}
}

CSolid CPlayer::getChainsaw(){
	// Return CSolid object of chainsaw for collisions
	CSolid saw(0,y+20,28,16);
	if(dir == LEFT){
		saw.x = x-20;
	}
	else{
		saw.x = x+28;
	}
	return saw;
}

void CPlayer::createExplosion(int _x, int _y){
	bullets.push_back(new CBullet(FLAME, _x, _y-12, DNONE, wpsprites));
	bullets.push_back(new CBullet(FLAME, _x, _y+12, DNONE, wpsprites));
	bullets.push_back(new CBullet(FLAME, _x-12, _y, DNONE, wpsprites));
	bullets.push_back(new CBullet(FLAME, _x+12, _y, DNONE, wpsprites));
	bullets.push_back(new CBullet(FLAME, _x, _y, DNONE, wpsprites));
}

CPlayer::~CPlayer(){
	// Delete all bullets at destruction
	for(int i = 0; i < bullets.size(); ++i){
		delete bullets.at(i);
	}
	bullets.clear();
}
