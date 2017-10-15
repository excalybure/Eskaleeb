#pragma once

namespace Yal
{
	template< typename scalar_type > inline scalar_type UnsignedCast( const int64_t value )
	{
		if ( value < 0 )
			throw std::exception( "Trying to assign negative value to positive type" );
		if ( value >= ( static_cast< uint64_t >( 1 ) << sizeof( scalar_type ) * 8 ) )
			throw std::exception( "Value is too^ big and cannot fit into type" );
		return static_cast< scalar_type >( value );
	}

	template< typename scalar_type > inline scalar_type SignedCast( const int64_t value )
	{
		static constexpr int64_t maxValue = static_cast< int64_t >( 1 ) << ( sizeof( scalar_type ) * 8 - 1 );
		if ( value >= maxValue - 1 )
			throw std::exception( "Value is too big and cannot fit into type" );
		if ( value <= -maxValue )
			throw std::exception( "Value is too small and cannot fit into type" );
		return static_cast< scalar_type >( value );
	}

	template< typename scalar_type > scalar_type Cast( const int64_t value );
	template<> inline uint8_t Cast< uint8_t >( const int64_t value ) { return UnsignedCast< uint8_t >( value ); }
	template<> inline uint16_t Cast< uint16_t >( const int64_t value ) { return UnsignedCast< uint16_t >( value ); }
	template<> inline uint32_t Cast< uint32_t >( const int64_t value ) { return UnsignedCast< uint32_t >( value ); }
	template<> inline uint64_t Cast< uint64_t >( const int64_t value )
	{
		if ( value < 0 )
			throw std::exception( "Trying to assign negative value to positive type" );
		return static_cast< uint64_t >( value );
	}

	template<> inline int8_t Cast< int8_t >( const int64_t value ) { return SignedCast< int8_t >( value ); }
	template<> inline int16_t Cast< int16_t >( const int64_t value ) { return SignedCast< int16_t >( value ); }
	template<> inline int32_t Cast< int32_t >( const int64_t value ) { return SignedCast< int32_t >( value ); }
	template<> inline int64_t Cast< int64_t >( const int64_t value ) { return value; }
}