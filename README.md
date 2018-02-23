# PopQuiz
![Linux](https://travis-ci.org/garrettsickles/PopQuiz.svg?branch=master "Linux")

Unit test in C++: Simple, Header-Only, Cross-Platform.

## Overview
I considered the following when I wrote this library:
 - Unit tests should be simple and so should unit testing.
 - There are too many frameworks and most of them have too many features.

That means **PopQuiz**:
 - Does not link against any external libraries (unlike `gtest`)
 - Does not link against other compiled `*.cpp` files
 - Compiles to its own binary executable per unit test file (`*.unit_test` or `*.unit_test.exe`)
 - Can only assert true, false, equals, or throw (uncaught exceptions are considered failures)
 - Can ignore certain units test at user discretion
 - Can output test results to **JSON** file per unit test

## Macros
Macros play an important role in configuring each PopQuiz unit test.


`POPQUIZ_DISABLE_STDOUT`
- Disable output to the console


`POPQUIZ_TEST_ALL`
- Return true even if a test fails


`POPQUIZ_JSON_OUTPUT`
- Set to the path of the **JSON** results file
- e.g. (`-DPOPQUIZ_JSON_OUTPUT="UnitTestOutput.json"`) 

## Example

`Example.unit_test.cpp`

```cpp
// Only need to include this file
// No linkage needed
#include <PopQuiz.hpp>

#include <thread>

// Must define the 'void PopQuiz::Setup()' function
void PopQuiz::Setup() {
    
    // Add a unit test in suite 'Example #1' named 'Do Nothing'
    AddTest("Example #1", "Do Nothing", [](void) {
        // Pass by default
    });

    // Add a unit test in suite 'Example #1' named 'Assert True'
    AddTest("Example #1", "Assert True", [](void) {
        // Assert 1 == 1 with the message 'One should equal one.'
        AssertEqual(1, 1, "One should equal one.");
    });
    
    // Add a unit test in suite 'Example #1' named 'Expect Exception'
    AddTest("Example #1", "Expect Exception", [](void) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        // Assert that we throw a 'std::runtime_error'
        AssertThrow<std::runtime_error>([](){
            throw std::runtime_error("This is a runtime error.");
        });
    //     Passing '5' tell PopQuiz that this test should not exceed
    //     5 millisecond. If the test execution does exceed 5 ms
    //     then the test will be considered a failure.
    },     5);
    
    // Add a unit test in suite 'Example #1' named 'Throw Exception'
    //     Templating 'AddTest' as 'false' will run the test but
    //     ignore the result so all other tests will run. This
    //     allows you to add a test knowing it is broken but
    //     still include it in your test suite. This test will
    //     not affect the return code of the unit test suite if
    //     set to 'false'.
    AddTest<false>("Example #1", "Throw Exception", [](void) {
        // Assert 1 == 2 with the message 'One should not equal two.'
        AssertEqual(1, 2, "One should not equal two."); 
    });
}
```

**Windows (Visual Studio)**

- See `example` folder. Project file provided.

**Linux, MacOS**

```bash
# Change <POPQUIZ_PATH> to the directory containing 'PopQuiz.hpp'
g++ -std=c++11 Example.unit_test.cpp -I<POPQUIZ_PATH> -DPOPQUIZ_OUTPUT_STDOUT -DPOPQUIZ_OUTPUT_JSON="test.json" -o Example.unit_test
./Example.unit_test
```

**Console Output**
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
