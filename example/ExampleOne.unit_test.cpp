#include <PopQuiz.hpp>

void PopQuiz::Setup() {

    OutputJSON("ExampleOne.unit_test.json");

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
    AddTest("Example #1", "Expect Exception", [](void) {
        // Assert that we throw a 'std::runtime_error'
        AssertThrow<std::runtime_error>([](){
            throw std::runtime_error("This is an error");
        }); 
    }, false);
    
    // Add a unit test in suite 'Example #1' named 'Throw Exception'
    AddTest("Example #1", "Throw Exception", [](void) {
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