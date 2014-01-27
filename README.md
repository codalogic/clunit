clunit
======

cl::clunit, is a simple C++ unit testing framework.  It's USP is that it
supports to-do statements so that you can easily record tests that you plan
to do, but haven't implemented yet.  This allows you to plan ahead, but
still only work on fixing one test at a time.

A test function is created using the TFEATURE( "A descriptive string" )
macro.  This represents the beginning of a test function that is
automagically registered for execution when the tests are initiated.

For example:

example-test.cpp:
```cpp
    #include "clunit.h"

    TFEATURE( "Example tests" )         // Register test with descriptive name
    {
        TDOC( "Test description" );     // Add any documentation (anywhere in function)
        TSETUP( int t=1 );              // Do any lines needed to setup a test
        int b=1;                        // Use of TSETUP for test setup is optional
        TTODO( "Need todo this" );      // Log any tests that need to be done
        TTODON( 2, "Need todo this" );  // As above but with a depth indicator (i.e. 2) to help prioritise work
        TTODOX( t == b );               // Log a todo that is compilable but not trying to pass yet
        TTODOXN( 2, t == b );           // A version of TTODOX() with a depth indicator
        TDOC( "More description" );
        TTEST( 1 != 0 );                // Run a test
        TTESTN( 2, 1 != 0 );            // A version of TTEST() to mirror TTODOXN()
        TCRITICALTEST( 1 == 1 );        // Return from function immediately if test fails
        TCRITICALTESTN( 2, 1 == 1 );    // Version of TCRITICALTEST() with depth indicator
        TTESTFAIL( 1 != 0 );            // Fail a test that should pass - to check it's being run
        TTESTNFAIL( 1, 1 != 0 );        // TTESTN variant of TTFAIL()
        TCALL( func( 12, "y", "y" ) );  // Call a test function
    }
```
A note can be made of features that need to be tested using the TFEATTODO()
macros.  These effectively create a test function containing a single
TTODO() test description macro.

For example (not in a function):

```cpp
    TFEATTODO( "Feature Todo" );        // A quick way of recording a feature that needs testing

    TFEATTODON( 2, "Feature N Todo" );  // As above but with a depth indicator
```

The tests are initiated by calling the TRUNALL(); macro in the test
program's main() function.  Before doing #include "clunit.h" in the test
file containing main(), be sure to #define CLUNIT_HOME first.

For example:

main-test.cpp:
```cpp
    #define CLUNIT_HOME
    #include "clunit.h"

    int main()
    {
        TRUNALL();  // Run registered tests and print final pass/fail result
    }
```

By default the test output goes to "clunit.out". If you wish to direct
the test output to a different file, do:
     #define CLUNIT_OUT "tout.out"
or similar.

See clunit.h for further description of how to use the code.
