#ifndef SEM_2_LAB_1_TEST_UTIL_H
#define SEM_2_LAB_1_TEST_UTIL_H


#include <iostream>
#include <stdexcept>
#include <optional>

#define ASSERT_EQUALS(expected, actual) tests::assert_equals(expected, actual, __FILE__, __LINE__);

#define ASSERT_NOT_EQUALS(expected, actual) tests::assert_not_equals(expected, actual, __FILE__, __LINE__);

#define ASSERT_TRUE(expected) tests::assert_true(expected, __FILE__, __LINE__);

#define ASSERT_FALSE(expected) tests::assert_false(expected, __FILE__, __LINE__);

#define ASSERT_OPTIONAL_EQUALS(expected, actual) tests::assert_optional_equals(expected, actual, __FILE__, __LINE__);

#define ASSERT_OPTIONAL_EMPTY(actual) tests::assert_optional_empty(actual, __FILE__, __LINE__);

#define ASSERT_THROWS(expression, exception_type)\
try {\
    expression;\
    tests::fail("Expression `"#expression"` didn't throw expected `"#exception_type"`");\
} catch (const exception_type &e) {}

#define RUN_TEST(test_function)\
std::cout << "Running test <" << #test_function << '>' << std::endl;\
try {\
    test_function;\
    std::cout << "Test <" << #test_function << " ended successfully" << std::endl;\
} catch (...) {\
    std::cout << "<!> Test <" << #test_function << " ended exceptionally" << std::endl;\
}

namespace tests {

    void fail(const char *message);

    template<typename TExpected, typename TActual>
    void assert_equals(const TExpected &expected, const TActual &actual,
                       const char *const &file, const size_t line) {
        if (!(expected == actual)) std::cerr << "Expected:\n\t" << expected
                << "\nActual:\n\t" << actual
                << "\nat " << file << ':' << line << std::endl;
    }

    template<typename TExpected, typename TActual>
    void assert_not_equals(const TExpected &expected, const TActual &actual,
                           const char *const &file, const size_t line) {
        if (!(expected != actual)) std::cerr << "Expected:\n\t" << expected
                << "\nActual:\n\t" << actual
                << "\nat " << file << ':' << line << std::endl;
    }

    template<typename TExpected, typename TActual>
    void assert_optional_equals(const TExpected &expected, const std::optional<TActual> &actual,
                                const char *const &file, const size_t line) {
        if (actual.has_value()) {
            const auto value = actual.value();
            if (!(value == expected)) std::cerr << "Expected:\n\toptional{" << expected
                    << "}\nActual:\n\toptional{" << value
                    << "}\nat " << file << ':' << line << std::endl;
        } else std::cerr << "Expected:\n\toptional{" << expected
                << "}\nActual:\n\tEmpty optional\nat " << file << ':' << line << std::endl;
    }

    template<typename TActual>
    void assert_optional_empty(const std::optional<TActual> &actual,
                               const char *const &file, const size_t line) {
        if (actual.has_value()) std::cerr << "Expected:\n\tEmpty optional"
                << "\nActual:\n\toptional{" << actual.value()
                << "}\nat " << file << ':' << line << std::endl;
    }

    void assert_true(bool actual, const char *const &file, size_t line);

    void assert_false(bool actual, const char *const &file, size_t line);
}

#endif //SEM_2_LAB_1_TEST_UTIL_H
