
#include "config.h"

#if PH_USE_GLFW3

#include "GLFW3WindowManager.h"

using namespace phoenix;

/*!
    Open function.
*/
bool GLFW3WindowManager::open( const Vector2d& _sz, const bool _f, const bool _resize )
{

    // Set our internal screen size variable.
    screensize = _sz;

	// No GLFW functions may be called before this
	if( !glfwInit() ) return false;

	// Important defines
	#define RED_BITS 8
	#define BLUE_BITS 8
	#define GREEN_BITS 8
	#define ALPHA_BITS 8
	#define STENCIL_BITS 0
	#ifdef DISABLE_DEPTHBUFFER
		#define DEPTH_BITS 0
	#else
		#define DEPTH_BITS 8
	#endif

	//int mode = _f ? GLFW_FULLSCREEN : GLFW_WINDOW;
	glfwWindowHint(GLFW_RESIZABLE, _resize ? GL_TRUE : GL_FALSE); //This function sets hints for the next call to glfwCreateWindow. 

	//if( !glfwOpenWindow(int(_sz.getX()), int(_sz.getY()), RED_BITS, BLUE_BITS, GREEN_BITS, ALPHA_BITS, DEPTH_BITS, STENCIL_BITS, mode) ) 
	//	return false;
	m_pWindow = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
	if (!m_pWindow)
	{
		glfwTerminate();
		return false;
	}

	// Before you can use the OpenGL API, it must have a current OpenGL context. 
	// You make a window's context current with glfwMakeContextCurrent. 
	// It will then remain as the current context until you make another context current or until the window owning it is destroyed.
	glfwMakeContextCurrent(m_pWindow);	// cjren, call this before glewInit() otherwise the later fail.

	// Disable vsync.
	glfwSwapInterval( 0 );

	// Set the window caption
	setWindowTitle( "Window title" );

	// Set GLFW event callbacks
	glfwSetKeyCallback			(m_pWindow, &GLFW3WindowManager::glfwKeyboardCallback		);
	glfwSetCharCallback			(m_pWindow,  &GLFW3WindowManager::glfwCharacterCallback		);
	glfwSetMouseButtonCallback  (m_pWindow,  &GLFW3WindowManager::glfwMouseButtonCallback	); 
    glfwSetWindowCloseCallback	(m_pWindow,  &GLFW3WindowManager::glfwWindowCloseCallback	);
	glfwSetWindowSizeCallback	(m_pWindow,  &GLFW3WindowManager::glfwWindowResizeCallback	);

#ifdef _GLFW_WM_MOVE_HACK
	glfwSetWindowMoveCallback( &GLFW3WindowManager::glfwWindowMoveCallback );
#endif

	// glfw3: The GLFW_KEY_REPEAT enable has been removed and key repeat is always enabled for both keys and characters.
	// Disable key repeat; event receiver manages that.
	//glfwDisable(GLFW_KEY_REPEAT);at glfw2, 

	return true;

}

/*
	Swap Buffers and Update Events.
*/
void GLFW3WindowManager::update() 
{
	// send the update event. ( done first because glfwSwapBuffers sends new events ).
	WindowEvent e;
	e.type = WET_UPDATE;
	signal(e);

	glfwSwapBuffers(m_pWindow); 
}

//! Window Resize callback (from GLFW).
void GLFW3WindowManager::glfwWindowResizeCallback(GLFWwindow*, int width, int height )
{
	WindowManagerPtr wm = WindowManager::Instance();

	Vector2d size( (float) width, (float) height );
	wm->setWindowSize( size );

	WindowEvent e;
	e.type = WET_RESIZE;
	e.vector_data = size;
	wm->signal(e);
}

//! Keyboard callback (from GLFW).
void GLFW3WindowManager::glfwKeyboardCallback( GLFWwindow *, int, int, int, int)
{
	//WindowEvent e;
	//e.type = WET_KEY;
	//e.int_data = key;
	//e.bool_data = action == GLFW_PRESS ? true : false;
	//Instance()->signal(e);
}

//! Character callback (from GLFW).
void GLFW3WindowManager::glfwCharacterCallback( GLFWwindow *, unsigned int )
{
		//WindowEvent e;
		//e.type = WET_CHAR;
		//e.int_data = key;
		//e.bool_data = action == GLFW_PRESS ? true : false;
		//Instance()->signal(e);
}

void GLFW3WindowManager::glfwMouseButtonCallback(GLFWwindow *, int, int, int)
{
}
			   

//! Window callback (from GLFW).
void GLFW3WindowManager::glfwWindowCloseCallback(GLFWwindow*)
{
	WindowEvent e;
	e.type = WET_CLOSE;
	e.bool_data = true;
	Instance()->signal(e);
}
			   
#ifdef _GLFW_WM_MOVE_HACK
void GLFW3WindowManager::glfwWindowMoveCallback( int state )
{
	WindowEvent e;
	e.type = WET_MOVE;
	e.bool_data = state == GLFW_MOVE_ENTER ? 1 : 0;
	Instance()->signal(e);
}

#endif //_GLFW_WM_MOVE_HACK
#endif //PH_USE_GLFW2