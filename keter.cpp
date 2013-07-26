#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "al.h"
#include "alc.h"
#include "alut.h"
#include <memory.h>
#include <windows.h>
#include <math.h>

#include "GL/glut.h"
#define NUM_BUFFERS 5
using namespace std;

ALfloat listenerPos[]={-0.7, 0.0, 3.5};
ALfloat listenerVel[]={0.0, 0.0, 0.0};
ALfloat listenerOri[]={0.0, 0.0, 1.0, 0.0, 1.0, 0.0};

const int num_sources = 5;

ALfloat sourcePosMatrix[num_sources][3];
ALfloat sourceVelMatrix[num_sources][3];

float x_uri[num_sources];
float y_uri[num_sources];
float z_uri[num_sources];
float dims[num_sources];

ALuint g_Sources[num_sources];
ALuint g_Buffers[NUM_BUFFERS];
int GLwin;
char command;
int whichCube = 0;

void initialize(int num,float x_uri[],float y_uri[],float z_uri[], float dims[]) {
	ALCdevice *Device;
	ALCcontext *Context;

	ALint error;

	ALsizei size;
	ALsizei freq;
	ALenum format;
	ALvoid *data= 0;
	ALboolean al_bool;

	float k = -3.0;
	float kk = 0.2;
	for(int i = 0; i < num_sources; i++) {
		if(i % 2 == 0) {
			x_uri[i] = k + (float) i;
			z_uri[i] = k + (float) i;
		} else {
			x_uri[i] = k + (float) i;
			z_uri[i] = k + (float) i;
		}
		y_uri[i] = 0.0;
		dims[i] = kk;
		kk += 0.2;
	}			
	
	for (int i = 0 ; i < num_sources;i++){
		sourcePosMatrix[i][0] = x_uri[i];
		sourcePosMatrix[i][1] = y_uri[i];
		sourcePosMatrix[i][2] = z_uri[i];
		sourceVelMatrix[i][0] = 0.0;
		sourceVelMatrix[i][1] = 0.0;
		sourceVelMatrix[i][2] = 0.0;
		}

	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);

	//Initializez Device-ul manual; daca nu sunt erori, initializez contextul (il fac si curent)
	Device = alcOpenDevice(NULL);
	if((error = alGetError()) != AL_NO_ERROR) {
		cout << "You've got an error trying to initialize the device !" << endl;
	} else {
		Context = alcCreateContext(Device, NULL);
		alcMakeContextCurrent(Context);
	}
	
	//Generez buffer-ele
	if((error = alGetError()) != AL_NO_ERROR) {
		cout << "You've got an error trying to generate the buffers !" << endl;
	} else {
		alGenBuffers(NUM_BUFFERS, g_Buffers);
	}

		//Vectorul de 'wav-uri' incarcate-n cate-un buffer
	ALbyte *waveArray[] = {
		(ALbyte *) "beep-6.wav",
		(ALbyte *) "beep-9.wav",
		(ALbyte *) "button-8.wav",
		(ALbyte *) "button-1.wav",
		(ALbyte *) "beep-4.wav"
	};

	for(int i = 0; i < 5; i++) {
		alutLoadWAVFile(waveArray[i], &format, &data, &size, &freq, &al_bool);
		alBufferData(g_Buffers[i], format, data, size, freq);
		alutUnloadWAV(format, data, size, freq);
	}

	//Verificam daca avem erori
	alGetError();
	alGenSources(num_sources, g_Sources);
	if((error = alGetError()) != AL_NO_ERROR) {
		cout << "You've got an error trying to generate the sources !" << endl;
	} else {
		cout << "You're okay ! No Source Generating errors" << endl;
	}

	for(int i = 0; i < num_sources; i++) {
		alSourcef(g_Sources[i], AL_PITCH, 1.0f);
		alSourcef(g_Sources[i], AL_GAIN, 1.0f);
		alSourcefv(g_Sources[i], AL_POSITION, sourcePosMatrix[i]);
		alSourcefv(g_Sources[i], AL_VELOCITY, sourceVelMatrix[i]);
		alSourcei(g_Sources[i], AL_BUFFER, g_Buffers[i]);
		alSourcei(g_Sources[i], AL_LOOPING, AL_TRUE);
	}

	cout << "De la stanga la dreapta:" << endl;
	cout << "Cubul 0 <-> Culoare verde" << endl;
	cout << "Cubul 1 <-> Culoare violet" << endl;
	cout << "Cubul 2 <-> Culoare galben" << endl;
	cout << "Cubul 3 <-> Culoare albastru" << endl;
	cout << "Cubul 4 <-> Culoare galben - verde :)" << endl;

}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glRotatef(25, 1.0, 1.0, 0.0);

	for(float i = 0; i < num_sources; i += 1.0) {
		glPushMatrix();
		glTranslatef(sourcePosMatrix[(int) i][0], sourcePosMatrix[(int) i][1], sourcePosMatrix[(int) i][2]);
		glColor3f((int) i % 3 - 0.5, ((int) i / 2) + 1 - 0.5, ((int) i) % 2);
		glutWireCube(0.7); //dims[i]
		glPopMatrix();
	}

	//Listener
	glPushMatrix();
	glTranslatef(listenerPos[0],listenerPos[1],listenerPos[2]);
    glColor3f(1.0,1.0,1.0);
	glutWireCube(0.5);
	glPopMatrix();

	glPopMatrix();
	glutSwapBuffers();
}

void reshape(int w, int h) // the reshape function
{
   glViewport(0,0,(GLsizei)w,(GLsizei)h) ;
   glMatrixMode(GL_PROJECTION) ;
   glLoadIdentity() ;
   gluPerspective(60.0,(GLfloat)w/(GLfloat)h,1.0,30.0) ;
   glMatrixMode(GL_MODELVIEW) ;
   glLoadIdentity() ;
   glTranslatef(0.0,0.0,-6.6) ;
}

bool clearWay(int cube) {
	float panta;
	panta = (sourcePosMatrix[cube][2] - listenerPos[2]) / (sourcePosMatrix[cube][0] - listenerPos[0]);
	cout << "panta = " <<panta << endl;
	for(int i = 0; i < num_sources; i++) {
		if(i != cube) {
			cout << "alta panta = " << (sourcePosMatrix[i][2] - listenerPos[2]) / (sourcePosMatrix[i][0] - listenerPos[0]) << endl;
			if(abs(panta - (sourcePosMatrix[i][2] - listenerPos[2]) / (sourcePosMatrix[i][0] - listenerPos[0])) <= 0.5) {
				if(sourcePosMatrix[cube][2] < sourcePosMatrix[i][2]) {
					return false;
				}
			}
		}
	}
	return true;
}

void pressKey(unsigned char key, int x, int y) {
	for (int i = 0; i < 2; i++){
		alSourcePlay(g_Sources[i]);
		}
	if(clearWay(whichCube) == false) {
		cout << "Acum nu se mai aude cubul " << whichCube << endl;
		alSourceStop(g_Sources[whichCube]);
	}
	switch(key) {
	    case '0':
		    whichCube = 0;
			cout << "Ai selectat cubul norocos cu numarul " << whichCube << endl;
		    break;
		case '1':
			whichCube = 1;
			cout << "Ai selectat cubul norocos cu numarul " << whichCube << endl;
			break;
		case '2':
			whichCube = 2;
			cout << "Ai selectat cubul norocos cu numarul " << whichCube << endl;
			break;
		case '3':
			whichCube = 3;
			cout << "Ai selectat cubul norocos cu numarul " << whichCube << endl;
			break;
		case '4':
			whichCube = 4;
			cout << "Ai selectat cubul norocos cu numarul " << whichCube << endl;
			break;
		case 'a':
		case 'A':
			sourcePosMatrix[whichCube][0] -= 0.1;
			alSourcefv(g_Sources[whichCube], AL_POSITION, sourcePosMatrix[whichCube]);
			break;
		case 'd':
		case 'D':
			sourcePosMatrix[whichCube][0] += 0.1;
			alSourcefv(g_Sources[whichCube], AL_POSITION, sourcePosMatrix[whichCube]);
			break;
		case 's':
		case 'S':
			sourcePosMatrix[whichCube][2] += 0.1;
			alSourcefv(g_Sources[whichCube], AL_POSITION, sourcePosMatrix[whichCube]);
			break;
		case 'w':
		case 'W':
			sourcePosMatrix[whichCube][2] -= 0.2;
			alSourcefv(g_Sources[whichCube], AL_POSITION, sourcePosMatrix[whichCube]);
			break;
		
		default:
			alSourceStop(g_Sources[whichCube]);
			alutExit();
			glutDestroyWindow(GLwin);
			exit(0);
			break;
		}
	glutPostRedisplay();
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   
	glutInitWindowSize(800, 800);

	alutInit(&argc, argv);

	GLwin = glutCreateWindow("Sound Of Vision!");
	
	initialize(5, x_uri, y_uri, z_uri, dims);
	glutDisplayFunc(display);

	glutKeyboardFunc(pressKey);
	glutReshapeFunc(reshape);
	
	glutMainLoop();

	return 0;
}