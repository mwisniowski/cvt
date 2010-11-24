#include <cvt/util/Signal.h>

#include <iostream>

using namespace cvt;

static void func( int x )
{
	std::cout << "BLA " << x << std::endl;
}


static void funcX( int x )
{
	std::cout << "BLUB " << x << std::endl;
}

static void funcY()
{
	std::cout << "i am void " << std::endl;
}

int main()
{
	Signal<int> a;
	Delegate<void (int)> d( &func );
	Delegate<void (int)> d2( &funcX );

	Signal<void> b;
	Delegate<void ()> xx( &funcY );
	b.add( &xx );
	b.notify();

	a.add( &d );
	a.add( &d2 );
	a.notify( 10 );

}
