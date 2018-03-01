#include <PopQuiz.hpp>

void PopQuiz::Setup() {

    AddTest<false>("Example #2", "Ignore", [](void) {

    });

    AddTest("Example #2", "Do nothing", [](void) {
        
    });

    AddTest<false>("Example #2", "Throw Exception and Ignore", [](void) {
        AssertEqual("Pi", "pi", "Almost Equal!");
    });
}