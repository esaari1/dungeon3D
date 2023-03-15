#include "Dungeon.h"
#include "generator/jbdungeon.h"
#include <math.h>

const GLfloat lightPosition[] = {0, 0, 0, 1};

Dungeon::Dungeon() :
   cam(),
   delU(0), delV(0), delN(0),
   roll(0), pitch(0), yaw(0)
{
   slideVel = pow(2.0, 2.0) / 100.0;
   rollVel = pow(3.0, 2.0) / 100.0;

   glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

   JBDungeonOptions options;
      options.size.x = 16;
      options.size.y = 16;
      options.minRoomX = 2;
      options.maxRoomX = 5;
      options.minRoomY = 2;
      options.maxRoomY = 5;

   int aveRoomArea = (int)
                 ( ( ( options.maxRoomX + options.minRoomX ) / 2.0 ) +
                 ( ( options.maxRoomY + options.minRoomY ) / 2.0 ) );

   int lw = (int)( ( options.size.x + options.size.y ) * 0.5 );

   int minMod = 1;
   int maxMod = 2;

   options.sparseness = (int)( lw * 0.5 );
   options.minRoomCount = (int)( ( lw * 4.0 / aveRoomArea ) * minMod );
   options.maxRoomCount = (int)( ( lw * 4.0 / aveRoomArea ) * maxMod );

   map = new JBDungeon(options);
   cam.set(1.8, 1.8, FWD);
   showMap = false;
}

Dungeon::~Dungeon() {
   delete map;
}

void Dungeon::setup() {
}

void axis() {
   glDisable(GL_LIGHTING);

   glBegin(GL_LINES);
   glColor3f(1, 0, 0);
   glVertex3i(0, 0, 0);
   glVertex3i(10, 0, 0);

   glColor3f(0, 1, 0);
   glVertex3i(0, 0, 0);
   glVertex3i(0, 10, 0);

   glColor3f(0, 0, 1);
   glVertex3i(0, 0, 0);
   glVertex3i(0, 0, 10);
   glEnd();

   glEnable(GL_LIGHTING);
}

void Dungeon::draw() {
   glPushMatrix();
   cam.setPosition();

   if(showMap) {
      drawMap();
   }
   else {
      axis();
      draw3d();
   }
   glPopMatrix();
}

float thick = 0.1f;
float scale = 1 + thick * 2;
float height = 0.85;

void Dungeon::draw3d() {
   float white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
   float red[] = { 1.0f, 0.0f, 0.0f, 1.0f };

   glBegin(GL_QUADS);

   for( int j = 0; j < map->getY(); j++ ) {
      for( int i = 0; i < map->getX(); i++ ) {
         int dir = map->getDungeonAt( i, j, 0 );
         if( dir == JBDungeon::c_PASSAGE || dir == JBDungeon::c_ROOM) {
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
            glNormal3f(0, 0, 1);
            glVertex3f(i * scale, j * scale, 0);
            glVertex3f(i * scale + scale, j * scale, 0);
            glVertex3f(i * scale + scale, j * scale + scale, 0);
            glVertex3f(i * scale, j * scale + scale, 0);
         }

         if( (i < map->getX() - 1) && (j < map->getY() - 1)) {
            JBMazePt p1( i, j, 0 );
            JBMazePt p2( i, j+1, 0 );
            JBMazePt p3( i+1, j, 0 );

            int wall = map->getWallBetween( p1, p2 ); /* south wall */
            if( wall != JBDungeonWall::c_NONE ) {
               if(wall > 1) {
                  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
               } else {
                  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
               }
               glNormal3f(0, 1, 0);
               float y = j * scale + scale + thick;
               glVertex3f(i*scale-thick, y, 0);
               glVertex3f(i*scale-thick, y, height);
               glVertex3f(i*scale+scale+thick, y, height);
               glVertex3f(i*scale+scale+thick, y, 0);

               y = j * scale + scale - thick;
               glNormal3f(0, -1, 0);
               glVertex3f(i*scale-thick, y, 0);
               glVertex3f(i*scale+scale+thick, y, 0);
               glVertex3f(i*scale+scale+thick, y, height);
               glVertex3f(i*scale-thick, y, height);
            }

            wall = map->getWallBetween( p1, p3 ); /* east wall */
            if( wall != JBDungeonWall::c_NONE ) {
               if(wall > 1) {
                  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
               } else {
                  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
               }

               float x = i * scale + scale + thick;

               glNormal3f(1, 0, 0);
               glVertex3f(x, j*scale-thick, 0);
               glVertex3f(x, j*scale+scale+thick, 0);
               glVertex3f(x, j*scale+scale+thick, height);
               glVertex3f(x, j*scale-thick, height);

               x = i * scale + scale - thick;

               glNormal3f(-1, 0, 0);
               glVertex3f(x, j*scale-thick, 0);
               glVertex3f(x, j*scale-thick, height);
               glVertex3f(x, j*scale+scale+thick, height);
               glVertex3f(x, j*scale+scale+thick, 0);
            }
         }
      }
   }
   glEnd();
}

void Dungeon::drawMap() {
   glDisable(GL_LIGHTING);

   int ofs = 0;
   int m_gridSize = 15;
   int vPort[4];
   glGetIntegerv(GL_VIEWPORT, vPort);

   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();
   glOrtho(0, vPort[2], 0, vPort[3], -1, 1);

   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();

   int x1, y1, x2, y2;

   glColor3f(1.0, 1.0, 1.0);
   glBegin(GL_QUADS);
   /* draw specific walls and doors, by checking each point and the points
    * to the left and below it to see if there is a wall between them. */
   for( int i = 0; i < map->getX() - 1; i++ ) {
      for( int j = 0; j < map->getY() - 1; j++ ) {
         JBMazePt p1( i, j, 0 );
         JBMazePt p2( i, j+1, 0 );
         JBMazePt p3( i+1, j, 0 );

         int wall = map->getWallBetween( p1, p2 ); /* south wall */
         if( wall != JBDungeonWall::c_NONE ) {
            x1 = ofs + i * m_gridSize;
            y1 = ofs + (j+1) * m_gridSize - 1;
            x2 = ofs + (i+1)*m_gridSize;
            y2 = ofs + (j+1) * m_gridSize + 1;

            if(wall > 1) glColor3f(1, 0, 0);
            else glColor3f(1, 1, 1);

            glVertex3i(x1, y1, 0);
            glVertex3i(x2, y1, 0);
            glVertex3i(x2, y2, 0);
            glVertex3i(x1, y2, 0);
         }
         wall = map->getWallBetween( p1, p3 ); /* east wall */
         if( wall != JBDungeonWall::c_NONE ) {
            if(wall > 1) glColor3f(1, 0, 0);
            else glColor3f(1, 1, 1);

            x1 = ofs + (i+1) * m_gridSize - 1;
            y1 = ofs + j * m_gridSize;
            x2 = ofs + (i+1) * m_gridSize + 1;
            y2 = ofs + (j+1) * m_gridSize;

            glVertex3i(x1, y1, 0);
            glVertex3i(x2, y1, 0);
            glVertex3i(x2, y2, 0);
            glVertex3i(x1, y2, 0);
         }
      }
   }
   glEnd();

   drawLocArrow();

   /* draw the basic walls and floors */
   glColor3f(0.5, 0.5, 0.5);
   glBegin(GL_QUADS);
   for( int j = 0; j < map->getY(); j++ ) {
      for( int i = 0; i < map->getX(); i++ ) {
         int dir = map->getDungeonAt( i, j, 0 );
         if( dir == JBDungeon::c_PASSAGE ) {
            x1 = ofs + i * m_gridSize;
            y1 = ofs + j * m_gridSize;
            x2 = ofs + (i+1) * m_gridSize;
            y2 = ofs + (j+1) * m_gridSize;
         } else if( dir == JBDungeon::c_ROOM ) {
            x1 = ofs + i * m_gridSize;
            y1 = ofs + j * m_gridSize;
            x2 = ofs + (i+1) * m_gridSize;
            y2 = ofs + (j+1) * m_gridSize;
         }

         glVertex3f(x1, y1, 0);
         glVertex3f(x2, y1, 0);
         glVertex3f(x2, y2, 0);
         glVertex3f(x1, y2, 0);
      }
   }
   glEnd();

   glPopMatrix();
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   glMatrixMode(GL_MODELVIEW);

   glEnable(GL_LIGHTING);
}

void Dungeon::drawLocArrow() {
   int m_gridSize = 15;
   int x = cam.getX() * m_gridSize + 7;
   int y = cam.getY() * m_gridSize;

   glPushMatrix();
   glTranslatef(x, y+6, 0);
   glRotatef(cam.getDir(), 0, 0, 1);

   glColor3f(1, 1, 1);
   glBegin(GL_LINES);
   glVertex2f(0, 0-6);
   glVertex2f(0, 0+6);
   glVertex2f(0-5, 0+4);
   glVertex2f(0+5, 0+4);
   glEnd();

   glPopMatrix();
}

void Dungeon::handleKeydown(SDL_KeyboardEvent &event) {
   switch(event.keysym.sym) {
      default: break;
   }
}

void Dungeon::handleKeyup(SDL_KeyboardEvent &event) {
   switch(event.keysym.sym) {
      case SDLK_LEFT:
         cam.turn(LEFT);
         break;
      case SDLK_RIGHT:
         cam.turn(RIGHT);
         break;
      case SDLK_UP:
         cam.move(FWD);
         break;
      case SDLK_DOWN:
         cam.move(BACK);
         break;
      case SDLK_TAB:
         showMap = !showMap;
         break;
      default: break;
   }
}
