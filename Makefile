CC=g++

CFLAGS=`sdl-config --cflags`

LDFLAGS=`sdl-config --libs` -lSDL_ttf -lSDL_mixer

SOURCES=CApp.cpp CSolid.cpp CPlayer.cpp CBullet.cpp CBloodsplatter.cpp CPickup.cpp

OBJECTS=CApp.o CSolid.o CPlayer.o CBullet.o CBloodsplatter.o CPickup.o

EXENAME=jetpacks

main: CApp.cpp
	$(CC) $(CFLAGS) -c $(SOURCES)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXENAME)

clean:
	rm -rf $(OBJECTS)
