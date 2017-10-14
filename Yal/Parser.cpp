#include "stdafx.h"
#include "Parser.h"


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

std::string ParseToken( std::string::const_iterator &it, const std::string::const_iterator &end )
{
	skipComments( it, end );
	if ( it == end )
		return std::string();
	
	auto start = it;
	it++;

	if ( *start == ',' )
		return std::string( start, it );

	while ( it != end && ( !isspace( *it ) && *it != ',' ) )
		++it;
	
	return std::string( start, it );
}