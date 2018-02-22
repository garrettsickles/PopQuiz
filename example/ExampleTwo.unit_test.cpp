#include <PopQuiz.hpp>

POPQUIZ_SETUP()
{
	POPQUIZ_JSON_OUTPUT();

    AddTest("Example #2", "Ignore", [](void) {

    }, false);

    AddTest("Example #2", "Do nothing", [](void) {
        
    });

    AddTest("Example #2", "Throw Exception and Ignore", [](void) {
        AssertEqual("Pi", "pi", "Almost Equal!");
    }, false);
}