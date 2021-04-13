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
		mCamera->setScale(+0.05);  // zoom in  (increases the scale)
		break;
	case '-':
		mCamera->setScale(-0.05);  // zoom out (decreases the scale)
		break;
	case 'l':
		mCamera->set3D();
		break;
	case 'o':
		mCamera->set2D();
		break;
	case 'U':
		mScene->setUpdate();
		break;
	case 'u':
		mScene->updateTriangle();
		break;
	case 'p':
		mCamera->changePrj();
		break;
	case '0':
		mCamera->set2D();
		mScene->changeScene(0);
		break;
	case '1':
		mCamera->set3D();
		mScene->changeScene(1);
		break;
	case'f':
	case 'F':
		mScene->savePhoto();
		break;
	case 'k':
		m2Vistas = !m2Vistas;

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
		if (mdf == GLUT_ACTIVE_CTRL)
			mCamera->moveLR(-5); 
		else
			mCamera->orbit(5,0);   // rotates -1 on the X axis
		break;
	case GLUT_KEY_LEFT:
		if (mdf == GLUT_ACTIVE_CTRL)
			mCamera->moveLR(-5);      // rotates 1 on the Y axis 
		else
			mCamera->moveLR(5);     // rotate -1 on the Y axis 
		break;
	case GLUT_KEY_UP:
		mCamera->moveUD(5);    // rotates 1 on the Z axis
		break;
	case GLUT_KEY_DOWN:
		mCamera->moveUD(-5);   // rotates -1 on the Z axis
		break;
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

		mScene->update();
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
		mCamera->orbit(c.x*0.08, c.y);
	}
	else if (mBot == GLUT_RIGHT_BUTTON) {
		mCamera->moveLR(c.x);
		mCamera->moveUD(c.y);
	}
	glutPostRedisplay();
}

void IG1App::mouseWheel(int n, int d, int x, int y)
{
	int m = glutGetModifiers();
	if(m==GLUT_ACTIVE_CTRL){
		if(d==1){
		mCamera->moveFB(10);
		}
		else{
		mCamera->moveFB(-10);
		}
	}
	else
	{
		mCamera->setScale(0.5 * d);
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


void IG1App::display2Vistas() const{ ///TODO->Gesti�n independiente de eventos
	Camera aux = *mCamera;
	Viewport auxVP = *mViewPort;
	Scene* auxScene = new Scene();
	
	auxScene->changeScene(0); //dcha.
	mScene->changeScene(1); //izda.

	//"partimos" el puerto
	mViewPort->setSize(mWinW / 2, mWinH);
	//"partimos" la camara	
	aux.setSize(mViewPort->width(), mViewPort->height());

	//cambiamos la posicion para que quepan ambos
	mViewPort->setPos(0, 0);
	//renderizamos 
	mScene->render(aux);  // uploads the viewport and camera to the GPU
	
	mViewPort->setPos(mWinW/2, 0);
	aux.setCenital();
	auxScene->render(aux);  // uploads the viewport and camera to the GPU

	*mViewPort = auxVP;
	delete auxScene;
}