#include <stdlib.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "Non_Cuda_Complex.cu"
#pragma hdrstop
using namespace std;

int width=512, height=512; // window size
int windowID;

GLfloat minX = -2.0f, maxX = 1.0f, minY = -1.2f, maxY = 1.8f; // complex plane boundaries
GLfloat stepX = (maxX - minX)/(GLfloat)width;
GLfloat stepY = (maxY - minY)/(GLfloat)height;

GLfloat black[] = {0.0f, 0.0f, 0.0f}; // black color
GLfloat white[] = {1.0f, 1.0f, 1.0f}; // white color
const int paletteSize = 128;
GLfloat palette[paletteSize][3];

bool fullScreen=false;

double maxIteration = 2000;
//****************************************
//GLfloat* calculateColor(GLfloat u, GLfloat v){
//	GLfloat re = u;
//	GLfloat im = v;
//	GLfloat tempRe=0.0;
//	for(int i=0; i < paletteSize; i++){
//		tempRe = re*re - im*im + u;
//		im = re * im * 2 + v;
//		re = tempRe;
//		if( (re*re + im*im) > radius ){
//			return palette[i];
//		}
//	}
//	return black;
//}

double startMouseClickX = 0.0;
double startMouseClickY = 0.0;
double endMouseClickX = 0.0;
double endMouseClickY = 0.0;
double currentMouseX = 0.0;
double currentMouseY = 0.0;
bool isBox = false;

void drag (int x, int y){
	//	cout << "============================="<<endl;
	//	cout << x << '\t' <<y<< endl;
	currentMouseX = x;
	currentMouseY = y;

}

void mouse(int button, int state, int x, int y){
	//	cout << "============================="<<endl;

	if (state==GLUT_DOWN)	{
		cout << "DOWN" <<endl;
		//		cout << "x: " << x << "\n";
		//		cout << "y: " << y << "\n";
		startMouseClickX = x;
		startMouseClickY = y;


	}
	if (state==GLUT_UP)	{
		cout << "UP" <<endl;

		//		cout << "x: " << x << "\n";
		//		cout << "y: " << y << "\n";
		endMouseClickX = x;
		endMouseClickY = y;
		isBox = true;
		cout << "Redisplaying" <<endl;
		glutPostRedisplay();
		isBox = false;
	}
}

GLfloat* calculateColor(GLfloat u, GLfloat v){
	GLfloat re = u;
	GLfloat im = v;
	GLfloat tempRe=0.0;
	Complex c = Complex((float)re,(float)im);
	Complex Zn0 = c;
	Complex Zn1(0,0);
	bool isWhite = false;
	short isWhiteIter = -100;

	for (int i = 0; i < maxIteration; ++i) {
		Zn1 = Zn0*Zn0 + c;
		if (Zn1.magnitude2() > 2.0*2.0) {
			isWhite = true;
			isWhiteIter = i;
			break;
			cout << "breaking!!";
		}
		Zn0 = Zn1;
	}

	if(isWhite && isWhiteIter >= 0)	{
		return palette[isWhiteIter%128];
	}
	else return black;
}

//****************************************

GLfloat* mandelImage[512][512];

void repaint() {// function called to repaint the window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen buffer
	glBegin(GL_POINTS); // start drawing in single pixel mode

	short cx = 0;
	short cy = 0;

	for(GLfloat y = maxY; y >= minY; y -= stepY){
		for(GLfloat x = minX; x <= maxX; x += stepX){
			glColor3fv(mandelImage[cx][cy]); // set color
			glVertex2f(x, y); // put pixel on screen (buffer) - [ 1 ]
			cx++;
		}
		cy++;
		cx = 0;
	}
	glEnd(); // end drawing
	if (isBox) {
		float side = ((endMouseClickX - startMouseClickX) < (endMouseClickY - startMouseClickY)) ? endMouseClickX - startMouseClickX : endMouseClickY - startMouseClickY;
		endMouseClickX = startMouseClickX + side;
		endMouseClickY = startMouseClickY + side;
		float topLeftXTransformed = (startMouseClickX - 256.0)/256.0;
		float topLeftYTransformed = (256.0 - startMouseClickY)/256.0;

		float bottomRightXTransformed = (endMouseClickX - 256.0)/256.0;
		float bottomRightYTransformed = (256.0 - endMouseClickY)/256.0;

		cout<<"Drawing red box: ("<<topLeftXTransformed<<", "<<topLeftYTransformed<<") : (" << bottomRightXTransformed <<", "<< bottomRightYTransformed <<")\n";
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(topLeftXTransformed, topLeftYTransformed);
		glVertex2f(bottomRightXTransformed,topLeftYTransformed);
		glVertex2f(bottomRightXTransformed,bottomRightYTransformed);
		glVertex2f(topLeftXTransformed,bottomRightYTransformed);
		glEnd();
	}

	glutSwapBuffers(); // swap the buffers - [ 2 ]
}

//****************************************
void reshape (int w, int h){ // function called when window size is changed
	stepX = (maxX-minX)/(GLfloat)w; // calculate new value of step along X axis
	stepY = (maxY-minY)/(GLfloat)h; // calculate new value of step along Y axis
	glViewport (0, 0, (GLsizei)w, (GLsizei)h); // set new dimension of viewable screen
	glutPostRedisplay(); // repaint the window
}

//****************************************
void keyFunction(unsigned char key, int x, int y){ // function to handle key pressing
	switch(key){
	case 'F': // pressing F is turning on/off fullscreen mode
	case 'f':
		if(fullScreen){
			glutReshapeWindow(width,height); // sets default window size
			GLsizei windowX = (glutGet(GLUT_SCREEN_WIDTH)-width)/2;
			GLsizei windowY = (glutGet(GLUT_SCREEN_HEIGHT)-height)/2;
			glutPositionWindow(windowX, windowY); // centers window on the screen
			fullScreen = false;
		}
		else{
			fullScreen = true;
			glutFullScreen(); // go to fullscreen mode
		}
		glutPostRedisplay();
		break;
	case 27 : // escape key - close the program
		glutDestroyWindow(windowID);
		exit(0);
		break;
	}
}

//****************************************
void createPalette(){
	int eight = 4;
	int four = 2;
	for(int i=0; i < 32; i++){
		palette[i][0] = (eight*i)/(GLfloat)255;
		palette[i][1] = (128-four*i)/(GLfloat)255;
		palette[i][2] = (255-eight*i)/(GLfloat)255;
	}
	for(int i=0; i < 32; i++){
		palette[32+i][0] = (GLfloat)1;
		palette[32+i][1] = (eight*i)/(GLfloat)255;
		palette[32+i][2] = (GLfloat)0;
	}
	for(int i=0; i < 32; i++){
		palette[64+i][0] = (128-four*i)/(GLfloat)255;
		palette[64+i][1] = (GLfloat)1;
		palette[64+i][2] = (eight*i)/(GLfloat)255;
	}
	for(int i=0; i < 32; i++){
		palette[96+i][0] = (GLfloat)0;
		palette[96+i][1] = (255-eight*i)/(GLfloat)255;
		palette[96+i][2] = (eight*i)/(GLfloat)255;
	}
}

//****************************************
int main(int argc, char** argv){
	glutInit(&argc, argv);
	createPalette();
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	GLsizei windowX = (glutGet(GLUT_SCREEN_WIDTH)-width)/2;
	GLsizei windowY = (glutGet(GLUT_SCREEN_HEIGHT)-height)/2;
	glutInitWindowPosition(windowX, windowY);
	glutInitWindowSize(width, height);
	windowID = glutCreateWindow("MANDELBROTH");

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glViewport (0, 0, (GLsizei) width, (GLsizei) height);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(minX, maxX, minY, maxY, ((GLfloat)-1), (GLfloat)1);

	// set the event handling methods

	short cx = 0;
	short cy = 0;
	for(GLfloat y = maxY; y >= minY; y -= stepY){
		for(GLfloat x = minX; x <= maxX; x += stepX){
			GLfloat* temp  = calculateColor(x,y);
			//			cout << temp;
			//			cout << cx <<"\t"<< cy <<endl;
			mandelImage[cx][cy] = temp;
			cx++;
		}
		cy++;
		cx = 0;
	}

	glutDisplayFunc(repaint);
	//	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyFunction);
	glutMouseFunc(mouse);
	glutMotionFunc(drag);

	glutMainLoop();

	return 0;
}
