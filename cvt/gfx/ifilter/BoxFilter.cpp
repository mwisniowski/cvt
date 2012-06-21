/*
			CVT - Computer Vision Tools Library

 	 Copyright (c) 2012, Philipp Heise, Sebastian Klose

 	THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 	KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 	IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 	PARTICULAR PURPOSE.
 */
#include <cvt/gfx/ifilter/BoxFilter.h>
#include <cvt/gfx/IMapScoped.h>

#include <cvt/cl/kernel/boxfilter/boxfilter_prefixsum.h>
#include <cvt/cl/kernel/boxfilter/boxfilter.h>

namespace cvt {
	static ParamInfoTyped<Image*> pin( "Input", true );
	static ParamInfoTyped<Image*> pout( "Output", false );
	static ParamInfoTyped<int>	  pradius( "Radius", false );

	static ParamInfo * _params[ 3 ] = {
		&pin,
		&pout,
		&pradius
	};

	BoxFilter::BoxFilter() :
		IFilter( "BoxFilter", _params, 3, IFILTER_CPU | IFILTER_OPENCL ),
		_clboxfilter_prefixsum( _boxfilter_prefixsum_source, "boxfilter_prefixsum" ),
		_clboxfilter( _boxfilter_source, "boxfilter" )
	{
	}

	void BoxFilter::apply( Image& dst, const Image& src, const int radius, IFilterType type ) const
	{
		if( type == IFILTER_OPENCL ) {
			// FIXME: integrate IntegralFilter
			dst.reallocate( src.width(), src.height(), dst.format(), IALLOCATOR_CL );
			CLNDRange global( Math::pad16( src.width() ), Math::pad16( src.height() ) );
			//if( integral ) {
				_clboxfilter_prefixsum.setArg( 0, dst );
				_clboxfilter_prefixsum.setArg( 1, src );
				_clboxfilter_prefixsum.setArg( 2, radius );
				_clboxfilter_prefixsum.run( global, CLNDRange( 16, 16 ) );
			/*} else {
				_clboxfilter.setArg( 0, dst );
				_clboxfilter.setArg( 1, src );
				_clboxfilter.setArg( 2, radius );
				_clboxfilter.setArg( 3, CLLocalSpace( sizeof( cl_float4 ) * ( 16 + 2 * radius ) * ( 16 + 2 * radius ) ) );
				_clboxfilter.runWait( global, CLNDRange( 16, 16 ) );
			}*/
		} else {
			dst.reallocate( src.width(), src.height(), dst.format() );
			if( src.format().formatID == IFORMAT_GRAY_FLOAT && dst.format().formatID == IFORMAT_GRAY_FLOAT ) {
				cvt::IMapScoped<float> mapdst( dst );
				cvt::IMapScoped<const float> mapsrc( src );
				SIMD::instance()->boxFilterPrefixSum1_f_to_f( mapdst.ptr(), mapdst.stride(), mapsrc.ptr(), mapsrc.stride(), dst.width(), dst.height(), 2*radius+1, 2*radius+1 );
			} else if( src.format().formatID == IFORMAT_GRAY_FLOAT && dst.format().formatID == IFORMAT_GRAY_UINT8 ) {
				cvt::IMapScoped<uint8_t> mapdst( dst );
				cvt::IMapScoped<const float> mapsrc( src );
				SIMD::instance()->boxFilterPrefixSum1_f_to_u8( mapdst.ptr(), mapdst.stride(), mapsrc.ptr(), mapsrc.stride(), dst.width(), dst.height(), 2*radius+1, 2*radius+1 );
			} else
				throw CVTException( "Unsupported image format!" );
		}
	}

	void BoxFilter::apply( const ParamSet* set, IFilterType t ) const
	{
		Image * in = set->arg<Image*>( 0 );
		Image * out = set->arg<Image*>( 2 );
		int radius = set->arg<int>( 3 );


		switch ( t ) {
			case IFILTER_OPENCL:
				this->apply( *out, *in, radius );
				break;
			default:
				throw CVTException( "Not implemented" );
		}
	}

}
