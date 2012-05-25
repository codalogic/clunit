//----------------------------------------------------------------------------
// Copyright (c) 2012, Codalogic Ltd (http://www.codalogic.com)
// All rights reserved.
//
// The license for this file is based on the BSD-3-Clause license
// (http://www.opensource.org/licenses/BSD-3-Clause).
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
//
// - Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// - Redistributions in binary form must reproduce the above copyright 
//   notice, this list of conditions and the following disclaimer in 
//   the documentation and/or other materials provided with the 
//   distribution.
// - Neither the name Codalogic Ltd nor the names of its contributors 
//   may be used to endorse or promote products derived from this 
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE.
//----------------------------------------------------------------------------

#include "clunit.h"

void fixed_size_log_tests()
{
	TBEGIN( "Fixed size log tests" );

	cl::fixed_size_log log( 35 );
	TTEST( log.size() == 0 );
	TTEST( log.empty() );
	
	#define SIXTEEN_CHAR_STRING "A 16 byte entry\n"
	#define EIGHT_CHAR_STRING "8 bytes\n"
	#define HUGE_CHAR_STRING "This is a long char string intended to be too long to log in the fixed size log"
	
	log.insert( SIXTEEN_CHAR_STRING );
	TTEST( log.size() == 1 );
	TTEST( ! log.empty() );
	TTEST( log.get() == SIXTEEN_CHAR_STRING );
	
	log.insert( EIGHT_CHAR_STRING );
	TTEST( log.size() == 2 );
	TTEST( log.get() == SIXTEEN_CHAR_STRING EIGHT_CHAR_STRING );
	
	// This will cause log to over fill and the string shouldn't be added
	log.insert( HUGE_CHAR_STRING );
	TTEST( log.size() == 3 );
	TTEST( log.get() == SIXTEEN_CHAR_STRING EIGHT_CHAR_STRING );
	
	// This would fit in the space we've got left in the log, but because 
	// the previous one didn't fit, this one shouldn't be looged either
	log.insert( EIGHT_CHAR_STRING );
	TTEST( log.size() == 4 );
	TTEST( log.get() == SIXTEEN_CHAR_STRING EIGHT_CHAR_STRING );
}

TREGISTER( fixed_size_log_tests );

void todo_tests()
{
	TBEGIN( "todo tests" );

	TTODO( "Master line counting" );
	TTODOX( 1 == 0 );
	TTODO( "Another test" );
}

TREGISTER( todo_tests );

#include "assert.h"	// For ASSERT macro used below

TFUNCTION( test_tests )
{
	TBEGIN( "Test tests" );

	TDOC( "Test description" );
	TSETUP( int t=1 );
	(void)t;	// Avoid gcc warning
	TTEST( 1 != 0 );
	TCRITICALTEST( 1 == 1 );
	// Commented out to avoid tests recording a test fail
	//TCRITICALTEST( 1 == 0 );
	//assert( 0 );	// Shouldn't get here
}
