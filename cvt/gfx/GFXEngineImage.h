#ifndef CVT_GFXENINGEIMAGE_H
#define CVT_GFXENINGEIMAGE_H

#include <cvt/gfx/GFXEngine.h>
#include <cvt/gfx/DummyFont.h>

namespace cvt {
	class GFXEngineImage : public GFXEngine
	{
		public:
			GFXEngineImage( Image& img );
			void begin();
			void end();
			void drawLines( const Vector2f* pts, size_t n, float width, const Color& c );
			void fillRect( const Recti& rect, const Color& c );
			void drawRect( const Recti& rect, float width, const Color& c );
			void fillRoundRect( const Recti& rect, float r, const Color& c ) {}
			void drawRoundRect( const Recti& rect, float width, float r, const Color& c ) {}
			void drawEllipse( const Vector2f& center, float rx, float ry, float width, const Color& c ) {}
			void fillEllipse( const Vector2f& center, float rx, float ry, const Color& c ) {}
			void strokePath( const Pathf& path, float width, const Color& c, GFX::StrokeStyle style ) {}
			void fillPath( const Pathf& path, const Color& c, GFX::PolygonFillRule rule ) {}
			void drawText( int x, int y, const char* text, const Color& c ) {}
			void drawImage( int x, int y, const Image& img, float alpha ) {}
			void drawImage( const Recti& rect, const Image& img, float alpha ) {}
			void drawIcon( int x, int y, GFX::Icon i, const Color& c ) {}
			void drawIcons( const Vector2i* pts, size_t npts, GFX::Icon i, const Color& c ) {}
			void drawIcons( const Vector2f* pts, size_t npts, GFX::Icon i, const Color& c ) {}

			const Font& font() const { return _dfont; }

			/* optional - only used by the Widget in paintChild */
			void setClipRect( const Recti& childrect ) { _crect = childrect; }
			const Recti& clipRect() const { return _crect;}

			void setTranslation( const Vector2i& t ) { _translation = t; }
			const Vector2i& translation() const { return _translation; }

		private:
			void drawLine( const Vector2i& pt1, const Vector2i& pt2, float width, const Color& c );

			Image&		_img;
			Recti		_crect;
			Vector2i	_translation;
			DummyFont	_dfont;
			uint8_t*	_ptr;
			size_t		_stride;
	};

	inline GFXEngineImage::GFXEngineImage( Image& img ) : _img( img )
	{
	}

	inline void GFXEngineImage::begin()
	{
		_ptr = _img.map( &_stride );
	}

	inline void GFXEngineImage::end()
	{
		_img.unmap( _ptr );
	}

}

#endif
