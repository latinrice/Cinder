#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;

typedef std::vector<vec2> MyLine;
typedef std::pair<std::vector<vec2>, Color> MyCurve;

// We'll create a new Cinder Application by deriving from the App class.
class BasicApp : public App {
  public:
	// Cinder will call 'mouseDrag' when the user moves the mouse while holding one of its buttons.
	// See also: mouseMove, mouseDown, mouseUp and mouseWheel.
	void mouseDrag(MouseEvent event) override;

	void mouseDown( MouseEvent event ) override;

	void mouseUp(MouseEvent event) override;

	// Cinder will call 'keyDown' when the user presses a key on the keyboard.
	// See also: keyUp.
	void keyDown( KeyEvent event ) override;

	// Cinder will call 'draw' each time the contents of the window need to be redrawn.
	void draw() override;

  private:
	// This will maintain a list of points which we will draw line segments between
	//std::vector<vec2> mPoints;
	//std::vector<MyLine> mLines;
	MyLine mCurrentLine;
	std::vector<MyCurve> mCurves;
	MyCurve mCurrentCurve;
	Color mCurrentPencilColor = Color(1.0f, 0.0f, 0.0f);
	Rectf pallete = Rectf(0, 0, 300, 100);
	Rectf redColor = Rectf(20, 20, 80, 80);
	Rectf greenColor = Rectf(120, 20, 180, 80);
	Rectf blueColor = Rectf(220, 20, 280, 80);
};

void prepareSettings( BasicApp::Settings* settings )
{
	settings->setMultiTouchEnabled( false );
}

void BasicApp::mouseDrag(MouseEvent event)
{
	if ((pallete.getX1() <= event.getX()) && (pallete.getY1() <= event.getY()) && (pallete.getX2() >= event.getX()) && (pallete.getY2() >= event.getY()))
		return;
	mCurrentCurve.second = mCurrentPencilColor;
	mCurrentCurve.first.push_back(event.getPos());
}

//void BasicApp::mouseDown( MouseEvent event )
//{
//	// Store the current mouse position in the list.
//	//mPoints.push_back( event.getPos() );
//	if (mCurrentLine.empty())
//	{
//		mCurrentLine.push_back(event.getPos());
//	}
//	else
//	{
//		mCurrentLine.push_back(event.getPos());
//		mLines.push_back(mCurrentLine);
//		mCurrentLine.clear();
//	}
//}

void BasicApp::mouseDown(MouseEvent event)
{
	if ((pallete.getX1() <= event.getX()) && (pallete.getY1() <= event.getY()) && (pallete.getX2() >= event.getX()) && (pallete.getY2() >= event.getY()))
	{
		if ((redColor.getX1() <= event.getX()) && (redColor.getY1() <= event.getY()) && (redColor.getX2() >= event.getX()) && (redColor.getY2() >= event.getX()))
		{
			mCurrentPencilColor = Color(1.0f, 0.0f, 0.0f);
		}
		else if ((greenColor.getX1() <= event.getX()) && (greenColor.getY1() <= event.getY()) && (greenColor.getX2() >= event.getX()) && (greenColor.getY2() >= event.getY()))
		{
			mCurrentPencilColor = Color(0.0f, 1.0f, 0.0f);
		}
		else if ((blueColor.getX1() <= event.getX()) && (blueColor.getY1() <= event.getY()) && (blueColor.getX2() >= event.getX()) && (blueColor.getY2() >= event.getY()))
		{
			mCurrentPencilColor = Color(0.0f, 0.0f, 1.0f);
		}
	}
}

void BasicApp::mouseUp(MouseEvent event)
{
	mCurves.push_back(mCurrentCurve);
	mCurrentCurve.first.clear();
}

void BasicApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'f' ) {
		// Toggle full screen when the user presses the 'f' key.
		setFullScreen( ! isFullScreen() );
	}
	else if( event.getCode() == KeyEvent::KEY_SPACE ) {
		// Clear the list of points when the user presses the space bar.
		//mPoints.clear();
		//mLines.clear();
		mCurves.clear();
	}
	else if( event.getCode() == KeyEvent::KEY_ESCAPE ) {
		// Exit full screen, or quit the application, when the user presses the ESC key.
		if( isFullScreen() )
			setFullScreen( false );
		else
			quit();
	}
}

void drawCurve(MyCurve curve)
{
	gl::begin(GL_LINE_STRIP);
	gl::color(curve.second);
	std::for_each(curve.first.begin(), curve.first.end(), static_cast<void(*)(const vec2&)>(gl::vertex));
	gl::end();
}

void drawPallete(Rectf pallete, Rectf r, Rectf g, Rectf b, Color curCol)
{
	gl::color(0.8f, 0.8f, 0.8f);
	gl::drawSolidRect(pallete);

	gl::color(1.0f, 0.0f, 0.0f);
	gl::drawSolidRect(r);

	gl::color(0.0f, 1.0f, 0.0f);
	gl::drawSolidRect(g);

	gl::color(0.0f, 0.0f, 1.0f);
	gl::drawSolidRect(b);

	gl::color(curCol);
	gl::drawSolidRect(Rectf(0, 0, 10, 10));
}

void BasicApp::draw()
{
	// Clear the contents of the window. This call will clear
	// both the color and depth buffers. 
	gl::clear( Color::white() );

	// Set the current draw color to orange by setting values for
	// red, green and blue directly. Values range from 0 to 1.
	// See also: gl::ScopedColor

	// We're going to draw a line through all the points in the list
	// using a few convenience functions: 'begin' will tell OpenGL to
	// start constructing a line strip, 'vertex' will add a point to the
	// line strip and 'end' will execute the draw calls on the GPU.
	/*gl::begin( GL_LINE_STRIP );
	std::for_each(mPoints.begin(), mPoints.end(), static_cast<void(*)(const vec2&)>(gl::vertex));
	gl::end();*/
	//std::for_each(mLines.begin(), mLines.end(), [](MyLine line) { gl::drawLine(line[0], line[1]); });
	std::for_each(mCurves.begin(), mCurves.end(), drawCurve);
	drawCurve(mCurrentCurve);

	drawPallete(pallete, redColor, greenColor, blueColor, mCurrentPencilColor);
}

// This line tells Cinder to actually create and run the application.
CINDER_APP( BasicApp, RendererGl, prepareSettings )
