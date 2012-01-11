#ifndef CVT_OBJLOADER_H
#define CVT_OBJLOADER_H

#include <cvt/util/PluginManager.h>
#include <cvt/geom/scene/Scene.h>
#include <cvt/util/Data.h>

namespace cvt {
	class ObjLoader : public SceneLoader {
		public:
			ObjLoader() {}
			~ObjLoader() {}
			void load( Scene& scene, const String& filename );

			const String& extension( size_t i ) const { return _extensions[ i ]; }
			size_t sizeExtensions() const { return 2; }
			const String& name() const { return _name; }

		private:
			static String _name;
			static String _extensions[ 2 ];
	};

}

#endif
