/*
            CVT - Computer Vision Tools Library

     Copyright (c) 2012, Philipp Heise, Sebastian Klose

    THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
    PARTICULAR PURPOSE.
 */
#ifndef CVT_BRIEF_H
#define CVT_BRIEF_H

#include <cvt/gfx/Image.h>
#include <cvt/gfx/IMapScoped.h>
#include <cvt/vision/ImagePyramid.h>
#include <cvt/vision/features/FeatureSet.h>
#include <cvt/vision/features/FeatureDescriptor.h>
#include <cvt/vision/features/FeatureDescriptorExtractor.h>

#include "MatchBruteForce.inl"

namespace cvt {

	template<size_t N>
	class BRIEF : public FeatureDescriptorExtractor
	{
		public:
			typedef FeatureDescriptorInternal<N, uint8_t, FEATUREDESC_CMP_HAMMING> Descriptor;

			BRIEF( size_t boxradius = 3 );
			BRIEF( const BRIEF& brief );
			~BRIEF();

			size_t					  size() const;
			FeatureDescriptor&		  operator[]( size_t i );
			const FeatureDescriptor&  operator[]( size_t i ) const;

			void extract( const Image& img, const FeatureSet& features );
			void extract( const ImagePyramid& pyr, const FeatureSet& features );

			void matchBruteForce( std::vector<FeatureMatch>& matches, const FeatureDescriptorExtractor& other, float distThresh ) const;

		private:
			struct DistFunc {
				DistFunc() : _simd( SIMD::instance() )
				{
				}

				float operator()( const Descriptor& a, const Descriptor& b ) const
				{
					return _simd->hammingDistance( a.desc, b.desc, N );
				}

				SIMD* _simd;
			};



			void extractF( const Image& img, const FeatureSet& features );
			void extractU8( const Image& img, const FeatureSet& features );

			const size_t			_boxradius;
			std::vector<Descriptor> _features;

	};

#include <cvt/vision/features/BRIEFPattern.h>

	typedef BRIEF<16> BRIEF16;
	typedef BRIEF<32> BRIEF32;
	typedef BRIEF<64> BRIEF64;

	template<size_t N>
	inline BRIEF<N>::BRIEF( size_t boxradius ) :
		_boxradius( boxradius )
	{
	}

	template<size_t N>
	inline BRIEF<N>::BRIEF( const BRIEF<N>& other ) :
		_boxradius( other._boxradius ),
		_features( other._features )
	{
	}

	template<size_t N>
	inline BRIEF<N>::~BRIEF()
	{
	}

	template<size_t N>
	inline size_t BRIEF<N>::size() const
	{
		return _features.size();
	}

	template<size_t N>
	inline FeatureDescriptor& BRIEF<N>::operator[]( size_t i )
	{
		return _features[ i ];
	}

	template<size_t N>
	inline const FeatureDescriptor& BRIEF<N>::operator[]( size_t i ) const
	{
		return _features[ i ];
	}

	template<size_t N>
	inline void BRIEF<N>::extract( const ImagePyramid& img, const FeatureSet& features )
	{
	}

	template<size_t N>
	inline void BRIEF<N>::extract( const Image& img, const FeatureSet& features )
	{
		if( img.channels() != 1 || ( img.format() != IFormat::GRAY_UINT8 && img.format() != IFormat::GRAY_FLOAT ) )
			throw CVTException( "Unimplemented" );

		if( img.format() == IFormat::GRAY_FLOAT )
			extractF( img, features );
		else if( img.format() == IFormat::GRAY_UINT8 )
			extractU8( img, features );
	}


	template<size_t N>
	inline void BRIEF<N>::extractF( const Image& img, const FeatureSet& features )
	{
		Image boximg;
		img.boxfilter( boximg, _boxradius );

#define DOBRIEFTEST( n ) ( map( py + _brief_pattern[ n ][ 1 ], px + _brief_pattern[ n ][ 0 ] ) < map( py + _brief_pattern[ n ][ 3 ], px + _brief_pattern[ n ][ 2 ] ) )

		IMapScoped<const float> map( boximg );
		size_t iend = features.size();
		for( size_t i = 0; i < iend; ++i ) {
			int px, py;
			px = features[ i ].pt.x;
			py = features[ i ].pt.y;
			_features.push_back( Descriptor( features[ i ] ) );
			for( size_t n = 0; n < N; n++ ) {
				uint8_t tests  = 0;
				size_t  offset = n * 8;
				for( int t = 0; t < 8; t++ )
					tests |= DOBRIEFTEST( n * 8 + t ) << t;
				_features.back().desc[ n ] = tests;
			}
		}

	}

	template<size_t N>
	inline void BRIEF<N>::extractU8( const Image& img, const FeatureSet& features )
	{
		Image boximg;
		img.boxfilter( boximg, _boxradius );

#define DOBRIEFTEST( n ) ( map( py + _brief_pattern[ n ][ 1 ], px + _brief_pattern[ n ][ 0 ] ) < map( py + _brief_pattern[ n ][ 3 ], px + _brief_pattern[ n ][ 2 ] ) )

		IMapScoped<const uint8_t> map( boximg );
		size_t iend = features.size();
		for( size_t i = 0; i < iend; ++i ) {
			int px, py;
			px = features[ i ].pt.x;
			py = features[ i ].pt.y;
			_features.push_back( Descriptor( features[ i ] ) );
			for( size_t n = 0; n < N; n++ ) {
				uint8_t tests  = 0;

				for( int t = 0; t < 8; t++ ) {
					tests |= ( DOBRIEFTEST( n * 8 + t ) << t );
				}
				_features.back().desc[ n ] = tests;
			}
		}
	}

	template<size_t N>
	inline void BRIEF<N>::matchBruteForce( std::vector<FeatureMatch>& matches, const FeatureDescriptorExtractor& other, float distThresh ) const
	{
		DistFunc dfunc;
		FeatureMatcher::matchBruteForce<Descriptor, typename BRIEF<N>::DistFunc>( matches, this->_features, ( ( const BRIEF<N>& ) other)._features, dfunc, distThresh );
	}
}

#endif
