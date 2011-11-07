#ifndef CVT_BUTTON_H
#define CVT_BUTTON_H

#include <cvt/gui/Widget.h>
#include <cvt/util/Signal.h>
#include <cvt/util/String.h>

namespace cvt {
	class Button : public Widget
	{
		public:
			Button( const String& label );
			~Button();

			void setLabel( const String & label ) { _label = label; }

			Signal<void> clicked;

		private:
			void paintEvent( PaintEvent* e, GFX* g );
			void mouseReleaseEvent( MouseReleaseEvent* event );


			Button( const Button& b );

			String _label;
	};
}

#endif
