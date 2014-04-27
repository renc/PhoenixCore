/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#ifndef __PHGLFW3WINDOWMANAGER_H__
#define __PHGLFW3WINDOWMANAGER_H__


#include "config.h"
#if PH_USE_GLFW3

#include <boost/function.hpp>
#include "WindowManager.h"
#include "EventReceiver.h"
#include "Vector2d.h"

namespace phoenix
{

//! GLFW Window Manager Implementation
/*!
	This class implements the GLFW window manager and provides translations
	for Events.
*/
class GLFW3WindowManager
	: public WindowManager
{
public:

	//! Instance.
	static boost::shared_ptr<WindowManager> Instance( ){
		if( !instance ){
			instance = WindowManagerPtr( new GLFW3WindowManager );
		}
		return instance;
	}

	//! Destructor
	virtual ~GLFW3WindowManager(){
		glfwTerminate();
	}

	/*! Open Command.
		Creates a new window and initializes OpenGL. Also sets up callbacks for
		key events.
		\param _sz The size of the window.
		\param _f Fullscreen.
		\param _resize Whether or not the window is natively resizable at all.
	*/
	virtual bool open( const Vector2d& _sz = Vector2d( 640, 480 ), const bool _f = false, const bool _resize = true );

	/*! Close Command.
		Closes the open window (if any).
	*/
	inline virtual void close() { glfwDestroyWindow(m_pWindow); }

	//! Gets the culmative running time in seconds.
	virtual double getTime() {
		return glfwGetTime();
	}

	//! Set window caption.
    inline virtual void setWindowTitle(const std::string& _str) { glfwSetWindowTitle(m_pWindow, _str.c_str()); }

	//! Set window size
	inline virtual void setWindowSize( const Vector2d& _sz ) { 
		glfwSetWindowSize(m_pWindow, (unsigned int) _sz.getX(), (unsigned int) _sz.getY() ); 
		WindowManager::setWindowSize(_sz);
	}

	//! Iconify
	inline virtual void iconify() { glfwIconifyWindow(m_pWindow); }

	//! Restore
	inline virtual void restore() { glfwRestoreWindow(m_pWindow); }

    /*! Set Cursor Visibility.
        Can hide or show the mouse cursor. Hiding it has three effects: The mouse cursor is invisible, the mouse is confined to the screen,
        and coordinates are no longer limited to the window size. By default, the mouse is hidden in fullscreen, but otherwise visible.
    */
    inline virtual void setCursorVisible(const bool _v) { 
		if (_v)
		{
			glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else
		{
			glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
	}

	//! Swap Buffers and Update Events.
	virtual void update();

protected:

	GLFW3WindowManager()
		: WindowManager(), m_pWindow(0)
	{}


	//! Keyboard callback (from GLFW).
    static void glfwKeyboardCallback( GLFWwindow *, int, int, int, int );

	//! Character callback (from GLFW).
	static void glfwCharacterCallback( GLFWwindow *, unsigned int );
	
	static void glfwMouseButtonCallback( GLFWwindow *, int, int, int );
													
    //! Window Resize callback (from GLFW).
    static void glfwWindowResizeCallback(GLFWwindow*,int,int);		    
    //! Window callback (from GLFW).
    static void glfwWindowCloseCallback(GLFWwindow*);				     
    
#ifdef _GLFW_WM_MOVE_HACK
	//! Window move callback ( from GLFW ).
	static void glfwWindowMoveCallback( int state );
#endif //_GLFW_WM_MOVE_HACK

	GLFWwindow *m_pWindow;
};

}

#endif //PH_USE_GLFW3
#endif //__PHGLFW3WINDOWMANAGER_H__

