// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma warning ( disable : 4710 )		// function not inlined
#pragma warning ( disable : 4061 )		// enumerator '...' in switch of enum '..' is not explicitly handled by a case label
#pragma warning ( disable : 4514 )		// nreferenced inline function has been removed
#pragma warning ( disable : 4820 )		// '...' bytes padding added after data member '...'

#pragma once

#pragma warning ( push )
#pragma warning ( disable : 4668 )		// is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
#pragma warning ( disable : 4514 )		// unreferenced inline function has been removed
#pragma warning ( disable : 4820 )		// bytes padding added after data member
#pragma warning ( disable : 4365 )		// conversion from '...' to '...', signed/unsigned mismatch
#pragma warning ( disable : 4571 )		// Informational: catch (...) semantics changed since Visual C++ 7.1; structured exceptions(SEH) are no longer caught
#pragma warning ( disable : 4625 )		// copy constructor was implicitly defined as deleted
#pragma warning ( disable : 4626 )		// assignment operator was implicitly defined as deleted
#pragma warning ( disable : 5026 )		// move constructor was implicitly defined as deleted
#pragma warning ( disable : 5027 )		// move assignment operator was implicitly defined as deleted
#pragma warning ( disable : 4774 )		// format string expected in argument 3 is not a string literal

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <exception>

// TODO: reference additional headers your program requires here
#include <Windows.h>

#pragma warning ( pop )