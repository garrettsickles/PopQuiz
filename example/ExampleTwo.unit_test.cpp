#include <PopQuiz.hpp>

POPQUIZ_SETUP()
{
	POPQUIZ_JSON_OUTPUT();

    PopQuiz::AddTest("Example #2", "Ignore", [](void) {

    }, false);

    PopQuiz::AddTest("Example #2", "Do nothing", [](void) {
        
    });

    PopQuiz::AddTest("Example #2", "Throw Exception and Ignore", [](void) {
        PopQuiz::AssertEqual("Pi", "pi", "Almost Equal!");
    }, false);
}