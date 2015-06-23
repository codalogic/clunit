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

TFEATURE( "Example tests" )     // Register test with descriptive name
{
    TDOC( "Test description" );     // Add any documentation (anywhere in function)
    TSETUP( int t=1 );              // Do any lines needed to setup a test
    int b=1;                        // Use of TSETUP for test setup is optional
    TTODO( "Need todo this" );      // Log any tests that need to be done
    TTODON( 2, "Need todo this" );  // As above but with a depth indicator (i.e. 2) to help prioritise work
    TTODOX( t == b );               // Log a todo that is compilable but not trying to pass yet
    TTODOXN( 2, t == b );           // As above but with a depth indicator (i.e. 2) to help prioritise work
    TDOC( "More description" );
    TTEST( 1 != 0 );                // Run a test
    TTESTN( 2, 1 != 0 );            // A runable version of TTODOXN()
    TCRITICALTEST( 1 == 1 );        // Return from function immediately if test fails
    TCRITICALTESTN( 2, 1 == 1 );    // Version of TCRITICALTEST() with depth indicator
}

TFEATURE( "Test another feature" )
{
    TTEST( 1==1 );
}

TFUNCTION( another_test )           // Register a test function without description
{
    TBEGIN( "Another test" );       // Manually display description during test run
    TTEST( 1==1 );
}

TFUNCTION( another_test_2 )
{
    TBEGIN( "Another test 2" );
    TTEST( 1==1 );
}

TFEATURETODO( "Feature Todo" );        // A quick way of recording a feature that needs testing

TFEATURETODON( 2, "Feature N Todo" );  // As above but with a depth indicator
