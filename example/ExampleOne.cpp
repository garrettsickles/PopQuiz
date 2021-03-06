#include <PopQuiz.hpp>

#include <thread>

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