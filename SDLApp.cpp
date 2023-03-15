#include "SDLApp.h"
#include "constants.h"
#include "math/Matrix.h"
#include "scene/Dungeon.h"
#include <GL/gl.h>
#include <GL/glu.h>

SDLApp::SDLApp() :
   stopApp(false), scene(new Dungeon())
{
   int w = 960;
   int h = 540;
   double near = 0.1;

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0f , (float) w / h , 0.1f , 10000.0f);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   GLfloat ambient_light[] = {0.0, 0.0, 0.0, 1.0};
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
//   glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);

   GLfloat diffuse_light[] = {1.0, 1.0, 1.0, 1.0};
   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
//   glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 2);
//   glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1);

   //Depth states
   glClearDepth(1.0f);
   glEnable(GL_DEPTH_TEST);

   glEnable(GL_CULL_FACE);
   glFrontFace(GL_CCW);
   glShadeModel(GL_SMOOTH);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
}

SDLApp::~SDLApp() {
   delete scene;
   SDL_Quit();
}

void SDLApp::start() {
   SDL_Event event;
   while(!stopApp) {
      while ( SDL_PollEvent(&event)) {
         switch (event.type) {
            case SDL_KEYDOWN :
               handleKeydown(event.key);
               break;
            case SDL_KEYUP :
               handleKeyup(event.key);
               break;
            case SDL_MOUSEMOTION :
//               handleMouseMotion(event.motion);
               break;
            case SDL_QUIT:
               stopApp = true;
               break;
         }
      }

      drawScene();
   }
}

void SDLApp::drawScene() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   scene->draw();
   glFlush();
   SDL_GL_SwapBuffers();
}

void SDLApp::handleKeydown(SDL_KeyboardEvent &event) {
   switch(event.keysym.sym) {
      default: break;
   }
}

void SDLApp::handleKeyup(SDL_KeyboardEvent &event) {
   switch(event.keysym.sym) {
      case SDLK_ESCAPE :
         stopApp = true;
         break;
      default:
         scene->handleKeyup(event);
         break;
   }
}

void SDLApp::handleMouseMotion(SDL_MouseMotionEvent &event) {
}
