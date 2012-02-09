#ifndef CVT_STEREO_SLAM_H
#define CVT_STEREO_SLAM_H

#include <cvt/vision/CameraCalibration.h>
#include <cvt/vision/PointCorrespondences3d2d.h>
#include <cvt/gfx/ifilter/IWarp.h>
#include <cvt/gfx/GFXEngineImage.h>
#include <cvt/vision/Vision.h>
#include <cvt/vision/EPnP.h>
#include <cvt/vision/FeatureMatch.h>
#include <cvt/math/SE3.h>
#include <cvt/math/sac/RANSAC.h>
#include <cvt/math/sac/EPnPSAC.h>
#include <cvt/math/LevenbergMarquard.h>
#include <cvt/util/Signal.h>
#include <cvt/util/Time.h>
#include <cvt/util/Time.h>

#include <cvt/vision/slam/SlamMap.h>
#include <cvt/vision/slam/Keyframe.h>

#include <cvt/vision/slam/stereo/FeatureTracking.h>
#include <cvt/vision/slam/stereo/ORBStereoMatching.h>
#include <cvt/vision/slam/stereo/DescriptorDatabase.h>
#include <cvt/vision/slam/stereo/ORBTracking.h>
#include <cvt/vision/slam/stereo/MapOptimizer.h>
#include <cvt/vision/slam/stereo/FeatureAnalyzer.h>

#include <set>

namespace cvt
{
	// Managing class for stereo SLAM
	class StereoSLAM
	{
		public:
			StereoSLAM( FeatureTracking* ft, 
					    const CameraCalibration& c0, size_t w0, size_t h0,
						const CameraCalibration& c1, size_t w1, size_t h1 );

			// new round with two new images, maybe also hand in a pose prediction?
			void newImages( const Image& img0, const Image& img1 );

			const Image& undistorted( size_t idx = 0 ) const 
			{
				if( idx )
					return _undist1;
				return _undist0;
			}

			const SlamMap & map() const { return _map; }
			void clear();

			void setPose( const Matrix4d& pose );
			const SE3<double>& pose() const { return _pose; };

			Signal<const Image&>		newStereoView;	
			Signal<void>				keyframeAdded;	
			Signal<const SlamMap&>		mapChanged;	
			Signal<const Matrix4f&>		newCameraPose;	
			Signal<size_t>				numTrackedPoints;	

		private:
			/* camera calibration data and undistortion maps */
			CameraCalibration	_camCalib0;
			CameraCalibration	_camCalib1;
			Image				_undistortMap0;
			Image				_undistortMap1;

			/* undistorted images */
			Image				_undist0;
			Image				_undist1;


			// FeatureTracker (single view)
			float				_trackingSearchRadius;
			FeatureTracking*	_featureTracking;

			// minimum needed features before new keyframe is added
			size_t				_minTrackedFeatures;


			/* the current pose of the camera rig */
			SE3<double>			_pose;

			/* the active Keyframe Id (closest to _pose) */
			int					_activeKF;
			double				_minKeyframeDistance;
			double				_maxKeyframeDistance;

			SlamMap				_map;

			MapOptimizer		_bundler;
			Image				_lastImage;

			void estimateCameraPose( const PointSet3d & p3d, const PointSet2d & p2d );

			void debugPatchWorkImage( const std::set<size_t> & indices,
								      const std::vector<size_t> & featureIds,
								      const std::vector<FeatureMatch> & matches );

			bool newKeyframeNeeded( size_t numTrackedFeatures ) const;

			void createDebugImageMono( Image & debugImage, const PointSet2d & tracked ) const;
			void createDebugImageStereo( Image & debugImage, 
										 const std::vector<FeatureMatch> & matches,
										 const std::vector<size_t> & indices ) const;
	};

}

#endif
