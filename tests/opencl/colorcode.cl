__kernel void FlowColorCode( __write_only image2d_t out, __read_only image2d_t gradient )
{
    const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;
    int2 coord;
    float4 grad, color;
    float4 black = ( float4 ) ( 0.0f, 0.0f, 0.0f, 1.0f );
    float4 white = ( float4 ) ( 1.0f, 1.0f, 1.0f, 1.0f );
    float rad, angle, val, frac;
    unsigned int index1, index2;
    float bla;
#define NUMCOLORS 55
	const float4 colorwheel[ NUMCOLORS ] = {
    1.000000f, 0.000000f, 0.000000f, 1.0f ,
    1.000000f, 0.066667f, 0.000000f, 1.0f ,
    1.000000f, 0.133333f, 0.000000f, 1.0f ,
    1.000000f, 0.200000f, 0.000000f, 1.0f ,
    1.000000f, 0.266667f, 0.000000f, 1.0f ,
    1.000000f, 0.333333f, 0.000000f, 1.0f ,
    1.000000f, 0.400000f, 0.000000f, 1.0f ,
    1.000000f, 0.466667f, 0.000000f, 1.0f ,
    1.000000f, 0.533333f, 0.000000f, 1.0f ,
    1.000000f, 0.600000f, 0.000000f, 1.0f ,
    1.000000f, 0.666667f, 0.000000f, 1.0f ,
    1.000000f, 0.733333f, 0.000000f, 1.0f ,
    1.000000f, 0.800000f, 0.000000f, 1.0f ,
    1.000000f, 0.866667f, 0.000000f, 1.0f ,
    1.000000f, 0.933333f, 0.000000f, 1.0f ,
    1.000000f, 1.000000f, 0.000000f, 1.0f ,
    0.833333f, 1.000000f, 0.000000f, 1.0f ,
    0.666667f, 1.000000f, 0.000000f, 1.0f ,
    0.500000f, 1.000000f, 0.000000f, 1.0f ,
    0.333333f, 1.000000f, 0.000000f, 1.0f ,
    0.166667f, 1.000000f, 0.000000f, 1.0f ,
    0.000000f, 1.000000f, 0.000000f, 1.0f ,
    0.000000f, 1.000000f, 0.250000f, 1.0f ,
    0.000000f, 1.000000f, 0.500000f, 1.0f ,
    0.000000f, 1.000000f, 0.750000f, 1.0f ,
    0.000000f, 1.000000f, 1.000000f, 1.0f ,
    0.000000f, 0.909091f, 1.000000f, 1.0f ,
    0.000000f, 0.818182f, 1.000000f, 1.0f ,
    0.000000f, 0.727273f, 1.000000f, 1.0f ,
    0.000000f, 0.636364f, 1.000000f, 1.0f ,
    0.000000f, 0.545455f, 1.000000f, 1.0f ,
    0.000000f, 0.454545f, 1.000000f, 1.0f ,
    0.000000f, 0.363636f, 1.000000f, 1.0f ,
    0.000000f, 0.272727f, 1.000000f, 1.0f ,
    0.000000f, 0.181818f, 1.000000f, 1.0f ,
    0.000000f, 0.090909f, 1.000000f, 1.0f ,
    0.000000f, 0.000000f, 1.000000f, 1.0f ,
    0.076923f, 0.000000f, 1.000000f, 1.0f ,
    0.153846f, 0.000000f, 1.000000f, 1.0f ,
    0.230769f, 0.000000f, 1.000000f, 1.0f ,
    0.307692f, 0.000000f, 1.000000f, 1.0f ,
    0.384615f, 0.000000f, 1.000000f, 1.0f ,
    0.461538f, 0.000000f, 1.000000f, 1.0f ,
    0.538462f, 0.000000f, 1.000000f, 1.0f ,
    0.615385f, 0.000000f, 1.000000f, 1.0f ,
    0.692308f, 0.000000f, 1.000000f, 1.0f ,
    0.769231f, 0.000000f, 1.000000f, 1.0f ,
    0.846154f, 0.000000f, 1.000000f, 1.0f ,
    0.923077f, 0.000000f, 1.000000f, 1.0f ,
    1.000000f, 0.000000f, 1.000000f, 1.0f ,
    1.000000f, 0.000000f, 0.833333f, 1.0f ,
    1.000000f, 0.000000f, 0.666667f, 1.0f ,
    1.000000f, 0.000000f, 0.500000f, 1.0f ,
    1.000000f, 0.000000f, 0.333333f, 1.0f ,
    1.000000f, 0.000000f, 0.166667f, 1.0f  };


    coord.x = get_global_id( 0 );
    coord.y = get_global_id( 1 );
    grad = read_imagef( gradient, sampler, coord );

	coord.x = coord.x * 2;

    rad = length( grad.xy );
    angle = atan2pi( -grad.y, -grad.x );
    val = ( ( angle + 1.0f ) / 2.0f ) *  ( float ) ( NUMCOLORS );
    frac = fract( val, &bla );
    index1 = ( ( unsigned int ) bla ) % NUMCOLORS;
    index2 = ( index1 + 1 ) % NUMCOLORS;
    color = mix( colorwheel[ index1 ], colorwheel[ index2 ], frac  ).zyxw;
    color = mix( white, color, fmin( rad / 2.0f, 1.0f ) );
    write_imagef( out, coord, color );

    rad = length( grad.zw );
    angle = atan2pi( -grad.w, -grad.z );
    val = ( ( angle + 1.0f ) / 2.0f ) *  ( float ) ( NUMCOLORS );
    frac = fract( val, &bla );
    index1 = ( ( unsigned int ) bla ) % NUMCOLORS;
    index2 = ( index1 + 1 ) % NUMCOLORS;
    color = mix( colorwheel[ index1 ], colorwheel[ index2 ], frac  ).zyxw;
    color = mix( white, color, fmin( rad / 2.0f, 1.0f ) );
    write_imagef( out, coord + ( int2 ) ( 1, 0 ), color );
}
