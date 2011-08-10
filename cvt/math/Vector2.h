#ifdef CVT_VECTOR_H

namespace cvt {

	template<typename T> class Vector3;

    template<typename T>
	class Vector2 {
	    public:
					Vector2();
					Vector2( const T x, const T y );
					Vector2( const Vector2<T>& vec2 );
					Vector2( const Vector3<T>& vec3 );

		void		set( T x, T y );
		void		setZero( void );

		T			operator[]( int index ) const;
		T&			operator[]( int index );
		Vector2<T>	operator-() const;
		T			operator*( const Vector2<T> &v ) const;
		Vector2<T>	operator*( const T c ) const;
		Vector2<T>	cmul( const Vector2<T>& v ) const;
		Vector2<T>	operator/( const T c ) const;
		Vector2<T>	operator+( const Vector2<T> &v ) const;
		Vector2<T>	operator-( const Vector2<T> &v ) const;
		Vector2<T>&	operator+=( const Vector2<T> &v );
		Vector2<T>&	operator-=( const Vector2<T> &v );
		Vector2<T>&	operator/=( const Vector2<T> &v );
		Vector2<T>&	operator+=( const T c );
		Vector2<T>&	operator-=( const T c );
		Vector2<T>&	operator/=( const T c );
		Vector2<T>&	operator*=( const T c );

		bool		operator==( const Vector2<T> &v ) const;
		bool		operator!=( const Vector2<T> &v ) const;

		T			length( void ) const;
		T			lengthSqr( void ) const;
		T			normalize( void );		// returns length
		void		clamp( const Vector2<T> &min, const Vector2<T> &max );

		int			dimension( void ) const;

        bool        isEqual( const Vector2<T> & other, T epsilon ) const;

		const T*	ptr( void ) const;
		T*			ptr( void );

		void		mix( const Vector2<T> &v1, const Vector2<T> &v2, float alpha );

        String      toString( void ) const;

        static Vector2<T> fromString( const String & s );

		T x, y;
	};

    template<typename T>
	inline Vector2<T>::Vector2()
	{
	}

    template<typename T>
	inline Vector2<T>::Vector2( const T x, const T y )
	{
	    this->x = x;
	    this->y = y;
	}

    template<typename T>
	inline Vector2<T>::Vector2( const Vector2<T>& v )
	{
	    x = v.x;
	    y = v.y;
	}

	template<typename T>
	inline Vector2<T>::Vector2( const Vector3<T>& v )
	{
		T invz = 1.0 / v.z;
	    x = invz * v.x;
	    y = invz * v.y;
	}

    template<typename T>
	inline void Vector2<T>::set( T x, T y )
	{
	    this->x = x;
	    this->y = y;
	}

    template<typename T>
	inline void Vector2<T>::setZero( )
	{
	    x = 0;
	    y = 0;
	}

    template<typename T>
	inline T Vector2<T>::operator[]( int index ) const
	{
	    return ( &x )[ index ];
	}

    template<typename T>
	inline T& Vector2<T>::operator[]( int index )
	{
	    return ( &x )[ index ];
	}

    template<typename T>
	inline Vector2<T> Vector2<T>::operator-() const
	{
	    return Vector2<T>( -x, -y );
	}

    template<typename T>
	inline Vector2<T> operator*( float c, const Vector2<T> &v )
	{
	    return Vector2<T>( c * v.x, c * v.y );
	}

    template<typename T>
	inline Vector2<T> Vector2<T>::operator*( const T c ) const
	{
	    return Vector2<T>( x * c, y * c );
	}

    template<typename T>
	inline Vector2<T> Vector2<T>::cmul( const Vector2<T>& v ) const
	{
	    return Vector2<T>( v.x * x, v.y * y );
	}

    template<typename T>
	inline T Vector2<T>::operator*( const Vector2<T>& v ) const
	{
	    return v.x * x + v.y * y;
	}

    template<typename T>
	inline Vector2<T> Vector2<T>::operator/( const T c ) const
	{
	    T inv = ( ( T ) 1 ) / c;
	    return Vector2<T>( x * inv, y * inv );
	}

    template<typename T>
	inline Vector2<T> Vector2<T>::operator+( const Vector2<T> &v ) const
	{
	    return Vector2<T>( x + v.x, y + v.y );
	}

    template<typename T>
	inline Vector2<T> Vector2<T>::operator-( const Vector2<T> &v ) const
	{
	    return Vector2<T>( x - v.x, y - v.y );
	}

    template<typename T>
	inline Vector2<T>& Vector2<T>::operator+=( const Vector2<T> &v )
	{
	    x += v.x;
	    y += v.y;
	    return *this;
	}

    template<typename T>
	inline Vector2<T>& Vector2<T>::operator-=( const Vector2<T> &v )
	{
	    x -= v.x;
	    y -= v.y;
	    return *this;
	}

    template<typename T>
	inline Vector2<T>& Vector2<T>::operator/=( const Vector2<T> &v )
	{
	    x /= v.x;
	    y /= v.y;
	    return *this;
	}

    template<typename T>
	inline Vector2<T>& Vector2<T>::operator+=( const T c )
	{
	    x += c;
	    y += c;
	    return *this;
	}

    template<typename T>
	inline Vector2<T>& Vector2<T>::operator-=( const T c )
	{
	    x -= c;
	    y -= c;
	    return *this;
	}


    template<typename T>
	inline Vector2<T>& Vector2<T>::operator/=( const T c )
	{
	    x /= c;
	    y /= c;
	    return *this;
	}

    template<typename T>
	inline Vector2<T>& Vector2<T>::operator*=( const T c )
	{
	    x *= c;
	    y *= c;
	    return *this;
	}

	template<>
	inline bool Vector2<float>::operator==( const Vector2<float> &v ) const
	{
		return Math::abs( v.x - x ) < Math::EPSILONF && Math::abs( v.y - y ) < Math::EPSILONF;
	}

	template<>
	inline bool Vector2<float>::operator!=( const Vector2<float> &v ) const
	{
		return Math::abs( v.x - x ) > Math::EPSILONF || Math::abs( v.y - y ) > Math::EPSILONF;
	}

	template<>
	inline bool Vector2<double>::operator==( const Vector2<double> &v ) const
	{
		return Math::abs( v.x - x ) < Math::EPSILOND && Math::abs( v.y - y ) < Math::EPSILOND;
	}

	template<>
	inline bool Vector2<double>::operator!=( const Vector2<double> &v ) const
	{
		return Math::abs( v.x - x ) > Math::EPSILOND || Math::abs( v.y - y ) > Math::EPSILOND;
	}


    template<typename T>
	inline T Vector2<T>::length( void ) const
	{
	    return Math::sqrt( x * x + y * y );
	}

    template<typename T>
	inline T Vector2<T>::lengthSqr( void ) const
	{
	    return x * x + y * y;
	}

    template<typename T>
	T Vector2<T>::normalize( void )
	{
	    T lenSqr, lenInv;

	    lenSqr = x * x + y * y;
	    lenInv = Math::invSqrt( lenSqr );
	    x *= lenInv;
	    y *= lenInv;
	    return lenSqr * lenInv;
	}

    template<typename T>
	void Vector2<T>::clamp( const Vector2<T> &min, const Vector2<T> &max )
	{
	    x = Math::clamp( x, min.x, max.x );
	    y = Math::clamp( y, min.y, max.y );
	}

    template<typename T>
	int Vector2<T>::dimension( void ) const
	{
	    return 2;
	}

    template <typename T>
    bool Vector2<T>::isEqual( const Vector2<T>& other, T epsilon ) const
    {
        return ( Math::abs( x - other.x ) < epsilon ) &&
               ( Math::abs( y - other.y ) < epsilon );
    }

    template<typename T>
	const T* Vector2<T>::ptr( void ) const
	{
	    return &x;
	}

    template<typename T>
	T* Vector2<T>::ptr( void )
	{
	    return &x;
	}

    template<typename T>
	void Vector2<T>::mix( const Vector2<T> &v1, const Vector2<T> &v2, float alpha )
	{
	    x = Math::mix( v1.x, v2.x, alpha );
	    y = Math::mix( v1.y, v2.y, alpha );
	}

    template<typename T>
	String Vector2<T>::toString( void ) const
	{
        String s;

        s += x; s+= " ";
        s += y; s+= " ";

	    return s;
	}


	template<typename T>
	std::ostream& operator<<( std::ostream& out, const Vector2<T> &v )
	{
		out << " | " << v.x << " " << v.y << " | ";
		return out;
	}

}

#endif
