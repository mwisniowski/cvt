/*
			CVT - Computer Vision Tools Library

 	 Copyright (c) 2012, Philipp Heise, Sebastian Klose

 	THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 	KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 	IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 	PARTICULAR PURPOSE.
 */
#ifndef CVT_QUATERNION_H
#define CVT_QUATERNION_H

#include <cvt/math/Vector.h>
#include <cvt/math/Matrix.h>

namespace cvt {
	template<typename T>
		class Quaternion
		{
			public:
				Quaternion();
				Quaternion( T x, T y, T z, T w );
				Quaternion( T phi, T theta, T psi );
				Quaternion( const Quaternion& q2 );

				void				set( T x, T y, T z, T w );
				void				setRotation( T x, T y, T z, T rad );
				void				setRotation( const Vector3<T>& axis, T rad );
				T					operator[]( int index ) const;
				T&					operator[]( int index );
				Quaternion<T>		operator-() const;
				Quaternion<T>&		operator=( const Quaternion<T>& q );
				Quaternion<T>		operator+( const Quaternion<T>& q ) const;
				Quaternion<T>&		operator+=( const Quaternion<T>& q );
				Quaternion<T>		operator-( const Quaternion<T>& q ) const;
				Quaternion<T>&		operator-=( const Quaternion<T>& q );
				Quaternion<T>		operator*( const Quaternion<T>& q ) const;
				Vector3<T>			operator*( const Vector3<T>& q ) const;
				Quaternion<T>		operator*( T a ) const;
				Quaternion<T>&		operator*=( const Quaternion<T>& q );
				Quaternion<T>&		operator*=( T a );

				bool				operator==(	const Quaternion<T>& q ) const;
				bool				operator!=(	const Quaternion<T>& q ) const;

				Quaternion<T>		inverse( void ) const;
				T					length( void ) const;
				T					normalize( void );

				int					dimension( void ) const;

				Matrix3<T>			toMatrix3( void ) const;
				Matrix4<T>			toMatrix4( void ) const;
				Vector3<T>			toEuler( void ) const;
				const T*			ptr( void ) const;
				T*					ptr( void );

				T x, y, z, w;
		};

	template<typename T>
		inline Quaternion<T>::Quaternion()
		{		
		}

	template<typename T>
		inline Quaternion<T>::Quaternion( T x, T y, T z, T w )
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

	template<typename T>
		inline Quaternion<T>::Quaternion( const Quaternion& q )
		{
			x = q.x;
			y = q.y;
			z = q.z;
			w = q.w;
		}

	template<typename T>
		inline Quaternion<T>::Quaternion( T phi, T theta , T psi )
		{
			T s_phi   = Math::sin( phi / ( T )2 );
			T s_theta = Math::sin( theta / ( T )2 );
			T s_psi   = Math::sin( psi / ( T )2 );
			T c_phi   = Math::cos( phi / ( T )2 );
			T c_theta = Math::cos( theta / ( T )2 );
			T c_psi   = Math::cos( psi / ( T )2 );

			w = c_phi * c_theta * c_psi + s_phi * s_theta * s_psi;
			x = s_phi * c_theta * c_psi - c_phi * s_theta * s_psi;
			y = c_phi * s_theta * c_psi + s_phi * c_theta * s_psi;
			z = c_phi * c_theta * s_psi - s_phi * s_theta * c_psi;
		}

	template<typename T>
		inline void Quaternion<T>::set( T x, T y, T z, T w )
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

	template<typename T>
		inline void Quaternion<T>::setRotation( T x, T y, T z, T rad )
		{
			Vector3<T> axis( x, y, z );
			axis.normalize();
			T c, s;

			c = Math::cos( rad * ( T ) 0.5 );
			s = Math::sin( rad * ( T ) 0.5 );
			this->x = axis.x * s;
			this->y = axis.y * s;
			this->z = axis.z * s;
			this->w = c;
		}

	template<typename T>
		inline void Quaternion<T>::setRotation( const Vector3<T>& _axis, T rad )
		{
			Vector3<T> axis( _axis );
			axis.normalize();
			T c, s;

			c = Math::cos( rad * ( T ) 0.5 );
			s = Math::sin( rad * ( T ) 0.5 );
			this->x = axis.x * s;
			this->y = axis.y * s;
			this->z = axis.z * s;
			this->w = c;
		}


	template<typename T>
		inline T Quaternion<T>::operator[]( int index ) const
		{
			return ( &x )[ index ];
		}

	template<typename T>
		inline T& Quaternion<T>::operator[]( int index )
		{
			return ( &x )[ index ];
		}

	template<typename T>
		inline Quaternion<T> Quaternion<T>::operator-() const
		{
			return Quaternion<T>( -x, -y, -z, -w );
		}

	template<typename T>
		inline Quaternion<T>& Quaternion<T>::operator=( const Quaternion<T> &q )
		{
			x = q.x;
			y = q.y;
			z = q.z;
			w = q.w;
		}

	template<typename T>
		inline Quaternion<T> Quaternion<T>::operator+( const Quaternion<T> &q ) const
		{
			return Quaternion<T>( x + q.x, y + q.y, z + q.z, w + q.w );
		}


	template<typename T>
		inline Quaternion<T>& Quaternion<T>::operator+=( const Quaternion<T> &q )
		{
			x += q.x;
			y += q.y;
			z += q.z;
			w += q.w;
			return *this;
		}

	template<typename T>
		inline Quaternion<T> Quaternion<T>::operator-( const Quaternion<T> &q ) const
		{
			return Quaternion<T>( x - q.x, y - q.y, z - q.z, w - q.w );
		}

	template<typename T>
		inline Quaternion<T>& Quaternion<T>::operator-=( const Quaternion<T> &q )
		{
			x -= q.x;
			y -= q.y;
			z -= q.z;
			w -= q.w;
			return *this;
		}

	template<typename T>
		inline Quaternion<T> Quaternion<T>::operator*( const Quaternion<T> &q ) const
		{
			return Quaternion<T>(	w * q.x + x * q.w + y * q.z - z * q.y,
									w * q.y + y * q.w + z * q.x - x * q.z,
									w * q.z + z * q.w + x * q.y - y * q.x,
									w * q.w - x * q.x - y * q.y - z * q.z );

		}

	template<typename T>
		inline Vector3<T> Quaternion<T>::operator*( const Vector3<T> &v ) const
		{
			T xxzz = x * x - z * z;
			T wwyy = w * w - y * y;

			T xw2 = x * w * 2;
			T xy2 = x * y * 2;
			T xz2 = x * z * 2;
			T yw2 = y * w * 2;
			T yz2 = y * z * 2;
			T zw2 = z * w * 2;

			return Vector3<T>(
							  ( xxzz + wwyy ) * v.x	+ ( xy2 + zw2 ) * v.y					 + ( xz2 - yw2 ) * v.z,
							  ( xy2 - zw2 ) * v.x	+ ( y * y + w * w- x * x - z * z ) * v.y + ( yz2 + xw2 ) * v.z,
							  ( xz2 + yw2 ) * v.x	+ ( yz2 - xw2 ) * v.y					 + ( wwyy - xxzz ) * v.z
							 );
		}

	template<typename T>
		inline Quaternion<T> Quaternion<T>::operator*( T c ) const
		{
			return Quaternion<T>( x * c, y * c, z * c, w * c );
		}

	template<typename T>
		inline Quaternion<T>& Quaternion<T>::operator*=( T c )
	{
		x *= c;
		y *= c;
		z *= c;
		w *= c;
		return *this;
	}

	template<typename T>
		inline Quaternion<T>& Quaternion<T>::operator*=( const Quaternion<T> &q )
		{
			*this = *this * q;
			return *this;
		}

	template<>
		inline bool Quaternion<float>::operator==( const Quaternion<float>& q ) const
		{
			return Math::abs( q.x - x ) < Math::EPSILONF
				&& Math::abs( q.y - y ) < Math::EPSILONF
				&& Math::abs( q.z - z ) < Math::EPSILONF
				&& Math::abs( q.w - w ) < Math::EPSILONF;
		}

	template<>
		inline bool Quaternion<float>::operator!=( const Quaternion<float>& q ) const
		{
			return Math::abs( q.x - x ) > Math::EPSILONF
				|| Math::abs( q.y - y ) > Math::EPSILONF
				|| Math::abs( q.z - z ) > Math::EPSILONF
				|| Math::abs( q.w - w ) > Math::EPSILONF;
		}

	template<>
		inline bool Quaternion<double>::operator==( const Quaternion<double>& q ) const
		{
			return Math::abs( q.x - x ) < Math::EPSILOND
				&& Math::abs( q.y - y ) < Math::EPSILOND
				&& Math::abs( q.z - z ) < Math::EPSILOND
				&& Math::abs( q.w - w ) < Math::EPSILOND;
		}

	template<>
		inline bool Quaternion<double>::operator!=( const Quaternion<double>& q ) const
		{
			return Math::abs( q.x - x ) > Math::EPSILOND
				|| Math::abs( q.y - y ) > Math::EPSILOND
				|| Math::abs( q.z - z ) > Math::EPSILOND
				|| Math::abs( q.w - w ) > Math::EPSILOND;
		}


	template<typename T>
		inline Quaternion<T> Quaternion<T>::inverse( void ) const
		{
			return Quaternion<T>( -x, -y, -z, w );
		}

	template<typename T>
		inline T Quaternion<T>::length( void ) const
		{
			return Math::sqrt( x * x + y * y + z * z  + w * w );
		}

	template<typename T>
		inline T Quaternion<T>::normalize( void )
		{
			T lenSqr, lenInv;

			lenSqr = x * x + y * y + z * z + w * w;
			lenInv = Math::invSqrt( lenSqr );
			x *= lenInv;
			y *= lenInv;
			z *= lenInv;
			w *= lenInv;
			return lenSqr * lenInv;
		}


	template<typename T>
		inline int Quaternion<T>::dimension( void ) const
		{
			return 4;
		}

	template<typename T>
		inline Matrix3<T> Quaternion<T>::toMatrix3( void ) const
		{
			Matrix3<T> mat;
			T wx, wy, wz;
			T xx, yy, yz;
			T xy, xz, zz;
			T x2, y2, z2;

			x2 = x + x;
			y2 = y + y;
			z2 = z + z;

			xx = x * x2;
			xy = x * y2;
			xz = x * z2;

			yy = y * y2;
			yz = y * z2;
			zz = z * z2;

			wx = w * x2;
			wy = w * y2;
			wz = w * z2;

			mat[ 0 ][ 0 ] = ( T ) 1 - ( yy + zz );
			mat[ 0 ][ 1 ] = xy + wz;
			mat[ 0 ][ 2 ] = xz - wy;

			mat[ 1 ][ 0 ] = xy - wz;
			mat[ 1 ][ 1 ] = ( T ) 1 - ( xx + zz );
			mat[ 1 ][ 2 ] = yz + wx;

			mat[ 2 ][ 0 ] = xz + wy;
			mat[ 2 ][ 1 ] = yz - wx;
			mat[ 2 ][ 2 ] = ( T ) 1 - ( xx + yy );

			return mat;
		}

	template<typename T>
		inline Matrix4<T> Quaternion<T>::toMatrix4( void ) const
		{
			Matrix4<T> mat;
			T wx, wy, wz;
			T xx, yy, yz;
			T xy, xz, zz;
			T x2, y2, z2;

			x2 = x + x;
			y2 = y + y;
			z2 = z + z;

			xx = x * x2;
			xy = x * y2;
			xz = x * z2;

			yy = y * y2;
			yz = y * z2;
			zz = z * z2;

			wx = w * x2;
			wy = w * y2;
			wz = w * z2;

			mat[ 0 ][ 0 ] = ( T ) 1 - ( yy + zz );
			mat[ 0 ][ 1 ] = xy + wz;
			mat[ 0 ][ 2 ] = xz - wy;
			mat[ 0 ][ 3 ] = 0;

			mat[ 1 ][ 0 ] = xy - wz;
			mat[ 1 ][ 1 ] = ( T ) 1 - ( xx + zz );
			mat[ 1 ][ 2 ] = yz + wx;
			mat[ 1 ][ 3 ] = 0;

			mat[ 2 ][ 0 ] = xz + wy;
			mat[ 2 ][ 1 ] = yz - wx;
			mat[ 2 ][ 2 ] = ( T ) 1 - ( xx + yy );
			mat[ 2 ][ 3 ] = 0;

			mat[ 3 ][ 0 ] = 0;
			mat[ 3 ][ 1 ] = 0;
			mat[ 3 ][ 2 ] = 0;
			mat[ 3 ][ 3 ] = ( T ) 1;

			return mat;
		}

	template<typename T>
		inline Vector3<T> Quaternion<T>::toEuler() const
		{
			Vector3<T> result; // phi, theta, psi
			T xxzz = x * x - z * z;
			T wwyy = w * w - y * y;

			result[ 0 ] = cvt::Math::atan2( 2.0f *( y * z + w * x ), wwyy - xxzz );

			T tmp = (T)-2 * x * z - w * y;
			tmp   = cvt::Math::max( (T)-1, cvt::Math::min( tmp, (T)1 ) );
			result[ 1 ] = cvt::Math::asin( tmp );
			result[ 2 ] = cvt::Math::atan2( (T)2 * ( x * y + w * z ), wwyy + xxzz );

			return result;
		}

	template<typename T>
		inline const T* Quaternion<T>::ptr( void ) const
		{
			return &x;
		}

	template<typename T>
		inline T* Quaternion<T>::ptr( void )
		{
			return &x;
		}

	template<typename T>
	std::ostream& operator<<( std::ostream& out, const Quaternion<T> &q )
	{
		out << "Quaternion: [ " << q.x << " " << q.y << " " << q.z << " " << q.w << " ] ";
		return out;
	}

	typedef Quaternion<float> Quaternionf;
	typedef Quaternion<double> Quaterniond;
}

#endif
