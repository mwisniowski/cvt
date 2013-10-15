/*
 * File:   FeatureMatch.h
 * Author: Sebastian Klose
 *
 * Created on June 28, 2011
 */

#ifndef CVT_FEATUREMATCH_H
#define	CVT_FEATUREMATCH_H

#include <cvt/vision/features/Feature.h>

namespace cvt {

    struct FeatureMatch {

        FeatureMatch() :
            feature0( 0 ), feature1( 0 ), distance( -1.0f )
        {
        }

        FeatureMatch( const FeatureMatch & other ) :
            feature0( other.feature0 ),
            feature1( other.feature1 ),
            distance( other.distance )
        {
        }

        FeatureMatch& operator=( const FeatureMatch& other )
        {
            feature0 = other.feature0;
            feature1 = other.feature1;
            distance = other.distance;
            return *this;
        }

        const Feature* 	feature0;
        const Feature* 	feature1;
        float			distance;
	};

	struct MatchingIndices {
		size_t	srcIdx;
		size_t	dstIdx;
		float	distance;
	};

}

#endif

