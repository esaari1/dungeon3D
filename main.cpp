#include <SDL/SDL.h>
#include "SDLApp.h"

void initSDL() {
   int w = 960;
   int h = 540;

   SDL_Init(SDL_INIT_VIDEO);
   SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
   SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
   SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
   SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 8 );
   SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
   SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );
   SDL_SetVideoMode(w, h, 0, SDL_OPENGL | SDL_HWSURFACE | SDL_DOUBLEBUF); // | SDL_FULLSCREEN);
}

int main(int argc, char **argv) {
   initSDL();
   SDLApp app;
   app.start();
   return 0;
}
