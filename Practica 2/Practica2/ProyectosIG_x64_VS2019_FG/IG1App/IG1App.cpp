#include "IG1App.h"
#include "CheckML.h"
#include <iostream>

using namespace std;

//-------------------------------------------------------------------------
// static single instance (singleton pattern)

IG1App IG1App::s_ig1app;  // default constructor (constructor with no parameters)

//-------------------------------------------------------------------------

void IG1App::close()  
{
	if (!mStop) {  // if main loop has not stopped
		cout << "Closing glut...\n";
		glutLeaveMainLoop();  // stops main loop and destroy the OpenGL context
		mStop = true;   // main loop stopped  
	}
	free();
}
//-------------------------------------------------------------------------

void IG1App::run()   // enters the main event processing loop
{
	if (mWinId == 0) { // if not initialized
		init();       // initialize the application 
	
		glutMainLoop();      // enters the main event processing loop 
		mStop = true;  // main loop has stopped  
		
	}
}
//-------------------------------------------------------------------------

void IG1App::init()
{
	// create an OpenGL Context
	iniWinOpenGL();   

	// create the scene after creating the context 
	// allocate memory and resources
	mViewPort = new Viewport(mWinW, mWinH); //glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)
	mCamera = new Camera(mViewPort);
	mScene = new Scene;

	mViewPortB = new Viewport(0, 0);
	mCameraB = new Camera(mViewPortB);
	mSceneB = new Scene();

	mCamera->set2D();
	mScene->init();
}
//-------------------------------------------------------------------------

void IG1App::iniWinOpenGL() 
{  // Initialization
	cout << "Starting glut...\n";
	int argc = 0;
	glutInit(&argc, nullptr);
		
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);  // GLUT_CORE_PROFILE
	glutInitContextFlags(GLUT_DEBUG);		// GLUT_FORWARD_COMPATIBLE

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(mWinW, mWinH);   // window size
	//glutInitWindowPosition (140, 140);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH /*| GLUT_STENCIL*/); // RGBA colors, double buffer, depth buffer and stencil buffer   
	
	mWinId = glutCreateWindow("IG1App");  // with its associated OpenGL context, return window's identifier 
	
	// Callback registration
	glutReshapeFunc(s_resize);
	glutKeyboardFunc(s_key);
	glutSpecialFunc(s_specialKey);
	glutDisplayFunc(s_display);
	glutIdleFunc(s_update);
	glutMouseFunc(s_mouse);
	glutMotionFunc(s_motion);
	glutMouseWheelFunc(s_mouseWheel);
	
	cout << glGetString(GL_VERSION) << '\n';
	cout << glGetString(GL_VENDOR) << '\n';
}
//-------------------------------------------------------------------------

void IG1App::free() 
{  // release memory and resources
	delete mScene; mScene = nullptr;
	delete mCamera; mCamera = nullptr;
	delete mViewPort; mViewPort = nullptr;

	delete mSceneB; mSceneB = nullptr;
	delete mCameraB; mCameraB = nullptr;
	delete mViewPortB; mViewPortB = nullptr;
}
//-------------------------------------------------------------------------

void IG1App::display() const   
{  // double buffering

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clears the back buffer
	
	if (!m2Vistas) {
		mScene->render(*mCamera);  // uploads the viewport and camera to the GPU
	}
	else{
		display2Vistas();
	}

	glutSwapBuffers();	// swaps the front and back buffer
}
//-------------------------------------------------------------------------

void IG1App::resize(int newWidth, int newHeight) 
{
	mWinW = newWidth; mWinH = newHeight;

	// Resize Viewport to the new window size
	mViewPort->setSize(newWidth, newHeight);

	// Resize Scene Visible Area such that the scale is not modified
	mCamera->setSize(mViewPort->width(), mViewPort->height()); 
}
//-------------------------------------------------------------------------

void IG1App::key(unsigned char key, int x, int y) 
{
	bool need_redisplay = true;
	
	switch (key) {
	case 27:  // Escape key 
		glutLeaveMainLoop();  // stops main loop and destroy the OpenGL context
	case '+':
		if (m2Vistas && mCoord.x > mWinW / 2) {
			mCameraB->setScale(0.05);
		}
		else mCamera->setScale(+0.05);  // zoom in  (increases the scale)
		break;
	case '-':
		if (m2Vistas && mCoord.x > mWinW / 2) {
		mCameraB->setScale(-0.05);
		}
		else mCamera->setScale(-0.05);  // zoom out (decreases the scale)
		break;
	case 'l':
		if (m2Vistas && mCoord.x > mWinW / 2) {
		mCameraB->set3D();
		}
		else mCamera->set3D();
		break;
	case 'o':
		if (m2Vistas && mCoord.x > mWinW / 2) {
		mCameraB->set2D();
		}
		else mCamera->set2D();	
		break;
	case 'U':
		if (m2Vistas && mCoord.x > mWinW / 2) {
			mSceneB->setUpdate();
		}
		else mScene->setUpdate();
		break;
	case 'u':
		if (m2Vistas && mCoord.x > mWinW / 2) {
			mSceneB->updateTriangle();
		}
		else mScene->updateTriangle();
		break;
	case 'p':
		if (m2Vistas && mCoord.x > mWinW / 2) {
			mCameraB->changePrj();
		}
		else mCamera->changePrj();
		break;
	case '0':
		if (m2Vistas && mCoord.x > mWinW / 2) {
			mCameraB->set2D();
			mSceneB->changeScene(0);
		}
		else{
		mCamera->set2D();
		mScene->changeScene(0);
		}
		break;
	case '1':
		if (m2Vistas && mCoord.x > mWinW / 2) {
			mCameraB->set3D();
			mSceneB->changeScene(1);
		}
		else{
		mCamera->set3D();
		mScene->changeScene(1);
		}
		break;
	case '2':
		if (m2Vistas && mCoord.x > mWinW / 2) {
			mCameraB->set3D();
			mSceneB->changeScene(2);
		}
		else{
		mCamera->set3D();
		mScene->changeScene(2);
		}
		break;
	case '3':
		if (m2Vistas && mCoord.x > mWinW / 2) {
			mCameraB->set3D();
			mSceneB->changeScene(3);
		}
		else{
		mCamera->set3D();
		mScene->changeScene(3);
		}
		break;
	case '4':
		if (m2Vistas && mCoord.x > mWinW / 2) {
			mCameraB->set2D();
			mSceneB->changeScene(4);
		}
		else{
		mCamera->set2D();
		mScene->changeScene(4);
		}
		break;
	case '5':
		if (m2Vistas && mCoord.x > mWinW / 2) {
			mCameraB->set2D();
			mSceneB->changeScene(5);
		}
		else{
		mCamera->set2D();
		mScene->changeScene(5);
		}
		break;
	case '6':
		if (m2Vistas && mCoord.x > mWinW / 2) {
			mCameraB->set2D();
			mSceneB->changeScene(6);
		}
		else{
		mCamera->set2D();
		mScene->changeScene(6);
		}
		break;
	case '7':
		if (m2Vistas && mCoord.x > mWinW / 2) {
			mCameraB->set2D();
			mSceneB->changeScene(7);
		}
		else{
		mCamera->set2D();
		mScene->changeScene(7);
		}
		break;
	case '8':
		if (m2Vistas && mCoord.x > mWinW / 2) {
			mCameraB->set2D();
			mSceneB->changeScene(8);
		}
		else{
		mCamera->set2D();
		mScene->changeScene(8);
		}
		break;
	case'f':
	case 'F':
		if (m2Vistas && mCoord.x > mWinW / 2) {
			mSceneB->savePhoto();
		}
		else mScene->savePhoto();
		break;
	case 'k':
		m2Vistas = !m2Vistas;
		break;
	case 'q':
		mScene->enableDirLight();
		break;
	case 'w':
		mScene->disableDirLight();
		break;
	case 'a':
		mScene->enablePosLight();
		break;
	case 's':
		mScene->disablePosLight();
		break;
	case 'z':
		mScene->enableSpotLight();
		break;
	case 'x':
		mScene->disableSpotLight();
		break;
	case 'e':
		mScene->enableDirLight();
		mScene->enablePosLight();
		mScene->enableSpotLight();
		break;
	case 'r':
		mScene->disableAllLights();
		break;
	case 'y':
		mScene->orbita();
		break;
	case 'b':
		mScene->rota();
		break;
	case 't':
		mScene->TIEsLightsOn();
		break;
	case 'g':
		mScene->TIEsLightsOff();
		break;
	default:
		need_redisplay = false;
		break;
	} //switch

	if (need_redisplay) {
		glutPostRedisplay(); // marks the window as needing to be redisplayed -> calls to display()
	}
}
//-------------------------------------------------------------------------

void IG1App::specialKey(int key, int x, int y)
{
	bool need_redisplay = true;
	int mdf = glutGetModifiers(); // returns the modifiers (Shift, Ctrl, Alt)

	switch (key) {
	case GLUT_KEY_RIGHT:
		if (m2Vistas && mCoord.x > mWinW / 2) {
		if (mdf == GLUT_ACTIVE_CTRL)
				mCameraB->moveLR(-5);
			else
				mCameraB->orbit(5, 0); 
		}
		else {
			if (mdf == GLUT_ACTIVE_CTRL)
				mCamera->moveLR(-5);
			else
				mCamera->orbit(5, 0);   // rotates -1 on the X axis
		}
		break;
	case GLUT_KEY_LEFT:
		if (m2Vistas && mCoord.x > mWinW / 2) {
			if (mdf == GLUT_ACTIVE_CTRL)
				mCameraB->moveLR(-5);      // rotates 1 on the Y axis 
			else
				mCameraB->moveLR(5);     // rotate -1 on the Y axis 
		
		}
		else {
			if (mdf == GLUT_ACTIVE_CTRL)
				mCamera->moveLR(-5);      // rotates 1 on the Y axis 
			else
				mCamera->moveLR(5);     // rotate -1 on the Y axis 
		}
		break;
	case GLUT_KEY_UP:
		if (m2Vistas && mCoord.x > mWinW / 2) {
			mCameraB->moveUD(5);
		}
		else{
			mCamera->moveUD(5);    // rotates 1 on the Z axis
		}
		break;
	case GLUT_KEY_DOWN:

	if (m2Vistas && mCoord.x > mWinW / 2) {
			mCameraB->moveUD(-5);
		}
		else{
			mCamera->moveUD(-5);    // rotates 1 on the Z axis
		}	break;
	default:
		need_redisplay = false;
		break;
	}//switch

	if (need_redisplay) {
		glutPostRedisplay(); // marks the window as needing to be redisplayed -> calls to display()
	}
}
//-------------------------------------------------------------------------

void IG1App::update()
{
	mLastUpdateTime += glutGet(GLUT_ELAPSED_TIME);

	if (mLastUpdateTime % 30000 == 0 && mScene->getmID()==0)
	{
		if (m2Vistas && mCoord.x >= mWinW/2) {
			mSceneB->update();
		}
		else {
			mScene->update();
		}

		glutPostRedisplay();
		mLastUpdateTime = 0;
	}
}

void IG1App::mouse(int button, int state, int x, int y)
{
	mBot = button;
	mCoord.x = x;
	mCoord.y = (mWinH - y);
}

void IG1App::motion(int x, int y)
{
	glm::dvec2 aux = mCoord;
	mCoord.x = x;
	mCoord.y = (mWinH - y);
	glm::dvec2 c;
	c = aux - mCoord;

	if (mBot == GLUT_LEFT_BUTTON) {
		if (m2Vistas && mCoord.x > mWinW / 2) {
		mCameraB->orbit(c.x*0.08, c.y);
		}
		else{
		mCamera->orbit(c.x*0.08, c.y);
		}
	}
	else if (mBot == GLUT_RIGHT_BUTTON) {
		if (m2Vistas && mCoord.x > mWinW / 2) {
		mCameraB->moveLR(c.x);
		mCameraB->moveUD(c.y);
		}
		else{
		mCamera->moveLR(c.x);
		mCamera->moveUD(c.y);
		}
	}
	glutPostRedisplay();
}

void IG1App::mouseWheel(int n, int d, int x, int y)
{
	int m = glutGetModifiers();
	if(m==GLUT_ACTIVE_CTRL){
	
		if (m2Vistas && mCoord.x > mWinW / 2) {
			if (d == 1) {
				mCameraB->moveFB(10);
			}
			else {
				mCameraB->moveFB(-10);
			}
		}
		else {
			if (d == 1) {
				mCamera->moveFB(10);
			}
			else {
				mCamera->moveFB(-10);
			}
		}
	}
	else
	{
		if (m2Vistas && mCoord.x > mWinW / 2) {
			mCameraB->setScale(0.5 * d);	
		}
		else {
			mCamera->setScale(0.5 * d);
		}

	}
	
	glutPostRedisplay();
}

void IG1App::s_mouse(int button, int state, int x, int y)
{
	s_ig1app.mouse(button, state, x, y);
}

void IG1App::s_motion(int x, int y)
{
	s_ig1app.motion(x, y);
}

void IG1App::s_mouseWheel(int n, int d, int x, int y)
{
	s_ig1app.mouseWheel(n, d, x, y);
}


void IG1App::display2Vistas() const{
	
	//"partimos" el puerto
	mViewPort->setSize(mWinW / 2, mWinH);
	mViewPortB->setSize(mWinW / 2, mWinH);

	//"partimos" la camara	
	mCamera->setSize(mViewPort->width(), mViewPort->height());
	mCameraB->setSize(mViewPortB->width(), mViewPortB->height());
	
	//cambiamos la posicion para que quepan ambos
	mViewPort->setPos(0, 0);
	mViewPortB->setPos(mWinW/2, 0);
	
	//renderizamos 
	mSceneB->render(*mCameraB);
	mScene->render(*mCamera); // uploads the viewport and camera to the GPU

	//restauramos las cosas por defecto
	mViewPort->setSize(mWinW, mWinH);
	mViewPortB->setSize(0, 0);
	mCamera->setSize(mViewPort->width(), mViewPort->height());
	mCameraB->setSize(mViewPortB->width(), mViewPortB->height());
	mViewPort->setPos(0, 0);
	mViewPortB->setPos(0, 0);
}