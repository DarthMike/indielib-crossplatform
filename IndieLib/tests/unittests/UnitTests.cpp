/*********************************** The zlib License ************************************
 *
 * Copyright (c) 2013 Indielib-crossplatform Development Team
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 *
 *****************************************************************************************/

/*  FRON UNITESTS++ DOCUMENTATION
Simple check macros

In test cases, we want to check the results of our system under test. UnitTest++ provides a number of check macros that handle comparison and proper failure reporting.

The most basic variety is the boolean CHECK macro:

  CHECK(false); // fails

It will fail if the boolean expression evaluates to false.

For equality checks, it's generally better to use CHECK_EQUAL:

  CHECK_EQUAL(10, 20); // fails
  CHECK_EQUAL("foo", "bar"); // fails

Note how CHECK_EQUAL is overloaded for C strings, so you don't have to resort to strcmp or similar. There is no facility for case-insensitive comparison or string searches, so you may have to drop down to a plain boolean CHECK with help from the CRT:

  CHECK(std::strstr("zaza", "az") != 0); // succeeds

For floating-point comparison, equality isn't necessarily well-defined, so you should prefer the CHECK_CLOSE macro:

  CHECK_CLOSE(3.14, 3.1415, 0.01); // succeeds

All of the macros are tailored to avoid unintended side-effects, for example:

  TEST(CheckMacrosHaveNoSideEffects)
  {
    int i = 4;
    CHECK_EQUAL(5, ++i); // succeeds
    CHECK_EQUAL(5, i); // succeeds
  }

The check macros guarantee that the ++i expression isn't repeated internally, as demonstrated above.
Array check macros

There is a set of check macros for array comparison as well:

  const float oned[2] = { 10, 20 };
  CHECK_ARRAY_EQUAL(oned, oned, 2); // succeeds
  CHECK_ARRAY_CLOSE(oned, oned, 2, 0.00); // succeeds

  const float twod[2][3] = { {0, 1, 2}, {2, 3, 4} };
  CHECK_ARRAY2D_CLOSE(twod, twod, 2, 3, 0.00); // succeeds

The array equal macro compares elements using operator==, so CHECK_ARRAY_EQUAL won't work for an array of C strings, for example.

The array close macros are similar to the regular CHECK_CLOSE macro, and are really only useful for scalar types, that can be compared in terms of a difference between two array elements.

Note that the one-dimensional array macros work for std::vector as well, as it can be indexed just as a C array.
Exception check macros

Finally, there's a CHECK_THROW macro, which asserts that its enclosed expression throws the specified type:

  struct TestException {};
  CHECK_THROW(throw TestException(), TestException); // succeeds

UnitTest++ natively catches exceptions if your test code doesn't. So if your code under test throws any exception UnitTest++ will fail the test and report either using the what() method for std::exception derivatives or just a plain message for unknown exception types.

Should your test or code raise an irrecoverable error (an Access Violation on Win32, for example, or a signal on Linux), UnitTest++ will attempt to map them to an exception and fail the test, just as for other unhandled exceptions.
Time constraints

UnitTest++ can fail a test if it takes too long to complete, using so-called time constraints.

They come in two flavors; local and global time constraints.

Local time constraints are limited to the current scope, like so:

  TEST(YourTimedTest)
  {
     // Lengthy setup...

     {
        UNITTEST_TIME_CONSTRAINT(50);

        // Do time-critical stuff
     }

     // Lengthy teardown...
  }

The test will fail if the "Do time-critical stuff" block takes longer than 50 ms to complete. The time-consuming setup and teardown are not measured, since the time constraint is scope-bound. It's perfectly valid to have multiple local time constraints in the same test, as long as there is only one per block.

A global time constraint, on the other hand, requires that all of the tests in a test run are faster than a specified amount of time. This allows you, when you run a suite of tests, to ask UnitTest++ to fail it entirely if any test exceeds the global constraint. The max time is passed as a parameter to an overload of RunAllTests().

If you want to use a global time constraint, but have one test that is notoriously slow, you can exempt it from inspection by using the UNITTEST_TIME_CONSTRAINT_EXEMPT macro anywhere inside the test body.

  TEST(NotoriouslySlowTest)
  {
     UNITTEST_TIME_CONSTRAINT_EXEMPT();

     // Oh boy, this is going to take a while
     ...
  }

*/

#include "dependencies/unittest++/src/UnitTest++.h"
#include "WorkingPath.h"
#include "IndiePlatforms.h"

#if defined(PLATFORM_LINUX)
#include <stdio.h>
#endif


int main() {
	//Just run all tests
    printf("PERFORMING UNIT TESTS...\n");
    const char* resourcesDir = WorkingPathSetup::resourcesDirectory();
    WorkingPathSetup::setWorkingPath(resourcesDir);
    
	return UnitTest::RunAllTests();
}

