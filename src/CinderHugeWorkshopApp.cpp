#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/params/Params.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CinderHugeWorkshopApp : public AppNative {
  public:
	void setup();
    void resize();
	void update();
	void draw();
    
    Quatf                   mRotation;
    Matrix44f               mCubeRotation;
    CameraPersp             mCam;
	Vec3f					mLightDirection;
    ColorAf                 mColor;
	params::InterfaceGlRef	mParams;
    float                   mSize;
    bool                    mWireframe, mFullscreen;
    
};

void CinderHugeWorkshopApp::setup()
{
    // camera
    mCam.lookAt( Vec3f( 3, 2, -4 ), Vec3f::zero() );
    
    mCubeRotation.setToIdentity();
    
    // set the light direction
	mLightDirection = Vec3f( 0, 0, -1 );
    
    // add a default color
    mColor = ColorA(1,0,1);
    
    // Cube size
    mSize = 2.0f;
    
    // draw wireframe?
    mWireframe = false;
    
    // fullscreen?
    mFullscreen = false;
    
    // set up interface
    mParams = params::InterfaceGl::create( getWindow(), "App parameters", toPixels( Vec2i( 200, 200 ) ) );
    mParams->addParam( "Color", &mColor );
    mParams->addParam( "Cube Rotation", &mRotation );
    mParams->addParam( "Size", &mSize ).min(0.1f).max(20.0f).step(0.01f);
    mParams->addParam( "Draw Wireframe", &mWireframe );
    mParams->addParam( "Fullscreen", &mFullscreen );
}

void CinderHugeWorkshopApp::resize()
{
    // now tell our Camera that the window aspect ratio has changed
    mCam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
    
    // and in turn, let OpenGL know we have a new camera
    gl::setMatrices( mCam );
}

void CinderHugeWorkshopApp::update()
{
    mCubeRotation.rotate( Vec3f( 1, 1, 1 ), toRadians(0.2f) );
    
    if (isFullScreen() != mFullscreen)
    {
        app::setFullScreen(mFullscreen);
    }
    
}

void CinderHugeWorkshopApp::draw()
{
    gl::enableDepthRead();
    gl::enableDepthWrite();
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    gl::color(mColor);
    
    glLoadIdentity();
    // enable the lighting
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    // the light position
    GLfloat lightPosition[] = { -mLightDirection.x, -mLightDirection.y, -mLightDirection.z, 0 };
    glLightfv( GL_LIGHT0, GL_POSITION, lightPosition );
    // the material
    glMaterialfv( GL_FRONT, GL_DIFFUSE,	mColor );
    
    gl::setMatrices( mCam );
    
    Vec3f size(mSize, mSize, mSize);
    
    glPushMatrix();
        if (mWireframe) gl::enableWireframe();
        gl::rotate(mRotation);
        gl::multModelView( mCubeRotation );
        gl::drawCube( Vec3f::zero(), size );
        gl::disableWireframe();
    glPopMatrix();
    
    // draw interface
    mParams->draw();
    
}

CINDER_APP_NATIVE( CinderHugeWorkshopApp, RendererGl )
