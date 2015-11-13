#include <stdio.h>
#include <Windows.h>
#include "glew.h"
#include "GL\gl.h"
#include "glut.h"
#include <cmath>
#include "materials.h"
#include "targa.h"
#include "obj.h"


GLuint texture[10], text_num;

// keys
bool keys[256], specialkeys[256], move, location_t = false;

GLdouble fi = -1.46;
GLdouble teta = 2.14, rotate = 0.0, move_y = 0.0, location_x, location_y;
int i = 0;
int timeB = 0;
void initOpenGL(){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT0);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	int w,h;
	GLenum format, type;
	GLvoid *data;
	load_targa("yellobrk.tga", w, h, format, type, data);
	
	glGenTextures(2, &texture[0]);	
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, format, type, data);

}

void specialKeyDown(int key, int x, int y){
	specialkeys[key] = true;

}

void handleKeys(){
	
	if(keys[27])
			exit(0);
	
	if(specialkeys[GLUT_KEY_RIGHT]){
		rotate += 0.2;
	}
	if(specialkeys[GLUT_KEY_LEFT]){
		rotate -= 0.2;
	}
		
	// r: reset scene
	if(keys[114]){
		move = false;
		move_y = 0.0;
		rotate = 0.0;	
	}
	// d:
	if(keys[100]){
		move = true;
	}	

}
void drawScene(){
	handleKeys();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity(); //load identity matrix

	GLdouble r = 16.0;
	GLdouble x = r * cos(teta) * cos(fi);
	GLdouble y = r * sin(teta);
	GLdouble z = r * cos(teta) * sin(fi);

	gluLookAt( x , y, z, 0, 0, 0, r * cos(teta + 0.1) * cos(fi), r * sin(teta + 0.1), r * cos(teta + 0.1) * sin(fi));
	
	glScalef(1.0, 1.0, 1.0);
	//
	//
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glColor3f(1.0,1.0,1.0);
	GLfloat dd = 7.0f;
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-dd, dd, 0.1f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-dd, -dd, 0.1f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(dd, -dd, 0.1f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(dd, dd, 0.1f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	
	glEnable(GL_LIGHTING);
	
	if(location_t){			
		int sign = 1;
		if(location_x/640 > 0.25)
			sign *= -1;
		glTranslatef(location_x/640*i*0.1 * sign, location_y/480*i*0.1, 0);
		printf("location_x = %f, location_x*i*0.01 =  = %f\n", location_x/640, location_x/640*i*0.1);
		if((location_y/480*i*0.1) < 9* location_y/480)
			i++;
	}
	//Draw teapot
	glColor3f(0.4, 0.2, 0.1);
	glTranslatef(0,0,2.0f);
	glRotatef(-264.60,1,0,0);
	
	// Draw sphere

	glTranslatef(5.0f,3.0f + move_y, 3.0f);
	glutSolidSphere(1, 30, 30);
	glTranslatef(-6.0f,-3.0f - move_y , 1.0f);
	glRotatef(rotate,0,1,0);
	
	// Draw a Teapot.
    glutSolidTeapot(3);
	glutSwapBuffers();
	glutPostRedisplay();

	if(move && move_y > - 2.7 && timeB == 0){
		move_y -= 0.01;
	}
	if(move_y <= -2.7 && timeB == 0){
		timeB = 1;
	}
	if(timeB == 1){
		move_y += 0.01;
		if(move_y > -1.0)
			timeB = 2;
	}
	if(timeB == 2){
		move_y -= 0.01;
		if(move_y <= -2.7)
			timeB = 3;
	}
}

void keyDown(unsigned char key, int x, int y){
	keys[key] = true;
}

void keyUp(unsigned char key, int x, int y){
	keys[key] = false;
}

void specialKeyUp(int key, int x, int y){
	specialkeys[key] = false;
}

void reshapeScreen(int w, int h){
	glViewport( 0, 0, (GLsizei) w, (GLsizei) h );
    // 1)Draw coordinate
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)w/(GLfloat)h, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	drawScene();	
}

void mouseFunction(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		location_x = x;
		location_y = y;
		location_t = true;
		i = 0;
	}
	if(state == GLUT_UP)
		location_t = false;
}

int main(int argc, char *argv[]){
	// OpenGL initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_ALPHA);
	// 
	glutInitWindowSize(640,480);
	// 
	glutCreateWindow("OpenGL");
	// 
	initOpenGL();

	GLenum err = glewInit();
	if(GLEW_OK != err){
		printf("Error: %s", glewGetErrorString(err));
	}
	
	// display scene
	glutDisplayFunc(drawScene);	

	// choose function when window reshape
    glutReshapeFunc( reshapeScreen );

	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	
	glutSpecialFunc(specialKeyDown);
	glutSpecialUpFunc(specialKeyUp);

	//Mouse
	glutMouseFunc(mouseFunction);
	glutMainLoop();

	return 0;
}

