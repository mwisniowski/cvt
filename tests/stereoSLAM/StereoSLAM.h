#ifndef CVT_STEREO_SLAM_H
#define CVT_STEREO_SLAM_H

#include <cvt/vision/CameraCalibration.h>
#include <cvt/vision/PointCorrespondences3d2d.h>
#include <cvt/gfx/ifilter/IWarp.h>
#include <cvt/vision/Vision.h>
#include <cvt/vision/ORB.h>
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

#include "ORBStereoMatching.h"
#include "DescriptorDatabase.h"
#include "FeatureTracking.h"
#include "MapOptimizer.h"
#include "FeatureAnalyzer.h"

#include <set>

namespace cvt
{
	// Managing class for stereo SLAM
	class StereoSLAM
	{
		public:

			struct ORBData
			{
				ORB* orb0;
				Image* img0;
				ORB* orb1;
				Image* img1;
				std::vector<FeatureMatch>* matches;
			};

			StereoSLAM( const CameraCalibration& c0,
						size_t w0, size_t h0,
						const CameraCalibration& c1,
						size_t w1, size_t h1 );

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

			Signal<const ORBData*>		newORBData;	
			Signal<const Keyframe&>		newKeyFrame;	
			Signal<const SlamMap&>		mapChanged;	
			Signal<const PointSet2d&>	trackedPoints;	
			Signal<const Matrix4f&>		newCameraPose;	

		private:
			/* camera calibration data and undistortion maps */
			CameraCalibration	_camCalib0;
			CameraCalibration	_camCalib1;
			Image				_undistortMap0;
			Image				_undistortMap1;

			/* undistorted images */
			Image				_undist0;
			Image				_undist1;

			/* descriptor matching parameters */
			float				_matcherMaxLineDistance;	
			float				_matcherMaxDescriptorDist;
			ORBStereoMatching	_stereoMatcher;
			float				_maxTriangReprojError;

			/* orb parameters */
			size_t				_orbOctaves;
			float				_orbScaleFactor;
			uint8_t				_orbCornerThreshold;
			size_t				_orbMaxFeatures;
			bool				_orbNonMaxSuppression;

			// FeatureTracker (single view)
			DescriptorDatabase<ORBFeature>	_descriptorDatabase;
			float				_trackingSearchRadius;
		    FeatureTracking		_featureTracking;
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

			// triangulate 3D points from 2D matches
			float triangulate( MapFeature& feature, FeatureMatch & match ) const;

			void estimateCameraPose( const PointSet3d & p3d, const PointSet2d & p2d );
			void correspondencesFromMatchedFeatures( PointSet3d& p3d, 
													 PointSet2d& p2d,
													 std::set<size_t>& matchedIndices,
													 const std::vector<size_t> & predictedIds, 
													 const std::vector<FeatureMatch> & matches );

			void debugPatchWorkImage( const std::set<size_t> & indices,
								      const std::vector<size_t> & featureIds,
								      const std::vector<FeatureMatch> & matches );

			bool newKeyframeNeeded( size_t numTrackedFeatures ) const;
	};

	inline StereoSLAM::StereoSLAM( const CameraCalibration& c0,
								   size_t w0, size_t h0, 
								   const CameraCalibration& c1,
								   size_t w1, size_t h1 ):
		_camCalib0( c0 ), _camCalib1( c1 ),
		_matcherMaxLineDistance( 5.0f ),
		_matcherMaxDescriptorDist( 70 ),
		_stereoMatcher( _matcherMaxLineDistance, _matcherMaxDescriptorDist, _camCalib0, _camCalib1 ),
		_maxTriangReprojError( 7.0f ),
		_orbOctaves( 3 ), 
		_orbScaleFactor( 0.5f ),
		_orbCornerThreshold( 10 ),
		_orbMaxFeatures( 2000 ),
		_orbNonMaxSuppression( true ),
		_trackingSearchRadius( 50.0f ),
		_featureTracking( _descriptorDatabase, _matcherMaxDescriptorDist, _trackingSearchRadius ),
		_minTrackedFeatures( 50 ),
		_activeKF( -1 ),
		_minKeyframeDistance( 0.1 ),
		_maxKeyframeDistance( 0.2 )
	{
		// create the undistortion maps
		_undistortMap0.reallocate( w0, h0, IFormat::GRAYALPHA_FLOAT );
		Vector3f radial = c0.radialDistortion();
		Vector2f tangential = c0.tangentialDistortion();
		float fx = c0.intrinsics()[ 0 ][ 0 ];
		float fy = c0.intrinsics()[ 1 ][ 1 ];
		float cx = c0.intrinsics()[ 0 ][ 2 ];
		float cy = c0.intrinsics()[ 1 ][ 2 ];
		IWarp::warpUndistort( _undistortMap0, radial[ 0 ], radial[ 1 ], cx, cy, fx, fy, w0, h0, radial[ 2 ], tangential[ 0 ], tangential[ 1 ] );

		_undistortMap1.reallocate( w1, h1, IFormat::GRAYALPHA_FLOAT );
		radial = c1.radialDistortion();
		tangential = c1.tangentialDistortion();
		fx = c1.intrinsics()[ 0 ][ 0 ];
		fy = c1.intrinsics()[ 1 ][ 1 ];
		cx = c1.intrinsics()[ 0 ][ 2 ];
		cy = c1.intrinsics()[ 1 ][ 2 ];
		IWarp::warpUndistort( _undistortMap1, radial[ 0 ], radial[ 1 ], cx, cy, fx, fy, w1, h1, radial[ 2 ], tangential[ 0 ], tangential[ 1 ] );
		Eigen::Matrix3d K;
		EigenBridge::toEigen( K, _camCalib0.intrinsics() );
		_map.setIntrinsics( K );
	}

	inline void StereoSLAM::newImages( const Image& img0, const Image& img1 )
	{
		// undistort the first image
		IWarp::apply( _undist0, img0, _undistortMap0 );

		// create the ORB
		ORB orb0( _undist0, 
				  _orbOctaves, 
				  _orbScaleFactor,
				  _orbCornerThreshold,
				  _orbMaxFeatures,
				  _orbNonMaxSuppression );

		// predict visible features with map and current pose
		std::vector<size_t>	  predictedIds;
		std::vector<Vector2f> predictedPositions;
		_map.selectVisibleFeatures( predictedIds, 
									predictedPositions, 
									_pose.transformation(), 
									_camCalib0 );

		std::vector<FeatureMatch> matchedFeatures;
		_featureTracking.trackFeatures( matchedFeatures, 
									    predictedIds,
									    predictedPositions,
									    orb0 );

		PointSet2d p2d;
		PointSet3d p3d;
		std::set<size_t> matchedIndices;
		correspondencesFromMatchedFeatures( p3d, p2d, matchedIndices, predictedIds, matchedFeatures );
		debugPatchWorkImage( matchedIndices, predictedIds, matchedFeatures );

		size_t numTrackedFeatures = p3d.size();
		if( numTrackedFeatures > 6 ){
			estimateCameraPose( p3d, p2d );
		} 

		if( newKeyframeNeeded( numTrackedFeatures ) ){
			std::cout << "Adding new keyframe: current features -> " << p3d.size() << std::endl;
			IWarp::apply( _undist1, img1, _undistortMap1 );

			// create the ORB
			ORB orb1( _undist1, 
					  _orbOctaves, 
					  _orbScaleFactor,
					  _orbCornerThreshold,
					  _orbMaxFeatures,
					  _orbNonMaxSuppression );

			// find stereoMatches by avoiding already found matches
			std::vector<FeatureMatch> matches;
			_stereoMatcher.matchEpipolar( matches, orb0, orb1, matchedIndices );

			// Create a new keyframe with image 0 as reference image
			if( matches.size() > 0 ){
				// add a new keyframe to the map
				size_t kId = _map.addKeyframe( _pose.transformation() );
				Keyframe & keyframe = _map.keyframeForId( kId );
				keyframe.setImage( _undist0 );

				MapMeasurement meas;
				meas.information *= ( 1.0 / _trackingSearchRadius );
				
				Eigen::Matrix4d featureCov = Eigen::Matrix4d::Identity();
				MapFeature mapFeat( Eigen::Vector4d::Zero(), featureCov );

				// add the currently tracked features to the keyframe!
				std::set<size_t>::const_iterator tracked = matchedIndices.begin();
				const std::set<size_t>::const_iterator trackedEnd = matchedIndices.end();
				while( tracked != trackedEnd )
				{
					size_t pointId = predictedIds[ *tracked ];
					meas.point[ 0 ] = matchedFeatures[ *tracked ].feature1->pt.x;
					meas.point[ 1 ] = matchedFeatures[ *tracked ].feature1->pt.y;
					_map.addMeasurement( pointId, kId, meas );
					++tracked;
				}
				
				size_t numNewPoints = 0;
				for( size_t i = 0; i < matches.size(); i++ ){
					if( matches[ i ].feature1 ){
						if( _featureTracking.checkFeature( matches[ i ], _undist0, _undist1 ) ){
							float reprErr = triangulate( mapFeat, matches[ i ] );
							if( reprErr < _maxTriangReprojError ){
								meas.point[ 0 ] = matches[ i ].feature0->pt.x;
								meas.point[ 1 ] = matches[ i ].feature0->pt.y;
								size_t newPointId = _map.addFeatureToKeyframe( mapFeat, meas, kId );
								_descriptorDatabase.addDescriptor( *( ORBFeature* )matches[ i ].feature0, newPointId );
								numNewPoints++;
							}
						}
					}
				}
				std::cout << "Added " << numNewPoints << " new 3D Points" << std::endl;

				// new keyframe added -> run the sba thread	
				if( _map.numKeyframes() > 1 ){
					_bundler.run( &_map );
					_bundler.join();
				}
				mapChanged.notify( _map );				
			} 
			// notify observers that there is new orb data
			// e.g. gui or a localizer 
			ORBData data;
			data.orb0 = &orb0;
			data.img0 = &_undist0;
			data.orb1 = &orb1;
			data.img1 = &_undist1;
			data.matches = &matches;
			newORBData.notify( &data );
		}
 
		int last = _activeKF;
		_activeKF = _map.findClosestKeyframe( _pose.transformation() );
		if( _activeKF != last )
			std::cout << "Active KF: " << _activeKF << std::endl;

		if( orb0.size() < 100 && _orbCornerThreshold > 15 )
			_orbCornerThreshold -=5;
		else if( orb0.size() > _orbMaxFeatures && _orbCornerThreshold < 40 )
			_orbCornerThreshold+=2;
		
		trackedPoints.notify( p2d );
	}

	inline float StereoSLAM::triangulate( MapFeature& feature, 
										  FeatureMatch & match ) const
	{
		Vector4f tmp;
		Vector4f repr;
		Vector2f repr2, p0, p1;

		p0 = match.feature0->pt;
		p1 = match.feature1->pt;

		Vision::correctCorrespondencesSampson( p0, 
											   p1, 
											  _stereoMatcher.fundamental() );

		Vision::triangulate( tmp,
							_camCalib0.projectionMatrix(),
							_camCalib1.projectionMatrix(),
							p0,
							p1 );
			
		// normalize 4th coord;
		tmp /= tmp.w;
		if( tmp.z > 0.0f && tmp.z < 30 ){
			float error = 0.0f;

			repr = _camCalib0.projectionMatrix() * tmp;
			repr2.x = repr.x / repr.z;
			repr2.y = repr.y / repr.z;

			error += ( match.feature0->pt - repr2 ).length();

			repr = _camCalib1.projectionMatrix() * tmp;
			repr2.x = repr.x / repr.z;
			repr2.y = repr.y / repr.z;
			error += ( match.feature1->pt - repr2 ).length();

			error /= 2.0f;

			if( error < _maxTriangReprojError ){
				Eigen::Vector4d & np = feature.estimate();
				np[ 0 ] = tmp.x;
				np[ 1 ] = tmp.y;
				np[ 2 ] = tmp.z;
				np[ 3 ] = tmp.w;

				// transform to world coordinates
				np = _pose.transformation().inverse() * np;
			}
			return error;
		} 
		
		return _maxTriangReprojError;
	}

	inline void StereoSLAM::estimateCameraPose( const PointSet3d & p3d, const PointSet2d & p2d )
	{
		Matrix3d K;
		const Matrix3f & kf = _camCalib0.intrinsics();
		K[ 0 ][ 0 ] = kf[ 0 ][ 0 ];	K[ 0 ][ 1 ] = kf[ 0 ][ 1 ]; K[ 0 ][ 2 ] = kf[ 0 ][ 2 ];
		K[ 1 ][ 0 ] = kf[ 1 ][ 0 ]; K[ 1 ][ 1 ] = kf[ 1 ][ 1 ]; K[ 1 ][ 2 ] = kf[ 1 ][ 2 ];
		K[ 2 ][ 0 ] = kf[ 2 ][ 0 ]; K[ 2 ][ 1 ] = kf[ 2 ][ 1 ]; K[ 2 ][ 2 ] = kf[ 2 ][ 2 ];
		Eigen::Matrix<double, 3, 3> Ke;
		Eigen::Matrix<double, 4, 4> extrC;
		EigenBridge::toEigen( Ke, K );
		EigenBridge::toEigen( extrC, _camCalib0.extrinsics() );
		
		Matrix4d m;
		Eigen::Matrix4d me;
	/*	
		EPnPSAC sacModel( p3d, p2d, K );
		double maxReprojectionError = 5.0;
		double outlierProb = 0.1;
		RANSAC<EPnPSAC> ransac( sacModel, maxReprojectionError, outlierProb );

		size_t maxRansacIters = 200;
		m = ransac.estimate( maxRansacIters );
	*/
	/*
		EPnPd epnp( p3d );
		epnp.solve( m, p2d, K );
		
		// from EPnP we get the pose of the camera, to get pose of the rig, we need to remove the extrinsics
		EigenBridge::toEigen( me, m );
		me = extrC.inverse() * me;
	*/

		me = _pose.transformation();		

		PointCorrespondences3d2d<double> pointCorresp( Ke, extrC );
		pointCorresp.setPose( me );

		Eigen::Matrix<double, 3, 1> p3;
		Eigen::Matrix<double, 2, 1> p2;
		for( size_t i = 0; i < p3d.size(); i++ ){
			p3[ 0 ] = p3d[ i ].x;
			p3[ 1 ] = p3d[ i ].y;
			p3[ 2 ] = p3d[ i ].z;
			p2[ 0 ] = p2d[ i ].x;
			p2[ 1 ] = p2d[ i ].y;
			pointCorresp.add( p3, p2 );
		}
		
		RobustHuber<double, PointCorrespondences3d2d<double>::MeasType> costFunction( 5.0 );
		LevenbergMarquard<double> lm;
		TerminationCriteria<double> termCriteria( TERM_COSTS_THRESH | TERM_MAX_ITER );
		termCriteria.setCostThreshold( 0.001 );
		termCriteria.setMaxIterations( 10 );
		lm.optimize( pointCorresp, costFunction, termCriteria );

		//me = pointCorresp.pose().transformation().inverse();
		me = pointCorresp.pose().transformation();
		_pose.set( me );

		Matrix4f mf;
		EigenBridge::toCVT( mf, me );
		newCameraPose.notify( mf );
	}

	inline void StereoSLAM::clear()
	{
		if( _bundler.isRunning() )
			_bundler.join();
		_map.clear();
		_descriptorDatabase.clear();
		Eigen::Matrix4d I( Eigen::Matrix4d::Identity() );
		_pose.set( I );
		_activeKF = -1;
	}

	inline void StereoSLAM::correspondencesFromMatchedFeatures( PointSet3d& p3d, 
															    PointSet2d& p2d, 
															    std::set<size_t>& matchedIndices,
															    const std::vector<size_t>& predictedIds,
															    const std::vector<FeatureMatch> & matches )
	{
		Vector3d p3;
		Vector2d p2;

		for( size_t i = 0; i < matches.size(); i++ ){
			if( matches[ i ].feature1 ){
				// got a match so add it to the point sets
				const MapFeature & mapFeat = _map.featureForId( predictedIds[ i ] );
				size_t keyframeId = *( mapFeat.pointTrackBegin() );
				if( _featureTracking.checkFeature( matches[ i ], 
												  _map.keyframeForId( keyframeId ).image(),
												  _undist0 ) ) {
					p3.x = mapFeat.estimate().x(); 
					p3.y = mapFeat.estimate().y(); 
					p3.z = mapFeat.estimate().z();
					p3d.add( p3 );
					p2.x = matches[ i ].feature1->pt.x;
					p2.y = matches[ i ].feature1->pt.y;
					p2d.add( p2 );
					matchedIndices.insert( i );
				}
			}
		}
	}

			
	inline void StereoSLAM::debugPatchWorkImage( const std::set<size_t> & indices,
											     const std::vector<size_t> & featureIds,
												 const std::vector<FeatureMatch> & matches )
	{
		std::set<size_t>::const_iterator idIter = indices.begin();
		const std::set<size_t>::const_iterator idIterEnd = indices.end();

		size_t patchSize = 31;
		size_t patchHalf = patchSize >> 1;
		FeatureAnalyzer patchWork( 20, patchSize, 10 );

		Vector2f p0, p1;
		while( idIter != idIterEnd ){
			size_t featureId = featureIds[ *idIter ];
			const MapFeature & mf = _map.featureForId( featureId );
			const Keyframe & kf = _map.keyframeForId( *( mf.pointTrackBegin() ) );
			const MapMeasurement & meas = kf.measurementForId( featureId );

			p0.x = ( float )meas.point[ 0 ]; 
			p0.y = ( float )meas.point[ 1 ];
			if( p0.x < patchHalf || p0.y < patchHalf )
				 std::cout << "Bad Point: " << p0  << " feature id: " << featureId << " kfId: " << kf.id() << std::endl;
			p0.x -= patchHalf;
			p0.y -= patchHalf;

			p1 = matches[ *idIter ].feature1->pt;
			p1.x -= patchHalf; 
			p1.y -= patchHalf;

			patchWork.addPatches( _undist0, p1, kf.image(), p0 );

			++idIter;
		}

		patchWork.image().save( "patchwork.png" );
	}

	inline bool StereoSLAM::newKeyframeNeeded( size_t numTrackedFeatures ) const
	{
		double kfDist = _minKeyframeDistance + 1.0;
	    if( _activeKF != -1 ){
			kfDist = _map.keyframeForId( _activeKF ).distance( _pose.transformation() );
		}

		// if distance is too far from active, always create a new one:
		if( kfDist > _maxKeyframeDistance )
			return true;

		// if too few features and minimum distance from last keyframe create new
		if( numTrackedFeatures < _minTrackedFeatures && kfDist > _minKeyframeDistance )
			return true;

		return false;
	}
}

#endif
