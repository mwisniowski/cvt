#ifndef CVT_ICONVERT_H
#define CVT_ICONVERT_H

#include <cvt/gfx/IFormat.h>
#include <cvt/util/Flags.h>
#include <cvt/util/Exception.h>

namespace cvt
{
	class Image;

	enum IConvertFlagTypes {
		ICONVERT_DEBAYER_LINEAR = ( 1 << 0 ),
		ICONVERT_DEBAYER_HQLINEAR = ( 1 << 1 )
		/*
		 TODO: gamma treatment

		 ICONVERT_COLOR_SRGB_TO_LINEAR = ( 1 << 2 ),
		 ICONVERT_COLOR_LINEAR_TO_LINEAR = ( 1 << 3 ),
		 ICONVERT_COLOR_LINEAR_TO_SRGB = ( 1 << 4 ),
		 */
	};

	CVT_ENUM_TO_FLAGS( IConvertFlagTypes, IConvertFlags )

	typedef void (*ConversionFunction)( Image&, const Image&, IConvertFlags flags );

	class IConvert
	{
		public:
			/* conversion from source format to dst format */
			static void convert( Image& dst, const Image& src, IConvertFlags flags = ICONVERT_DEBAYER_LINEAR );

			static const IConvert & instance();

		private:
			IConvert();
			IConvert( const IConvert& ) {}

			static IConvert * _instance;
			ConversionFunction * _convertFuncs;

			void initTable();
	};
}

#endif
