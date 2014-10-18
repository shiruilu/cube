/* OpenGL demo - makes a spinning textured cube */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <GL/glut.h>
#include <math.h>

#include "ppmRead.h"

#define PI 3.141593

/* prototypes */
void advance(int unused);
void mousePress(int btn, int state, int x, int y);
void drawCube(void);
void display(void);
void init(void);
int setTexture(char *s);
int saveImage(char *s);

/* a unit square in the x,y plane */
GLfloat unitSqr[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0 };
GLfloat angle = 0;
GLubyte *texpat;
FILE *fin;
GLsizei w,h;
int spinning = 1;
GLuint texName[2]; /* array to hold texture names */


/**************************************************************************
	Stub for your image saving routine
****************************************************************************/
int saveImage(char *s)
{

  printf("DB save image to file %s\n", s);

  return 0;
}

/**************************************************************************
	Reads in a ppm file (name given in s) and loads it as a texture.
	File should have height and width of a power of 2.  Returns 0
	if error detected, otherwise returns 1
****************************************************************************/
int setTexture(char *s)
{
  if ( !(fin = fopen(s, "rb")) )  {  return 0; }
  texpat = readPPM(fin, &w, &h); /* w and h must be a power of 2 */
  
  if (texpat == NULL) return 0;
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  // Uncomment one of the glTexEnvi lines below
  // (GL_MODULATE is the default)
  // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTexImage2D( GL_TEXTURE_2D, /* target */ 0, /* level */
    3, /* components */
    w, h, /* width, height */ 0, /* border */
    GL_RGB,  /* format */   GL_UNSIGNED_BYTE, /* type */
    texpat);
  free(texpat); /* free the texture memory */
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  return 1;
}


/**************************************************************************
	Register as the mouseFunc callback - allows the user to
	start and stop the spinning of the model with a button click
****************************************************************************/
void mousePress(int btn, int state, int x, int y)
{
  int err;
  if ( state != GLUT_DOWN) return;
  if (GLUT_ACTIVE_SHIFT == glutGetModifiers()) {
    spinning =0;
    err = saveImage("savedImg.ppm");
    printf("error code: %d\n", err);
  } else
  spinning = 1 - spinning;
}



/**************************************************************************
	Continually registers using the timer function to advance the spin
	angle in time.  The screen is redrawn after each update to angle.
****************************************************************************/
void advance(int unused)
{
  if (spinning)
  angle+= 12;
        
  glutTimerFunc(100, advance, 0);
  glutPostRedisplay();
}

/**************************************************************************
	Draws a cube with all sides except the top.  Each side is textured with
	1 of 2 textures.  The cube rotates based upon the angle value which
	is updated by a separate routine.  Textures are mapped to appear
	correctly when their outside surface is viewed.  You can see the texture
	on the inner surface also but the lettering appears "mirrored"
****************************************************************************/
void drawCube(void)
{
  int i;
  glPushMatrix(); /* save CTM */
  glLoadIdentity();
	
  glRotatef(45, 1.0,0.0,0.0);  /* rotX(45), make an interesting view angle */
  glRotatef(angle, 0.0, 1.0, 0.0); /* spin  about y axis */
  glTranslatef(-0.5,-0.5,0.5);  /* move unit square so unit cube is at origin */
  glColor4f(0.0, 1.0, 0.0, 0.5);
  /* front face */	
  glEnable(GL_TEXTURE_2D);
    
  glBindTexture(GL_TEXTURE_2D, texName[0]);
  glBegin(GL_QUADS);
  for (i=0; i<8; i+=2) {
    glTexCoord2f(unitSqr[i], unitSqr[i+1]);
    glVertex2fv(unitSqr+i);
  }		
  glEnd();

  glPushMatrix();

  /* left side */
  glRotatef(-90.0, 0.0, 1.0, 0.0);	
  glTranslatef(-1.0,0.0, 0.0);
  glBindTexture(GL_TEXTURE_2D, texName[0]); 
  glBegin(GL_QUADS);
  for (i=0; i<8; i+=2) {
    glTexCoord2f(unitSqr[i], unitSqr[i+1]);
    glVertex2fv(unitSqr+i);
  }		
  glEnd();
  glPopMatrix();		
    
  glPushMatrix();
    
  /* bottom */
  glRotatef(-90.0, 1.0, 0.0, 0.0);
  glBindTexture(GL_TEXTURE_2D, texName[0]);  
  glBegin(GL_QUADS);
  for (i=0; i<8; i+=2) {
    // glTexCoord2f(3*unitSqr[i], 3*unitSqr[i+1]);
    glTexCoord2f(unitSqr[i], unitSqr[i+1]);
    glVertex2fv(unitSqr+i);
  }		
  glEnd();
  glPopMatrix();	

  glPushMatrix(); 

  /* top */
  glRotatef(-90.0, 1.0, 0.0, 0.0);
  glTranslatef(0.0, 0.0, 1.0);
  glBindTexture(GL_TEXTURE_2D, texName[0]);  
  glBegin(GL_QUADS);
  for (i=0; i<8; i+=2) {
    // glTexCoord2f(3*unitSqr[i], 3*unitSqr[i+1]);
    glTexCoord2f(unitSqr[i], unitSqr[i+1]);
    glVertex2fv(unitSqr+i);
  }		
  glEnd();
  glPopMatrix();	

  glPushMatrix(); 

  /* back */
  glTranslatef(1.0, 0.0, -1.0);
  glRotatef(180, 0.0, 1.0, 0.0);
  glBindTexture(GL_TEXTURE_2D, texName[0]);	  
  glBegin(GL_QUADS);
  for (i=0; i<8; i+=2) {
    // glTexCoord2f(2*unitSqr[i], 2*unitSqr[i+1]);
    glTexCoord2f(unitSqr[i], unitSqr[i+1]);
    glVertex2fv(unitSqr+i);
  }		
  glEnd();
  glPopMatrix();	

  glPushMatrix();
      
  /* right */
  glTranslatef(1.0, 0.0, 0.0);
  glRotatef(90.0, 0.0, 1.0, 0.0);	 
  // glBindTexture(GL_TEXTURE_2D, texName[1]);
  glBindTexture(GL_TEXTURE_2D, texName[0]);
  glBegin(GL_QUADS);
  for (i=0; i<8; i+=2) {
    // glTexCoord2f(2*unitSqr[i], 2*unitSqr[i+1]);
    glTexCoord2f(unitSqr[i], unitSqr[i+1]);
    glVertex2fv(unitSqr+i);
  }		
  glEnd();
  glPopMatrix();	
	
  glPopMatrix();	
}


/**************************************************************************
	Registered as the display function for GLUT.  Clears the buffer and
	redraws the cube.
****************************************************************************/
void display(void)
{
  /* clear the color buffer */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  drawCube();

  glutSwapBuffers();
}



/**************************************************************************
	Initializes buffer, matrix and loads and binds texture objects
****************************************************************************/
void init(void)
{
  /* define color to clear with */
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0, 1.0,-1.0,1.0,-1.0,1.0);
  /* define the texture objects */
  glGenTextures(2, texName); /* make 2 texture names */
  glBindTexture(GL_TEXTURE_2D, texName[0]);
  setTexture("../../perlin_noise/build/demo.ppm");
  // glBindTexture(GL_TEXTURE_2D, texName[1]);
  // setTexture("../../perlin_noise/build/demo.ppm");
  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);
}

/**************************************************************************
****************************************************************************/
int main(int argc, char **argv)
{
  int mainWin;

  glutInit(&argc, argv);
 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(640,640);
  mainWin = glutCreateWindow("Perlin Noise Test");
  init();
  glutDisplayFunc(display);
  glutSetWindow(mainWin);
  glutTimerFunc(250, advance, 0);
  glutMouseFunc(mousePress);
  glutMainLoop();
  return 0;
}