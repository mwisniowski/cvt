/*
            CVT - Computer Vision Tools Library

     Copyright (c) 2012, Philipp Heise, Sebastian Klose

    THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
    PARTICULAR PURPOSE.
 */
#ifndef CVT_IMAPSCOPED_H
#define CVT_IMAPSCOPED_H

#include <cvt/gfx/Image.h>

namespace cvt {

	template<typename T>
		class IMapScoped {
			private:
				template<typename T2>
					struct ITypeSelector;

				template<typename T2>
					struct ITypeSelector
					{
						typedef Image& IType;
						typedef uint8_t* IPtrType;
					};

				template<typename T2>
					struct ITypeSelector<const T2>
					{
						typedef const Image& IType;
						typedef const uint8_t* IPtrType;
					};

				typedef typename ITypeSelector<T>::IType IType;
				typedef typename ITypeSelector<T>::IPtrType IPtrType;

			public:
				IMapScoped( IType img );
				~IMapScoped();

				void		reset();
				T*			ptr();
				T*			base();
				T*			line( size_t y );
				void		prevLine();
				void		nextLine();
				void		setLine( size_t l );
				void		operator++( int );
				void		operator--( int );
				size_t		stride() const;
				T&			operator()( size_t row, size_t col );
				const T&	operator()( size_t row, size_t col ) const;

			private:
				IType	    _img;
				IPtrType	_base;
				IPtrType	_line;
				size_t		_stride;
		};

	template<typename T>
	inline IMapScoped<T>::IMapScoped( IMapScoped<T>::IType img ) : _img( img )
	{
		_base = _img.map( &_stride );
		_line = _base;
	}

	template<typename T>
	inline IMapScoped<T>::~IMapScoped( )
	{
		_img.unmap( _base );
	}

	template<typename T>
	inline void IMapScoped<T>::reset()
	{
		_line = _base;
	}

	template<typename T>
	inline T* IMapScoped<T>::ptr()
	{
		return ( T* ) _line;
	}

	template<typename T>
	inline T* IMapScoped<T>::base()
	{
		return ( T* ) _base;
	}


	template<typename T>
	inline T* IMapScoped<T>::line( size_t y )
	{
		return ( T* ) ( _base + _stride * y );
	}

	template <typename T>
	inline void IMapScoped<T>::setLine( size_t l )
	{
		if( l >= _img.height() )
			throw CVTException( "Trying to set line higher than actual image height!" );
		_line = _base + _stride * l;
	}

	template<typename T>
	inline void IMapScoped<T>::nextLine()
	{
		_line += _stride;
	}

	template<typename T>
	inline void IMapScoped<T>::prevLine()
	{
		_line -= _stride;
	}

	template<typename T>
	inline void IMapScoped<T>::operator++( int )
	{
		_line += _stride;
	}

	template<typename T>
	inline void IMapScoped<T>::operator--( int )
	{
		_line -= _stride;
	}

	template<typename T>
	inline size_t IMapScoped<T>::stride() const
	{
		return _stride;
	}

	template<typename T>
	inline T& IMapScoped<T>::operator()( size_t row, size_t col )
	{
		return *( ( T* ) ( _base + row * _stride + col * sizeof( T ) ) );
	}

	template<typename T>
	inline const T& IMapScoped<T>::operator()( size_t row, size_t col ) const
	{
		return *( ( const T* ) ( _base + row * _stride + col * sizeof( T ) ) );
	}

}

#endif
