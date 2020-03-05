#include "test_util.h"

namespace tests {

    void fail(const char *const message) {
        std::cerr << message << std::endl;
    }

    void assert_true(const bool actual, const char *const &file, const size_t line) {
        if (!actual) std::cerr << "Expected:\n\ttrue\nActual:\n\tfalse\nat " << file << ':' << line << std::endl;
    }

    void assert_false(const bool actual, const char *const &file, const size_t line) {
        if (actual) std::cerr << "Expected:\n\tfalse\nActual:\n\ttrue\nat " << file << ':' << line << std::endl;
    }
}
