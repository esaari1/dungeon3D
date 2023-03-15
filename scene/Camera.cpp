#include "Camera.h"
#include <math.h>
#include <GL/glu.h>
#include <stdio.h>

void Camera::set(float _x, float _y, int direction) {
   dir = direction;
   x = _x;
   y = _y;
   updateLookAt();
}

void Camera::turn(int direction) {
   if(direction == LEFT) {
      dir -= 90;
      if(dir < 0) {
         dir += 360;
      }
   }
   else if(direction == RIGHT) {
      dir = (dir + 90) % 360;
   }

   updateLookAt();
}

void Camera::move(int direction) {
   float scale = 1.2f;
   if(direction == FWD) {
      switch(dir) {
         case FWD: y += scale; break;
         case BACK: y -= scale; break;
         case LEFT: x -= scale; break;
         case RIGHT: x += scale; break;
      }
   }
   else if(direction == BACK) {
      switch(dir) {
         case FWD: y -= scale; break;
         case BACK: y += scale; break;
         case LEFT: x += scale; break;
         case RIGHT: x -= scale; break;
      }
   }

   updateLookAt();
}

void Camera::setPosition() {
   if(dir == FWD) {
      gluLookAt(x, y-0.5, 0.5, lx, ly, 0.5, 0, 0, 1);
   }
   else if(dir == BACK) {
      gluLookAt(x, y+0.5, 0.5, lx, ly, 0.5, 0, 0, 1);
   }
   else if(dir == LEFT) {
      gluLookAt(x+0.5, y, 0.5, lx, ly, 0.5, 0, 0, 1);
   }
   else {
      gluLookAt(x-0.5, y, 0.5, lx, ly, 0.5, 0, 0, 1);
   }
}

void Camera::updateLookAt() {
   switch(dir) {
      case FWD:   lx = x; ly = y + 1; break;
      case BACK:  lx = x; ly = y - 1; break;
      case LEFT:  lx = x - 1; ly = y; break;
      case RIGHT: lx = x + 1; ly = y; break;
   }
}