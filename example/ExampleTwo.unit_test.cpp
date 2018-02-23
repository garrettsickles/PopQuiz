#include <PopQuiz.hpp>

void PopQuiz::Setup() {

    AddTest("Example #2", "Ignore", [](void) {

    }, false);

    AddTest("Example #2", "Do nothing", [](void) {
        
    });

    AddTest("Example #2", "Throw Exception and Ignore", [](void) {
        AssertEqual("Pi", "pi", "Almost Equal!");
    }, false);
}