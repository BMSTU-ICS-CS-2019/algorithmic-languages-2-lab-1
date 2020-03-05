#include "simple_string.h"
#include "test_util.h"

#include <iostream>
#include <sstream>

using lab::String;

void run_tests();

int main() {
    run_tests();

    return 0;
}

void test_equality() {
    ASSERT_EQUALS(String(""), String(""))
    ASSERT_EQUALS(String("hello"), String("hello"))
    ASSERT_NOT_EQUALS(String("foo"), String("bar"))
}

void test_comparison() {
    ASSERT_TRUE(String("foo") > String("fo"))
    ASSERT_TRUE(String("foo") >= String("fo"))
    ASSERT_TRUE(String("fo") < String("foo"))
    ASSERT_TRUE(String("fo") <= String("foo"))
}

void test_length() {
    ASSERT_EQUALS(static_cast<size_t>(0), String("").length())
    ASSERT_EQUALS(static_cast<size_t>(3), String("foo").length())
    ASSERT_EQUALS(static_cast<size_t>(11), String("Hello world").length())
}

void test_empty() {
    ASSERT_TRUE(String("").empty())
    ASSERT_FALSE(String("hello").empty())
    ASSERT_FALSE(String(" ").empty())
    ASSERT_FALSE(String("     ").empty())
    ASSERT_FALSE(String("\n").empty())
}

void test_append() {
    String string("");
    ASSERT_EQUALS(String(""), string)
    ASSERT_EQUALS(String("foo"), string + String("foo"))
    ASSERT_EQUALS(String(""), string)

    string = String("bar");
    ASSERT_EQUALS(String("bar"), string)
    ASSERT_EQUALS(String("barbaz"), string + String("baz"))
    ASSERT_EQUALS(String("bar"), string)
}

void test_multiply() {
    String string("");
    ASSERT_EQUALS(String(""), string)
    ASSERT_EQUALS(String(""), string * 3)
    ASSERT_EQUALS(String(""), string)

    string = String("bar");
    ASSERT_EQUALS(String("bar"), string)
    ASSERT_EQUALS(String("barbarbarbarbar"), string * 5)
    ASSERT_EQUALS(String("bar"), string)
}

void test_indexed_read() {
    String string("");
    ASSERT_THROWS(string[0], std::out_of_range)
    ASSERT_THROWS(string[1], std::out_of_range)

    string = String("Hello world");
    ASSERT_EQUALS(L'H', string[0])
    ASSERT_EQUALS(L'e', string[1])
    ASSERT_EQUALS(L'd', string[10])
    ASSERT_THROWS(string[11], std::out_of_range)
}

void test_indexed_write() {
    String string("");
    ASSERT_THROWS(string[0] = 'a', std::out_of_range)
    ASSERT_THROWS(string[1] = 'b', std::out_of_range)

    string = String("Hello world");
    ASSERT_EQUALS(L'H', string[0])
    string[0] = '*';
    ASSERT_EQUALS(L'*', string[0])
    string[5] = '*';
    ASSERT_EQUALS(L'*', string[5])
    string[10] = '*';
    ASSERT_EQUALS(L'*', string[10])

    ASSERT_THROWS(string[11] = '!', std::out_of_range)
}

void test_output() {
    std::stringstream out;

    out << "";
    ASSERT_EQUALS(std::string(), out.str())

    out << String("Hello");
    ASSERT_EQUALS(std::string("Hello"), out.str())

    out << String(" ") << "w";
    ASSERT_EQUALS(std::string("Hello w"), out.str())

    out << "orld";
    out << "!";
    ASSERT_EQUALS(std::string("Hello world!"), out.str());
}

void test_index_of() {
    String string;

    ASSERT_OPTIONAL_EQUALS(0, string.index_of(String("")))
    ASSERT_OPTIONAL_EMPTY(string.index_of(L' '))
    ASSERT_OPTIONAL_EMPTY(string.index_of(' '))

    ASSERT_OPTIONAL_EMPTY(string.index_of(String(" ")))
    ASSERT_OPTIONAL_EMPTY(string.index_of(L'h'))
    ASSERT_OPTIONAL_EMPTY(string.index_of('h'))

    ASSERT_OPTIONAL_EMPTY(string.index_of(String("h")))
    ASSERT_OPTIONAL_EMPTY(string.index_of(String("hello")))
    ASSERT_OPTIONAL_EMPTY(string.index_of(String("hello world")))

    string = String("foo bar baz");

    ASSERT_OPTIONAL_EQUALS(0, string.index_of(String("")))
    ASSERT_OPTIONAL_EQUALS(0, string.index_of(String("foo bar baz")))

    ASSERT_OPTIONAL_EQUALS(0, string.index_of(L'f'))
    ASSERT_OPTIONAL_EQUALS(0, string.index_of('f'))
    ASSERT_OPTIONAL_EQUALS(0, string.index_of(String("f")))

    ASSERT_OPTIONAL_EQUALS(1, string.index_of(L'o'))
    ASSERT_OPTIONAL_EQUALS(1, string.index_of('o'))
    ASSERT_OPTIONAL_EQUALS(1, string.index_of(String("o")))

    ASSERT_OPTIONAL_EQUALS(5, string.index_of(L'a'))
    ASSERT_OPTIONAL_EQUALS(5, string.index_of('a'))
    ASSERT_OPTIONAL_EQUALS(5, string.index_of(String("a")))

    ASSERT_OPTIONAL_EQUALS(3, string.index_of(L' '))
    ASSERT_OPTIONAL_EQUALS(3, string.index_of(' '))
    ASSERT_OPTIONAL_EQUALS(3, string.index_of(String(" ")))

    ASSERT_OPTIONAL_EQUALS(8, string.index_of(String("baz")))

    ASSERT_OPTIONAL_EQUALS(10, string.index_of('z'))
    ASSERT_OPTIONAL_EQUALS(10, string.index_of(L'z'))
    ASSERT_OPTIONAL_EQUALS(10, string.index_of(String("z")))

    ASSERT_OPTIONAL_EMPTY(string.index_of(L'u'))
    ASSERT_OPTIONAL_EMPTY(string.index_of('u'))
    ASSERT_OPTIONAL_EMPTY(string.index_of(String("u")))

    ASSERT_OPTIONAL_EMPTY(string.index_of(L'#'))
    ASSERT_OPTIONAL_EMPTY(string.index_of('#'))
    ASSERT_OPTIONAL_EMPTY(string.index_of(String("#")))

    ASSERT_OPTIONAL_EMPTY(string.index_of(L'\n'))
    ASSERT_OPTIONAL_EMPTY(string.index_of('\n'))
    ASSERT_OPTIONAL_EMPTY(string.index_of(String("\n")))

    ASSERT_OPTIONAL_EMPTY(string.index_of(L'\r'))
    ASSERT_OPTIONAL_EMPTY(string.index_of('\r'))
    ASSERT_OPTIONAL_EMPTY(string.index_of(String("\r")))

    ASSERT_OPTIONAL_EMPTY(string.index_of(String("fo0")))
    ASSERT_OPTIONAL_EMPTY(string.index_of(String("hello world!")))
}

void test_input() {
    std::stringstream in;

    in << "Wow\n";
    String string;
    in >> string;
    ASSERT_EQUALS(String("Wow"), string)

    in << "OMG";
    string = String("");
    in.ignore(); // skip '\n'
    in >> string;
    std::cout << "Testing eq of " << String("OMGr") << " and " << string << std::endl;
    ASSERT_EQUALS(String("OMG"), string)
}

void run_tests() {
    RUN_TEST(test_equality())
    RUN_TEST(test_comparison())
    RUN_TEST(test_length())
    RUN_TEST(test_empty())
    RUN_TEST(test_append())
    RUN_TEST(test_multiply())
    RUN_TEST(test_indexed_read())
    RUN_TEST(test_indexed_write())
    RUN_TEST(test_index_of())
    RUN_TEST(test_output())
    RUN_TEST(test_input())
}
