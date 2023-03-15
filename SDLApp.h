#ifndef _SDLAPP_H
#define _SDLAPP_H

#include <SDL/SDL.h>
#include "scene/Scene.h"

class SDLApp {

public:
   SDLApp();
   ~SDLApp();
   void start();

private:
   void handleKeydown(SDL_KeyboardEvent &event);
   void handleKeyup(SDL_KeyboardEvent &event);
   void handleMouseMotion(SDL_MouseMotionEvent &event);
   void drawScene();

   bool stopApp;
   Scene* scene;
};

#endif
