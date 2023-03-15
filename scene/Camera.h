#ifndef _CAMERA_H
#define _CAMERA_H

#define FWD   0
#define BACK  180
#define LEFT  270
#define RIGHT 90

class Camera {

public:
   Camera() { }

   void set(float _x, float _y, int direction);
   void turn(int direction);
   void move(int direction);

   void setPosition();
   int getX() const { return (int) (x/1.2); }
   int getY() const { return (int) (y/1.2); }
   int getDir() const { return dir; }

private:
   void updateLookAt();
   float x, y;
   float lx, ly;
   int dir;
};

#endif
