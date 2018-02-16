#include <PopQuiz.hpp>

void PopQuiz::Setup()
{
	PopQuiz::OutputConsole(false);
	PopQuiz::OutputJSON("ExampleOne.unit_test.json");

    PopQuiz::AddTest("Example #1", "Do Nothing", [](void) {

    });

    PopQuiz::AddTest("Example #1", "Assert True", [](void) {
        PopQuiz::AssertEqual(1, 1, "Equal, Yay!");
    });

    PopQuiz::AddTest("Example #1", "Throw Exception", [](void) {
        PopQuiz::AssertEqual(1, 2, "Not equal, duh...");
    }, false);
}