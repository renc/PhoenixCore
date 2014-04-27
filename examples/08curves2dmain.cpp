#include <vector>
#include <assert.h> 
#include "Banana.h"

using namespace banana;

// we are going to build a bezier curve and use the polyline along the curve to view it.  
// the control points of the curves.
std::vector<Vector2d> aCPs;
// the hull of the control points
Polyline polyline;
// the polyline along the actually curve. 
Polyline curvePolyline; 

// we are going to add some manipulation to the curve.
// First By press the key 0 1 2 3, we select a control point (index 0, 1, 2, 3).
int g_iSelectedCP = -1;
// Second, press Up/Down/Left/Right to move the control point. 
   
Vector2d evaluateCubicBezier_deCasteljau(const std::vector<Vector2d> &aCPs, float t)
{
	// this function is very interesting. everytime the line sections reduce, recursively, 
	// when it redecued to one, we do a linear interopolation of it and get the result.
    // 
	
	
	if (!aCPs.size()) 
	{
		assert(false);
		return Vector2d();
	}
	if (aCPs.size() == 1)
		return aCPs[0]; 
	
	std::vector<Vector2d> a;
	for (int i = 0; i < aCPs.size() - 1; ++i)
		a.push_back( aCPs[i] + (aCPs[i+1] - aCPs[i]) * t );
	assert(a.size() == aCPs.size() - 1);
	return evaluateCubicBezier_deCasteljau(a, t); 
} 

void calculatePolylineFromCP(const std::vector<Vector2d> &aCPs)
{
	// build a polyline from our control points, for visible. 
	polyline.clear();
	polyline.setPosition(Vector2d(400, 400) );
	for (int i = 0; i < aCPs.size(); ++i)
		polyline.addVertex( aCPs[i] );

	// to do the evaluation of the curve, get an array pos on the curve
	std::vector<Vector2d> aPos;
	for (int i = 0; i <= 200; ++i)
		aPos.push_back( evaluateCubicBezier_deCasteljau(aCPs, i * 1.0f / 200) );
	// we use those pos on the curve to simulate a curve by a polyline, for visible.
	curvePolyline.clear(); 
	curvePolyline.setPosition(Vector2d(400, 400) );
	for (int i = 0; i < aPos.size(); ++i)
		curvePolyline.addVertex( aPos[i] ); 
}

// slot/callback to update the control point of the curve,
// and refresh the polyline which used to view the curve.
void onKeyboardEvent(const WindowEvent &e)
{
	if (e.eType != WET_KEY)
		return;

	switch (e.int_data)
	{					  
	case banana::Key_0: g_iSelectedCP = 0; break;
	case banana::Key_1: g_iSelectedCP = 1; break;
	case banana::Key_2: g_iSelectedCP = 2; break;
	case banana::Key_3: g_iSelectedCP = 3; break;
	case banana::Key_UP: 
		{
			if (g_iSelectedCP < aCPs.size() && -1 < g_iSelectedCP) {
				aCPs[g_iSelectedCP].setY( aCPs[g_iSelectedCP].getY() + 1 ); 
				calculatePolylineFromCP(aCPs);
			}
		} break; 
	case banana::Key_DOWN: 
		{
			if (g_iSelectedCP < aCPs.size() && -1 < g_iSelectedCP) {
				aCPs[g_iSelectedCP].setY( aCPs[g_iSelectedCP].getY() - 1 );
				calculatePolylineFromCP(aCPs);
			}
		} break; 
	case banana::Key_LEFT: 
		{
			if (g_iSelectedCP < aCPs.size() && -1 < g_iSelectedCP) {
				aCPs[g_iSelectedCP].setX( aCPs[g_iSelectedCP].getX() - 1 );
				calculatePolylineFromCP(aCPs);
			}
		} break; 
	case banana::Key_RIGHT: 
		{
			if (g_iSelectedCP < aCPs.size() && -1 < g_iSelectedCP) {
				aCPs[g_iSelectedCP].setX( aCPs[g_iSelectedCP].getX() + 1 );
				calculatePolylineFromCP(aCPs);
			}
		} break; 
	default:
		{
			int a = 0;
			int b = a;
			++b;
		}
		break;
	} 
} 

int main()
{
	// define 4 control points for a cubic bezier curve (degree 3, order 4).
	aCPs.clear();
	aCPs.push_back( Vector2d(-100, 0) );  
	aCPs.push_back( Vector2d(   0, 100) );
	aCPs.push_back( Vector2d( 100, 100) );
	aCPs.push_back( Vector2d( 200, 0) );
	calculatePolylineFromCP(aCPs);

	// to initialize a render system, which will open a glfw window. 
	RenderSystem system(Vector2d(800, 600), false, false);
	// to add a callback function / slot to respond to the key board event.
	WindowManager::Instance()->addListener(&onKeyboardEvent);

    //! The Loop.
    while( system.run() )
    {	
		for (int i = 0; i < 8; ++i)
		{
			// - 
			system.drawLine( Vector2d( 0,100 * i ), Vector2d( 800, 100 * i ), Color(100, 100, 100), Color(100, 100, 100) );
			// | 
			system.drawLine( Vector2d( 100 * i,0 ), Vector2d( 100 * i, 600 ), Color(50, 50, 50), Color(50, 50, 50) );
		}

		// 
		BatchGeometryPtr pPolylineBatchGeometry = system.drawPolyline( polyline, banana::Color::redColor() ); 
		BatchGeometryPtr pCurveBatchGeometry	= system.drawPolyline( curvePolyline, banana::Color::greenColor() ); 
    }

    return 0;

}

/******************************************************
References:
+ http://www.rhino3d.com/nurbs/

******************************************************/