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
    void mouseDown( MouseEvent event );
    void resize();
	void update();
	void draw();
    
    Matrix44f               mCubeRotation;
    CameraPersp             mCam;
    Vec3f                   mLightDirection;
    ColorAf                 mColor;
    params::InterfaceGlRef  mParams;
    float                   mCubeSize;
    bool                    mFullscreen;
    Quatf                   mRotation;
    
};

void CinderHugeWorkshopApp::setup()
{
    Vec3f eyePoint(200.0f, 300.0f, -400.0f);
    Vec3f target = Vec3f::zero();
    
    mCam.lookAt(eyePoint, target);
    mCubeRotation.setToIdentity();
    
    mLightDirection = Vec3f(100,100,-400.0f);
    
    mColor = ColorA(1.0f, 0.0f, 1.0f);
    
    mCubeSize = 200.0f;
    
    // draw fullscreen?
    mFullscreen = false;
    
    mParams = params::InterfaceGl::create(getWindow(), "App Params", toPixels( Vec2f( 200, 200 )));
    mParams->addParam("Cube Color", &mColor);
    mParams->addParam("Rotation", &mRotation);
    mParams->addParam("Size", &mCubeSize).min(20.0f).max(500.0f).precision(0.5f);
    mParams->addParam("Draw Fullscreen", &mFullscreen);
}

void CinderHugeWorkshopApp::mouseDown(MouseEvent event)
{
}

void CinderHugeWorkshopApp::resize()
{
    mCam.setPerspective( 60 , getWindowAspectRatio(), 1, 1000);
    gl::setMatrices( mCam );
}

void CinderHugeWorkshopApp::update()
{
    if (isFullScreen() != mFullscreen) {
        setFullScreen(mFullscreen);
    }
    
    mCubeRotation.rotate( Vec3f( 1, 1, 1 ), toRadians(0.2f) );
}

void CinderHugeWorkshopApp::draw()
{
    gl::enableDepthRead();
    gl::enableDepthWrite();
    
    // clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
//    Vec2i pos = getMousePos();
//    
//    mColor = ColorAf(1.0f, lmap<float>(pos.x, 0.0f, (float)getWindowWidth(), 0.0f, 1.0f), 0.0f);
    gl::color(ColorA::white());
    
    glLoadIdentity();
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    GLfloat lightPosition[] = {
        -mLightDirection.x,
        -mLightDirection.y,
        -mLightDirection.z,
        0
    };
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mColor);
    
    gl::setMatrices(mCam);
    
    Vec3f size(mCubeSize, mCubeSize, mCubeSize);
    
    glPushMatrix();
        gl::rotate(mRotation);
        gl::multModelView( mCubeRotation );
        gl::drawCube( Vec3f::zero(), size );
    glPopMatrix();
    
    // draw the params
    mParams->draw();
}

CINDER_APP_NATIVE( CinderHugeWorkshopApp, RendererGl )
