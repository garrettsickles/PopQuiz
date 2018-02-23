#include <cstdio>
#include <cstdint>
#include <chrono>
#include <functional>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#if defined(_MSC_VER)
    #if (_MSC_VER <= 1800)
        #define POPQUIZ_NOEXCEPT
    #endif
#else
    #define POPQUIZ_NOEXCEPT noexcept
#endif

#define _POPQUIZ_STRINGIFY(x) #x
#define _POPQUIZ_STRING(x) _POPQUIZ_STRINGIFY(x)
#define _POPQUIZ_P(...) if (PopQuiz::output_stdout) { std::printf(__VA_ARGS__); }
#if defined (_MSC_VER)
    #include <Windows.h>
    #define _POPQUIZ_C_RD 0x0C
    #define _POPQUIZ_C_GN 0x0A
    #define _POPQUIZ_C_CN 0x0B
    #define _POPQUIZ_C_GY 0x07
    #define _POPQUIZ_C_SET(color) do {\
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);\
        SetConsoleTextAttribute(hConsole, color);\
    } while (0)
#else
    #include <sys/ioctl.h>
    #define _POPQUIZ_C_RD "\033[1m\033[31m"
    #define _POPQUIZ_C_GN "\033[1m\033[32m"
    #define _POPQUIZ_C_CN "\033[1m\033[36m"
    #define _POPQUIZ_C_GY "\033[0m\033[37m"
    #define _POPQUIZ_C_SET(color) _POPQUIZ_P(color)
#endif
#define _POPQUIZ_PC(color,...) _POPQUIZ_C_SET(color);_POPQUIZ_P(__VA_ARGS__);_POPQUIZ_C_SET(_POPQUIZ_C_GY);
#define _POPQUIZ_P_RD(...) _POPQUIZ_PC(_POPQUIZ_C_RD,__VA_ARGS__)
#define _POPQUIZ_P_GN(...) _POPQUIZ_PC(_POPQUIZ_C_GN,__VA_ARGS__)
#define _POPQUIZ_P_CN(...) _POPQUIZ_PC(_POPQUIZ_C_CN,__VA_ARGS__)
#define _POPQUIZ_P_GY(...) _POPQUIZ_PC(_POPQUIZ_C_GY,__VA_ARGS__)

namespace PopQuiz {

static bool output_stdout = false;

using Test  = std::function<void(void)>;
using Case  = std::tuple<std::string, Test, bool, std::int64_t>;
using Suite = std::vector<Case>;
using List  = std::map<std::string, Suite>;

void Setup();

class Failure {
    std::string _what = "PopQuiz Failure";
    std::string _file = "";
    int         _line = -1;
public:
    explicit Failure(const char* const what, const char* const file, const int line) POPQUIZ_NOEXCEPT
        { this-> _what = what; this->_file = file; this->_line = line; }
    explicit Failure(const std::string what, const char* const file, const int line) POPQUIZ_NOEXCEPT
        { this-> _what = what; this->_file = file; this->_line = line; }
    char const* what() const POPQUIZ_NOEXCEPT { return this->_what.c_str(); }
    char const* file() const POPQUIZ_NOEXCEPT { return this->_file.c_str(); }
    int         line() const POPQUIZ_NOEXCEPT { return this->_line; }
};

static List _pq_test_suite;
void AddTest(const std::string suite, const std::string name, const Test& test, const bool use = true, const std::int64_t duration = -1)
    { _pq_test_suite[suite].push_back(Case(name,test,use,duration)); }

#define AssertTrue(arg,message) do {\
        std::string _pq_msg_tmp(message);\
        if (static_cast<bool>(arg) == false) {\
            _POPQUIZ_P_RD("        - %s (%s:%d - AssertTrue)\n", _pq_msg_tmp.c_str(), __FILE__, __LINE__);\
            throw Failure(message,__FILE__,__LINE__);\
        } else {\
            _POPQUIZ_P_GN("        - %s\n", _pq_msg_tmp.c_str());\
        }\
    } while (0)

#define AssertFalse(arg,message) do {\
        std::string _pq_msg_tmp(message);\
        if (static_cast<bool>(arg) == false) {\
            _POPQUIZ_P_RD("        - %s (%s:%d - AssertTrue)\n", _pq_msg_tmp.c_str(), __FILE__, __LINE__);\
            throw Failure(message,__FILE__,__LINE__);\
        } else {\
            _POPQUIZ_P_GN("        - %s\n", _pq_msg_tmp.c_str());\
        }\
    } while (0)

#define AssertEqual(expected,actual,message) do {\
        std::string _pq_msg_tmp(message);\
        if (expected != actual) {\
            _POPQUIZ_P_RD("        - %s (%s:%d - AssertEqual)\n", _pq_msg_tmp.c_str(), __FILE__, __LINE__);\
            throw Failure(message,__FILE__,__LINE__);\
        } else {\
            _POPQUIZ_P_GN("        - %s\n", _pq_msg_tmp.c_str());\
        }\
    } while (0)

template<class T> void AssertThrow(const std::function<void(void)>& test) {
    try { test(); }
    catch (const T& exception) {
        _POPQUIZ_P_GN("        - Exception Caught!\n");
        return;
    } catch(...) { throw; }
}
}

int main() {
    #ifdef POPQUIZ_OUTPUT_STDOUT
        PopQuiz::output_stdout = true;
    #endif
    PopQuiz::Setup();
    bool return_fail   = false;
    #ifdef POPQUIZ_OUTPUT_JSON
        std::stringstream json_stream;
        json_stream << "{";
    #endif
    std::size_t suite_number = 0;
    for (const auto& suite : PopQuiz::_pq_test_suite) {
        _POPQUIZ_P_CN(">>> Test Suite: ")
        _POPQUIZ_P_GY("%s\n", suite.first.c_str());
        #ifdef POPQUIZ_OUTPUT_JSON
            if (suite_number > 0) json_stream << ",";
            json_stream << "\"" << suite.first << "\":[";
        #endif
        std::size_t  test_count    = suite.second.size();
        std::size_t  test_number   = 0;
        std::size_t  test_ignores  = 0;
        std::size_t  test_failures = 0;
        std::size_t  test_succeeds = 0;
        std::int64_t test_duration = 0;
        std::vector<std::tuple<std::string,bool,std::int64_t>> summary;
        for (const auto test : suite.second) {
            _POPQUIZ_P_CN("\n    >>> Scenario (%lu/%lu):", test_number + 1, test_count);
            _POPQUIZ_P_GY(" %s", std::get<0>(test).c_str());
            const bool test_use = std::get<2>(test);
            if (!test_use) {
                test_ignores++;
                _POPQUIZ_P_GY(" (Ignored)");
            }
            _POPQUIZ_P_GY("\n");
            bool test_success    = true;
            std::string actual   = "";
            std::string message  = "";
            std::string expected = "";
            auto end_time        = std::chrono::steady_clock::now();
            auto begin_time      = std::chrono::steady_clock::now();
            try {
                std::get<1>(test)();
                test_succeeds++;
                end_time = std::chrono::steady_clock::now();
            } catch (const PopQuiz::Failure& failure){
                end_time = std::chrono::steady_clock::now();
                message = std::string(failure.what()) + " (" + std::string(failure.file()) + ":" + std::to_string(failure.line()) + ")";
                test_success = false;
                test_failures++;
                return_fail = return_fail || test_use;
            } catch (...) {
                _POPQUIZ_P_RD("!!! Uncaught Exception, Now exiting... !!!");
                return 1;
            }
            const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - begin_time).count();
            test_duration += duration;
            const auto duration_expected = std::get<3>(test);
            if (duration_expected > 0) {
                if (duration <= duration_expected) {
                    _POPQUIZ_P_GN("        Duration (%lld ms <= %lld ms)\n", static_cast<long long>(duration), static_cast<long long>(duration_expected));
                } else {
                    _POPQUIZ_P_RD("        Duration (%lld ms > %lld ms)\n", static_cast<long long>(duration), static_cast<long long>(duration_expected));
                    test_success = false;
                }
            }
            #ifdef POPQUIZ_OUTPUT_JSON
                if (test_number > 0) json_stream << ",";
                json_stream << "{";
                json_stream << "\"name\":\"" << std::get<0>(test) << "\",";
                json_stream << "\"status\":" << (test_success ? "true" : "false") << ",";
                json_stream << "\"ignore\":" << (test_use ? "false" : "true") << ",";
                json_stream << "\"duration\":" << std::to_string(duration);
                if (!test_success) json_stream << ",\"error\":\"" << message << "\"";
                json_stream << "}";
            #endif
            summary.push_back(std::make_tuple(std::get<0>(test), test_success, static_cast<std::int64_t>(duration)));
            test_number++;
        }
        const bool suite_success = test_succeeds + test_ignores > test_count;
        if (test_count > 0) {
            _POPQUIZ_P_GY("\n    ");
            _POPQUIZ_P_CN("Summary:\n");
            char   success_stats[100] = { 0 };
            double success_percent    = static_cast<double>(test_succeeds) / static_cast<double>(test_count) * 100.0;
            std::sprintf(success_stats, "%3.2lf%% of tests passed (%lu/%lu)", success_percent, test_succeeds, test_count);
            char   ignore_stats[100] = { 0 };
            double ignore_percent    = static_cast<double>(test_ignores) / static_cast<double>(test_count) * 100.0;
            std::sprintf(ignore_stats, "%3.2lf%% of tests ignored (%lu/%lu)", ignore_percent, test_ignores, test_count);
            if (suite_success) {
                _POPQUIZ_P_GN("        %s\n", success_stats);
            } else {
                _POPQUIZ_P_RD("        %s\n", success_stats);
            }
            _POPQUIZ_P_GY("        %s\n", ignore_stats);
            _POPQUIZ_P_GY("        Duration %lld ms\n", static_cast<long long>(test_duration));
            _POPQUIZ_P_GY("\n");
            for (const auto s : summary) {
                _POPQUIZ_P_GY("        ");
                if (std::get<1>(s)) {
                    _POPQUIZ_P_GN("PASS");
                } else {
                    _POPQUIZ_P_RD("FAIL");
                }
                _POPQUIZ_P_GY(" (%s)\n", std::get<0>(s).c_str());
            }
            _POPQUIZ_P_GY("\n");
            if (suite_success) {
                _POPQUIZ_P_GN("    SUCCESS");
            } else {
                _POPQUIZ_P_RD("    FAILURE");
            }
            _POPQUIZ_P_GY(" (%s)\n", suite.first.c_str());
        }
        #ifdef POPQUIZ_OUTPUT_JSON
            if (test_count > 0) json_stream << ",";
            json_stream << "{";
            json_stream << "\"count\":" << std::to_string(test_count) << ",";
            json_stream << "\"success\":" << std::to_string(test_succeeds) << ",";
            json_stream << "\"fail\":" << std::to_string(test_failures) << ",";
            json_stream << "\"ignore\":" << std::to_string(test_ignores) << ",";
            json_stream << "\"duration\":" << std::to_string(test_duration) << ",";
            json_stream << "\"result\":" << (suite_success ? "true" : "false");
            json_stream << "}]";
        #endif
        suite_number++;
    }
    _POPQUIZ_P_GY("\n");
    #ifdef POPQUIZ_OUTPUT_JSON
        json_stream <<  "}";
        std::string filename = std::string(_POPQUIZ_STRING(POPQUIZ_OUTPUT_JSON));
        if (filename == "") {
            _POPQUIZ_P_RD("!!! Error empty JSON filename. Please define POPQUIZ_OUTPUT_JSON as filename !!!");
            return 1;
        }
        filename = filename.substr(0, filename.find_last_of(".")) + ".json";
        FILE* json_file = fopen(filename.c_str(), "w");
        if (json_file) {
            fprintf(json_file, "%s", json_stream.str().c_str());
            fclose(json_file);
        } else {
            _POPQUIZ_P_RD("!!! Error writing JSON output \"%s\" !!!", filename.c_str());
        }
    #endif
    #ifdef POPQUIZ_TEST_ALL
        return 0;
    #else
        return (return_fail ? 1 : 0);
    #endif
}
