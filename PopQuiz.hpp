#ifndef _POPQUIZ_POPQUIZ_HPP
#define _POPQUIZ_POPQUIZ_HPP

#include <cstdio>
#include <cstdint>
#include <chrono>
#include <functional>
#include <map>
#include <string>
#include <vector>

namespace PopQuiz {

using Test  = std::function<void(void)>;
using Case  = std::tuple<std::string, Test, bool>;
using Suite = std::vector<Case>;
using List  = std::map<std::string, Suite>;

void Setup();

#define POPQUIZ_SETUP() void PopQuiz::Setup()
#define POPQUIZ_JSON_OUTPUT() do {\
        std::string filename = std::string(__FILE__);\
        filename = filename.substr(0, filename.find_last_of(".")) + ".json";\
        PopQuiz::OutputJSON(filename);\
    } while (0)

List& GetList()
    { static List suite; return suite; }
void AddTest(const std::string suite, const std::string name, const Test& test, const bool use = true)
    { GetList()[suite].push_back(Case(name,test,use)); }

// STDOUT
static bool output_stdout = true;
void OutputConsole(bool out)
    { output_stdout = out; }

// JSON Output
static bool output_json = false;
static std::string output_json_path = "";
void OutputJSON(const std::string& path)
    { output_json_path = path; output_json = output_json_path != ""; }

#if defined(_MSC_VER)
    #if (_MSC_VER <= 1800)
        #define POPQUIZ_NOEXCEPT
    #endif
#else
    #define POPQUIZ_NOEXCEPT noexcept
#endif

class Exception : std::exception {
    std::string _what = "Unknown";
public:
    explicit Exception(const char* const message) { this-> _what = message; }
    explicit Exception(const std::string message) { this-> _what = message; }
    char const* what() const POPQUIZ_NOEXCEPT override { return this->_what.c_str(); }
};

template<class T> void AssertEqual(const T& e, const T& a, const std::string msg)
    { if (e != a) throw Exception(msg); }
template<> void AssertEqual(const std::string& e, const std::string& a, const std::string msg)
    { if (e != a) throw Exception(msg+"("+e+" is not "+a+")"); } }

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

int main() {
    bool return_fail = false;
    PopQuiz::Setup();
    FILE* json = NULL;
    if (PopQuiz::output_json) json = fopen(PopQuiz::output_json_path.c_str(), "w");
    if (json) fprintf(json, "{");
    std::size_t suite_number = 0;
    for (const auto& suite : PopQuiz::GetList()) {
        _POPQUIZ_P_GY(">------------------------------------------\n");
        _POPQUIZ_P_GY(">>> ");
        _POPQUIZ_P_CN("Test Suite: ")
        _POPQUIZ_P_GY("%s\n", suite.first.c_str());
        if (json) {
            if (suite_number > 0) fprintf(json, ",");
            fprintf(json, "\"%s\":[", suite.first.c_str());
        }

        std::size_t test_number    = 0;
        std::size_t test_count     = suite.second.size();
        std::size_t test_ignores   = 0;
        std::size_t test_failures  = 0;
        std::size_t test_succeeds  = 0;
        std::int64_t test_duration = 0;

        std::vector<std::tuple<std::string,bool,std::int64_t>> summary;
        for (const auto test : suite.second) {
            _POPQUIZ_P_GY("\n    >>> ");
            _POPQUIZ_P_CN("Scenario (%lu/%lu):\n", test_number + 1, test_count);
            _POPQUIZ_P_GY("        - %s\n", std::get<0>(test).c_str());

            bool test_success    = true;
            std::string message  = "";
            std::string expected = "";
            std::string actual   = "";

            const bool test_use = std::get<2>(test);
            if (!test_use) test_ignores++;
            const auto begin    = std::chrono::steady_clock::now();
            auto end            = std::chrono::steady_clock::now();
            try {
                std::get<1>(test)();
                test_succeeds++;
                end = std::chrono::steady_clock::now();
            } catch(const PopQuiz::Exception& e){
                end = std::chrono::steady_clock::now();
                message = e.what();
                test_success = false;
                test_failures++;
                return_fail = return_fail || test_use;
            }

            const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
            test_duration += duration;

            _POPQUIZ_P("        - Completed in %ld ms\n", duration);
            _POPQUIZ_P("        - ");

            if (test_success) {
                _POPQUIZ_P_GN("PASS");
            } else {
                _POPQUIZ_P_RD("FAIL (%s)", message.c_str());
            }
            
            if (json) {
                if (test_number > 0) fprintf(json, ",");
                fprintf(json, "{");
                fprintf(json, "\"name\":\"%s\",", std::get<0>(test).c_str());
                fprintf(json, "\"status\":%s,", test_success ? "true" : "false");
                fprintf(json, "\"ignore\":%s,", test_use ? "false" : "true");
                fprintf(json, "\"duration\":%ld", duration);
                if (!test_success) fprintf(json, ",\"message\":\"%s\"", message.c_str());
                fprintf(json, "}");
            }

            _POPQUIZ_P("\n");
            summary.push_back(std::make_tuple(std::get<0>(test), test_success, static_cast<std::int64_t>(duration)));
            test_number++;
        }

        if (test_count > 0) {
            _POPQUIZ_P_GY("\n");
            _POPQUIZ_P_GY(">>> ");
            _POPQUIZ_P_CN("Summary: ");
            _POPQUIZ_P_GY("%s\n", suite.first.c_str())
            
            char   success_stats[100]   = { 0 };
            double success_percent = static_cast<double>(test_succeeds) / static_cast<double>(test_count) * 100.0;
            std::sprintf(success_stats, "%3.2lf%% of tests passed (%lu/%lu)", success_percent, test_succeeds, test_count);
            
            char   ignore_stats[100]   = { 0 };
            double ignore_percent = static_cast<double>(test_ignores) / static_cast<double>(test_count) * 100.0;
            std::sprintf(ignore_stats, "%3.2lf%% of tests ignored (%lu/%lu)", ignore_percent, test_ignores, test_count);

            if (test_succeeds + test_ignores == test_count) {
                _POPQUIZ_P_GN("    - %s\n", success_stats);
            } else {
                _POPQUIZ_P_RD("    - %s\n", success_stats);
            }
            _POPQUIZ_P_GY("    - %s\n", ignore_stats);
            _POPQUIZ_P_GY("\n");

            for (const auto s : summary) {
                _POPQUIZ_P_GY("    - ");
                if (std::get<1>(s)) {
                    _POPQUIZ_P_GN("PASS");
                } else {
                    _POPQUIZ_P_RD  ("FAIL");
                }
                
                _POPQUIZ_P_GY(" (%s - %ld ms)\n", std::get<0>(s).c_str(), std::get<2>(s));
            }
        }

        if (json) {
            if (test_count > 0) fprintf(json, ",");
            fprintf(json, "{");
            fprintf(json, "\"count\":%lu,", test_count);
            fprintf(json, "\"success\":%lu,", test_succeeds);
            fprintf(json, "\"fail\":%lu,", test_failures);
            fprintf(json, "\"ignore\":%lu,", test_ignores);
            fprintf(json, "\"total_duration\":%ld", test_duration);
            fprintf(json, "}]");
        }
        suite_number++;
    }
    _POPQUIZ_P_GY("\n");
    if (json) fprintf(json, "}");
    if (json) fclose(json);
    return (return_fail ? 1 : 0);
}
#endif