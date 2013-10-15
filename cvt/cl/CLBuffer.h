#ifndef CVT_CLBUFFER_H
#define CVT_CLBUFFER_H

#include <cvt/cl/CLObject.h>
#include <cvt/cl/CLUtil.h>

#include <cvt/gl/GLBuffer.h>

namespace cvt {
	class CLContext;

	class CLBuffer : public CLMemory
	{
		public:
			CLBuffer( const CLContext& context, size_t size, cl_mem_flags flags = CL_MEM_READ_WRITE, void* host_ptr = NULL );
			CLBuffer( size_t size, cl_mem_flags = CL_MEM_READ_WRITE );
			CLBuffer( void* data, size_t size, cl_mem_flags = CL_MEM_READ_WRITE );
			CLBuffer( const CLContext& context, const GLBuffer& buf, cl_mem_flags flags = CL_MEM_READ_WRITE );
			CLBuffer( const GLBuffer& buf, cl_mem_flags = CL_MEM_READ_WRITE );

			CLUTIL_GETINFOTYPE( memType, CL_MEM_TYPE, cl_mem_object_type, _object, ::clGetMemObjectInfo )
			CLUTIL_GETINFOTYPE( memFlags, CL_MEM_FLAGS, cl_mem_flags, _object, ::clGetMemObjectInfo )
			CLUTIL_GETINFOTYPE( hostPtr, CL_MEM_HOST_PTR, void*, _object, ::clGetMemObjectInfo )
			CLUTIL_GETINFOTYPE( mapCount, CL_MEM_MAP_COUNT, cl_uint, _object, ::clGetMemObjectInfo )

			CLContext	context() const;

			void		acquireGLObject() const;
			void		releaseGLObject() const;

			size_t		size() const;
			void		read( void* dst );
			void		write( void* src );

			void*		map();
			const void* map() const;
			void		unmap( const void* ptr ) const;

		private:
			CLBuffer( cl_mem mem ) : CLMemory( mem ) {};
			CLBuffer( const CLBuffer& buf ) : CLMemory( buf._object ) {};

			CLUTIL_GETINFOTYPE( _context, CL_MEM_CONTEXT, cl_context, _object, ::clGetMemObjectInfo )
			CLUTIL_GETINFOTYPE( _clsize, CL_MEM_SIZE, size_t, _object, ::clGetMemObjectInfo )

			size_t _size;
	};

	inline size_t CLBuffer::size() const
	{
		return _size;
	}

}

#endif
