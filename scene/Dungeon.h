#ifndef _DUNGEON_H
#define _DUNGEON_H

#include <GL/gl.h>

#include "Camera.h"
#include "Scene.h"
#include <SDL/SDL.h>

class JBDungeon;

class Dungeon : public Scene {

public:
   Dungeon();
   virtual ~Dungeon();

   virtual void setup();
   virtual void draw();
   virtual void handleKeydown(SDL_KeyboardEvent &event);
   virtual void handleKeyup(SDL_KeyboardEvent &event);

private:
   void initializeBackground();
   void draw3d();
   void drawMap();
   void drawLocArrow();

   Camera cam;

   float delU, delV, delN;
   float roll, pitch, yaw;
   double rollVel, slideVel;
   JBDungeon* map;
   bool showMap;
};

#endif
