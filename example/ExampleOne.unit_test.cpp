#include <PopQuiz.hpp>

void PopQuiz::Setup() {
    
	OutputJSON("ExampleOne.unit_test.json");

    AddTest("Example #1", "Do Nothing", [](void) {

    });

    AddTest("Example #1", "Assert True", [](void) {
        AssertEqual(1, 1, "Equal, Yay!");
    });

    AddTest("Example #1", "Throw Exception", [](void) {
        AssertEqual(1, 2, "Not equal, duh...");
    }, false);

    AddTest("Example #1", "Expect Exception", [](void) {
        AssertThrow<std::runtime_error>([](){
            throw std::runtime_error("Hello!");
        });
    });
}