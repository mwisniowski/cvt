#include <RGBDVOApp.h>

#include <cvt/gui/Application.h>
#include <cvt/util/EigenBridge.h>
#include <cvt/util/Delegate.h>
#include <cvt/util/Time.h>

namespace cvt
{
    RGBDVOApp::RGBDVOApp( const String& folder, const Matrix3f& K, const VOParams& params ) :
        _parser( folder, 0.02f ),
        _vo( K, params ),
        _cumulativeAlignmentSpeed( 0.0f ),
        _numAlignments( 0 ),
        _mainWindow( "RGBD-VO" ),
        _kfMov( &_keyframeImage ),
        _imageMov( &_currentImage ),
        _poseMov( &_poseView ),
        _nextButton( "next" ),
        _nextPressed( false ),
        _stepButton( "toggle stepping" ),
        _step( false ),
        _optimizeButton( "optimize" ),
        _optimize( true )
    {
        _timerId = Application::registerTimer( 10, this );
        setupGui();

        // observe the vo changes:
        Delegate<void ( const Matrix4f& )> kfAddDel( this, &RGBDVOApp::keyframeAddedCallback );
        Delegate<void ()> actkfChgDel( this, &RGBDVOApp::activeKeyframeChangedCallback );
        _vo.keyframeAdded.add( kfAddDel );
        _vo.activeKeyframeChanged.add( actkfChgDel );

        _parser.loadNext();
        while( _parser.data().poseValid == false )
            _parser.loadNext();

        Image gray, smoothed, depth;
        _parser.data().rgb.convert( gray, IFormat::GRAY_FLOAT );
        _parser.data().depth.convert( depth, IFormat::GRAY_FLOAT );

        smoothed.reallocate( gray.width(), gray.height(), IFormat::GRAY_FLOAT );
        gray.convolve( smoothed, IKernel::GAUSS_HORIZONTAL_3, IKernel::GAUSS_VERTICAL_3 );

        _vo.addNewKeyframe( smoothed, depth, _parser.data().pose );

        _avgTransError.setZero();
        _validPoseCounter = 0;

        _fileOut.open( "trajectory.txt" );
    }

    RGBDVOApp::~RGBDVOApp()
    {
        Application::unregisterTimer( _timerId );
        _fileOut.close();
    }

    void RGBDVOApp::onTimeout()
    {        
        if( _nextPressed || !_step ){
            if( !_parser.hasNext() ){
                _avgTransError /= _validPoseCounter;
                std::cout << "Mean Translational Error: " << _avgTransError << " Len: " << _avgTransError.length() << std::endl;
                Application::exit();
            }
            _parser.loadNext();
            _currentImage.setImage( _parser.data().rgb );
            _nextPressed = false;
        }

        if( _optimize ){
            Time t;
            const RGBDParser::RGBDSample& d = _parser.data();
            // try to align:
            Image gray( d.rgb.width(), d.rgb.height(), IFormat::GRAY_FLOAT );
            d.rgb.convert( gray );

            Image smoothed( gray.width(), gray.height(), IFormat::GRAY_FLOAT );
            gray.convolve( smoothed, IKernel::GAUSS_HORIZONTAL_3, IKernel::GAUSS_VERTICAL_3 );

            _vo.updatePose( smoothed, d.depth );
            _cumulativeAlignmentSpeed += t.elapsedMilliSeconds();
            _numAlignments++;
            String title;
            title.sprintf( "RGBDVO: Avg. Speed %0.1f ms", _cumulativeAlignmentSpeed / _numAlignments );
            _mainWindow.setTitle( title );


            // update the absolute pose
            Matrix4f absPose;
            _vo.pose( absPose );

            if( d.poseValid ){
                _avgTransError.x += Math::abs( absPose[ 0 ][ 3 ] - d.pose[ 0 ][ 3 ] );
                _avgTransError.y += Math::abs( absPose[ 1 ][ 3 ] - d.pose[ 1 ][ 3 ] );
                _avgTransError.z += Math::abs( absPose[ 2 ][ 3 ] - d.pose[ 2 ][ 3 ] );
                _validPoseCounter++;
            }

            writePose( absPose, d.stamp );

            _poseView.setCamPose( absPose );
            _poseView.setGTPose( d.pose );
        }
    }    

    void RGBDVOApp::writePose( const Matrix4f& pose, double stamp )
    {
        Quaternionf q( pose.toMatrix3() );

        _fileOut.precision( 15 );
        _fileOut << std::fixed << stamp << " "
                 << pose[ 0 ][ 3 ] << " "
                 << pose[ 1 ][ 3 ] << " "
                 << pose[ 2 ][ 3 ] << " "
                 << q.x << " "
                 << q.y << " "
                 << q.z << " "
                 << q.w << std::endl;

    }

    void RGBDVOApp::setupGui()
    {
        _mainWindow.setSize( 800, 600 );

        _mainWindow.addWidget( &_kfMov );
        _mainWindow.addWidget( &_imageMov );

        // add widgets as necessary
        _kfMov.setSize( 300, 200 );
        _kfMov.setTitle( "Current Keyframe" );

        _imageMov.setSize( 300, 200 );
        _imageMov.setPosition( 300, 0 );
        _imageMov.setTitle( "Current Image" );

        _mainWindow.addWidget( &_poseMov );
        _poseMov.setSize( 300, 200 );
        _poseMov.setPosition( 600, 200 );
        _poseMov.setTitle( "Poses" );

        WidgetLayout wl;

        /*
        wl.setRelativeLeftRight( 0.02f, 0.98f);
        wl.setRelativeTopBottom( 0.02f, 0.98f);
        _mainWindow.addWidget( &_sceneView, wl );
        */

        wl.setAnchoredBottom( 5, 30 );
        wl.setAnchoredRight( 5, 70 );
        _mainWindow.addWidget( &_nextButton, wl );
        Delegate<void ()> d( this, &RGBDVOApp::nextPressed );
        _nextButton.clicked.add( d );

        wl.setAnchoredBottom( 40, 30 );
        _mainWindow.addWidget( &_stepButton, wl );
        Delegate<void ()> d1( this, &RGBDVOApp::toggleStepping );
        _stepButton.clicked.add( d1 );

        wl.setAnchoredBottom( 75, 30 );
        _mainWindow.addWidget( &_optimizeButton, wl );
        Delegate<void ()> d2( this, &RGBDVOApp::optimizePressed );
        _optimizeButton.clicked.add( d2 );

        _mainWindow.setVisible( true );
    }

    void RGBDVOApp::nextPressed()
    {
        std::cout << "click" << std::endl;
        _nextPressed = true;
    }

    void RGBDVOApp::optimizePressed()
    {
        _optimize = !_optimize;
    }

    void RGBDVOApp::toggleStepping()
    {
        _step = !_step;
    }

    void RGBDVOApp::keyframeAddedCallback( const Matrix4f& pose )
    {
        _poseView.addKeyframe( pose );
    }

    void RGBDVOApp::activeKeyframeChangedCallback()
    {
        // TODO: draw the active keyframe in green in the view
    }

}
