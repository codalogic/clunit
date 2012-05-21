//----------------------------------------------------------------------
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
//----------------------------------------------------------------------

#ifndef CLUNIT
#define CLUNIT

#include <iostream>
#include <fstream>
#include <strstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <crtdbg.h>

#define OutputDebugString  OutputDebugStringA
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA( const char * lpOutputString );

namespace cl {

class fixed_size_log
{
	// We want to log TODO operations while carrying out tests, but also
	// check for memory leaks.  Therefore memory for logging can't be
	// allocated during the testing process.  Therefore this class allocates
	// a chunk of memory up front and we log into that until it is full.
private:
	std::string log;
	size_t n_items_logged;
	bool is_log_full;

public:
	fixed_size_log( size_t size )
		:
		n_items_logged( 0 ),
		is_log_full( false )
	{
		log.reserve( size );
	}
	void insert( const std::string & new_entry )
	{
		++n_items_logged;
		if( ! is_log_full )
		{
			if( log.size() + new_entry.size() < log.capacity() )
				log += new_entry;
			else
				is_log_full = true;
		}
	}
	const std::string & get() const { return log; }
	size_t size() const { return n_items_logged; }
	bool empty() const { return  n_items_logged == 0; }
};

#define TBEGIN( x ) cl::clunit::tbegin( x, __FILE__ )
#define TDOC( x ) cl::clunit::tdoc( x )
#define TSETUP( x ) cl::clunit::tsetup_log( #x ); x
#define TTODO( x ) cl::clunit::ttodo( x, __FILE__, __LINE__ )
#define TTODOX( x ) { bool t=(x); cl::clunit::ttodox( #x, t, __FILE__, __LINE__ ); }
#define TTEST( x ) { bool t=(x); cl::clunit::ttest( #x, t, __FILE__, __LINE__ ); }
#define TRUNALL() { cl::clunit::run(); size_t n_errors = cl::clunit::report(); if( n_errors > 255 ) return 255; return n_errors; }

typedef void(*job_func_ptr)();
typedef std::vector< job_func_ptr > job_list;

class clunit
{
private:
	class singleton
	{
	private:
		bool is_first;
		int n_tests;
		int n_errors;
		fixed_size_log todo_log;

		job_list & get_jobs();
		std::ostream & tout();
	
	public:
		singleton()
			:
			is_first( false ),
			n_tests( 0 ),
			n_errors( 0 ),
			todo_log( 10000 )
		{}

		void add_job( job_func_ptr job )
		{
			get_jobs().push_back( job );
		}
		void tbegin( const char * what, const char * file )
		{
			tout() << "\n\n    " << what << " (" << file << ")\n    ==========================\n";
			OutputDebugString( (std::string( what ) + "\n").c_str() );
		}
		void tdoc( const char * what )
		{
			tout() << "      " << what << "\n";
		}
		void tsetup_log( const char * what )
		{
			tout() << "      : " << what << '\n';
		}
		void ttodo( const char * what, const char * file, int line )
		{
			std::ostringstream report;
			report << "- " << what << " [" << file << ":" << line << "]\n";
			todo_log.insert( report.str() );
		}
		void ttodox( const char * what, bool is_passed, const char * file, int line )
		{
			std::ostringstream report;
			report << "- " << what << ((is_passed)?" (passing)":" (failing)") << " [" << file << ":" << line << "]\n";
			todo_log.insert( report.str() );
		}
		void ttest( const char * what, bool is_passed, const char * file, int line )
		{
			if( ! is_passed )
			{
				tout() << "not ";
				++n_errors;
			}
			else
			{
				tout() << "    ";
			} 
			++n_tests;
			tout() << "ok: " << what;
			if( ! is_passed ) 
				tout() << " (" << __LINE__ << ")";
			tout() << "\n";
		}
		void run()
		{
			{
			// The iostream (and possibly string) functions dynamically allocate memory
			// -the first time they are called.  They are not cleared until the program
			// -ends.  So that these allocations do not muck up the heap checking stats,
			// -dummy uses of the libraries are made so that they are initialised.  We
			// -can then checkpoint the heap after this point.
			std::ostrstream t1;
			t1 << "" << 12;
			std::ostringstream t2;
			t2 << "" << 12;
			tout() << "";
			}

			for( job_list::const_iterator task( get_jobs().begin() ), task_end( get_jobs().end() );
					task != task_end;
					++task )
			{ 
				_CrtMemState s1, s2, s3;
				// Store a memory checkpoint in the s1 memory-state structure
				_CrtMemCheckpoint( &s1 );

				try
				{
					(*task)(); 
				}
				catch(...)
				{
					TTEST( "Unhandled exception" == NULL );		// Force fail case
				}

				// Store a 2nd memory checkpoint in s2
				_CrtMemCheckpoint( &s2 );
				TTEST( ! _CrtMemDifference( &s3, &s1, &s2 ) );
				if ( _CrtMemDifference( &s3, &s1, &s2 ) )
				{
					_CrtMemDumpStatistics( &s3 );
					_CrtMemDumpAllObjectsSince( &s1 );
				}
			}

			TTEST( _CrtCheckMemory() != 0 );
		}
		size_t report()
		{
			if( ! todo_log.empty() )
			{
				std::ostringstream todo_report;
				todo_report << "TODOs (" << todo_log.size() << "):\n------------------------\n" << todo_log.get();
				tout() << todo_report.str();
				OutputDebugString( todo_report.str().c_str() );
			}
			std::ostringstream summary;
			summary << n_errors << " error(s), " << todo_log.size() << " todo(s) after " << n_tests << " test(s)\n";
			tout() << summary.str();
			std::cout << summary.str();
			OutputDebugString( summary.str().c_str() );
			return n_errors;
		}	
		void clear() { get_jobs().clear(); }
	};
	
	static singleton my_singleton;

public:
	clunit( job_func_ptr job ) { my_singleton.add_job( job ); }
	static void tbegin( const char * what, const char * file ) { my_singleton.tbegin( what, file ); }
	static void tdoc( const char * what ) { my_singleton.tdoc( what ); }
	static void tsetup_log( const char * what ) { my_singleton.tsetup_log( what ); }
	static void ttodo( const char * what, const char * file, int line ) { my_singleton.ttodo( what, file, line ); }
	static void ttodox( const char * what, bool is_passed, const char * file, int line ) { my_singleton.ttodox( what, is_passed, file, line ); }
	static void ttest( const char * what, bool is_passed, const char * file, int line ) { my_singleton.ttest( what, is_passed, file, line ); }
	static void run() { my_singleton.run(); }
	static size_t report() { return my_singleton.report(); }
	static void clear() { my_singleton.clear(); }
};

#ifdef CLUNIT_HOME
	clunit::singleton clunit::my_singleton;
	job_list & clunit::singleton::get_jobs()
	{
		static job_list jobs;
		return jobs;
	}
	std::ostream & clunit::singleton::tout()
	{
#ifdef CLUNIT_OUT
		static std::ofstream o_tout( CLUNIT_OUT );
#else
		static std::ofstream o_tout( "clunit.out" );
#endif
		if( is_first )
		{
			time_t t=time(NULL);
			o_tout << ctime(&t) << '\n';
			is_first = false;
		}
		return o_tout;
	}
#endif

// cl::clunit test
// To use cl::clunit, create a function in a test file that has a similar format to the 
// example below.  Then assign the pseudo return value of the function to a global 
// variable.  This will ensure that the test function is called before main is entered. 
// In main, call the TSUMMARY function.
//
// In the main test file, do #define CLUNIT_HOME to ensure the main function is include.
// If you wish to direct the test output to a different file, do:
//		 #define CLUNIT_OUT "tout.out"
// or similar
/* Example:

	void basic_test()
	{
		TBEGIN( "Test set name" );		// Setup testing for function
		TDOC( "Test description" );		// Do any documentation
		TSETUP( strlen( "This\n" ) );	// Do any lines needed to setup a test
		TTODO( "Need todo this" );		// Log any tests that need to be done
		TTODOX( a == b );				// Log a todo that is compilable but not trying to pass yet
		TTEST( 1 != 0 );				// Run tests
		TTEST( 2 == 2 );
		TTEST( (1 == 0) == false );
	}

	static cl::clunit t1( basic_test );	// Ensure basic_test is registered for calling

	void main()
	{
		TRUNALL();						// Print final pass/fail result
	}
*/

} // End of namespace cl

#endif CLUNIT
