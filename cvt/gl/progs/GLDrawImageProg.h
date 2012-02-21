/*
			CVT - Computer Vision Tools Library

 	 Copyright (c) 2012, Philipp Heise, Sebastian Klose

 	THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 	KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 	IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 	PARTICULAR PURPOSE.
 */
#ifndef CVT_GLDRAWIMAGEPROG_H
#define CVT_GLDRAWIMAGEPROG_H

#include <cvt/gl/OpenGL.h>
#include <cvt/gl/GLProgram.h>
#include <cvt/gl/GLVertexArray.h>
#include <cvt/gl/GLBuffer.h>
#include <cvt/gl/GLTexture.h>
#include <cvt/gfx/Image.h>
#include <cvt/math/Matrix.h>
#include <cvt/geom/Rect.h>

namespace cvt {
	class GLDrawImageProg : private GLProgram
	{
		public:
			GLDrawImageProg();
			~GLDrawImageProg();

			using GLProgram::bind;
			using GLProgram::unbind;
			void setProjection( const Matrix4f& projection );
			void setAlpha( float alpha );
			void drawImage( int x, int y, const Image& img );
			void drawImage( int x, int y, int width, int height, const Image& img );
			void drawImage( int x, int y, int width, int height, const GLTexture& tex );

		private:
			Matrix4f	_proj;
			GLVertexArray _vao;
			GLBuffer _vbo;
			GLint _mvploc;
			GLint _texloc;
	};


}

#endif
