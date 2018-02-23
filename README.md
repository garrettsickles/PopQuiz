# PopQuiz
![Linux](https://travis-ci.org/garrettsickles/PopQuiz.svg?branch=master "Linux")

Unit test in C++: Simple, Header-Only, Cross-Platform.

## Overview
I considered the following when I wrote this library:
 - Unit tests should be simple and so should unit testing.
 - Unit testing can be painful.
 - There are too many frameworks and most of them have too many features.

That means **PopQuiz**:
 - Does not link against any external libraries (unlike `gtest`)
 - Does not link against other `*.cpp` files
 - Compiles to its own binary executable per unit test file (`*.unit_test` or `*.unit_test.exe`)
 - Can only assert true, false, equals, or throw (uncaught exceptions are considered failures)
 - Can ignore certain units test at user discretion
 - Can output test results to **JSON** file per unit test
 
## Example

`Example.unit_test.cpp`

```cpp
// Only need to include this file
// No linkage needed
#include <PopQuiz.hpp>

// Must define the 'void PopQuiz::Setup()' function
// Can also use `POPQUIZ_SETUP()` macro
void PopQuiz::Setup()
{
    // Disable console output even though the colors are pretty and cross platform :(
    OutputConsole(false);
    
    // Output results to JSON
    // Can also use `POPQUIZ_JSON_OUTPUT()` macro for automatic filename deduction
    OutputJSON("Example.unit_test.json");

    // Add a unit test in suite 'Example #1' named 'Do Nothing'
    AddTest("Example #1", "Do Nothing", [](void) {
        // Pass by default
    });

    // Add a unit test in suite 'Example #1' named 'Assert True'
    AddTest("Example #1", "Assert True", [](void) {
        // Assert 1 == 1 with the message 'Equal, Yay!'
        AssertEqual(1, 1, "Equal, Yay!");
    });
    
    // Add a unit test in suite 'Example #1' named 'Expect Exception'
    AddTest("Example #2", "Expect Exception", [](void) {
        // Assert that we throw a 'std::runtime_error'
        AssertThrow<std::runtime_error>([](){
            throw std::runtime_error("This is an error");
        }); 
    }, false);
    
    // Add a unit test in suite 'Example #1' named 'Throw Exception'
    AddTest("Example #2", "Throw Exception", [](void) {
        // Assert 1 == 2 with the message 'Not equal, duh...'
        AssertEqual(1, 2, "Not equal, duh..."); 
    },
        // Passing 'false' to 'AddTest' after the test lambda
        // will run the test but ignore the result so all other
        // tests will run. This allows you to add a test knowing
        // it is broken but still include it in your test suite.
        // This test will not affect the return code of the unit
        // test suite if set to 'false'.
        false
    );
}
```

**Windows (Visual Studio)**

- See `example` folder. Project file provided.

**Linux, MacOS**

```bash
# Change <POPQUIZ_PATH> to the directory containing 'PopQuiz.hpp'
g++ -std=c++11 Example.unit_test.cpp -I<POPQUIZ_PATH> -o Example.unit_test
./Example.unit_test
```

![Example Result](https://raw.githubusercontent.com/garrettsickles/PopQuiz/master/example/Example.PNG  "Text 1")

**JSON Output**
```json
{
    "Example #1":[
        {
            "name":"Do Nothing",
            "status":true,
            "ignore":false,
            "duration":0
        },
        {
            "name":"Assert True",
            "status":true,
            "ignore":false,
            "duration":0
        },
        {
            "name":"Expect Exception",
            "status":true,
            "ignore":true,
            "duration":0
        },
        {
            "name":"Throw Exception",
            "status":false,
            "ignore":true,
            "duration":0,
            "message":"Not equal, duh..."
        },
        {
            "count":4,
            "success":3,
            "fail":1,
            "ignore":2,
            "total_duration":0
        }
    ]
}
```
