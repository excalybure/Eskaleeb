#include "stdafx.h"
#include "Lexer.h"

namespace Yal
{
	namespace Lexer
	{
		static void skipSpaces( std::string::const_iterator &it, const std::string::const_iterator &end )
		{
			while ( it != end && isspace( *it ) )
				++it;
		}

		static void skipComments( std::string::const_iterator &it, const std::string::const_iterator &end )
		{
			skipSpaces( it, end );
			while ( std::distance( it, end ) >= 2 && it[0] == '/' && it[1] == '/' )
			{
				it += 2;
				while ( it != end && *it != '\n' )
					++it;
				skipSpaces( it, end );
			}
		}


		static bool isLiteralCharacter( const char c )
		{
			return isalnum( c ) != 0 || c == '_';
		}

		std::string ParseToken( std::string::const_iterator &it, const std::string::const_iterator &end )
		{
			skipComments( it, end );
			if ( it == end )
				return std::string();

			auto start = it;
			it++;

			if ( !isLiteralCharacter( *start ) )
				return std::string( start, it );

			bool isNumber = isdigit( *start ) != 0;

			if ( isNumber )
			{
				bool isFloat = false;
				while ( it != end && ( isdigit( *it ) || *it == '.' ) )
				{
					if ( *it == '.' )
					{
						if ( isFloat )
							throw std::exception( "Malformed number encountered" );
						isFloat = true;
					}
					++it;
				}
				if ( it != end && *it == 'f' )
					++it;
				if ( it != end && isalnum( *it ) )
					throw std::exception( "Malformed number encountered" );
			}
			else
			{
				while ( it != end && isLiteralCharacter( *it ) )
					++it;
			}

			return std::string( start, it );
		}
	}
}