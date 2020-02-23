#ifndef SEM_2_LAB_1_SIMPLE_STRING_H
#define SEM_2_LAB_1_SIMPLE_STRING_H


#include <cstddef>
#include <cstring>

namespace lab {

    /**
     * @brief Simple implementation of a
     */
    class SimpleString {
    protected:

        /**
         * @brief Character buffer, its length is at least {@code length_}
         *
         * @note stored string is not 0-terminated
         */
        wchar_t *buffer_;

        /**
         * @brief Length of allocated {@code buffer_}
         */
        size_t capacity_,
        /**
         * @brief Length of the meaningful part of {@code buffer_}
         */
        length_;

        /*
         * Protected constructor
         */

        /**
         * @brief Creates a new simple string of given length with no extra buffer space
         *
         * @param length length of the created string
         */
        explicit SimpleString(size_t length);

        /**
         * @brief Creates a new simple string of given length and capacity
         *
         * @param length length of the created string
         * @param capacity capacity of the created string
         * @throws if {@code capacity_} is less than {@code length_}
         */
        explicit SimpleString(size_t length, size_t capacity);

    public:

        /*
         * Public constructors
         */

        /**
         * @brief Creates a new empty string
         */
        SimpleString();

        /**
         * @brief Creates a new string of given size with its content set to {@ode #symbol}
         *
         * @param length length of the created string
         * @param symbol symbol to fill the string with
         */
        SimpleString(size_t length, wchar_t symbol);

        /**
         * @brief Creates a new string based on the given C-string (0-terminated dynamic {@code char}-array)
         *
         * @param c_string original string to be copied into the created one
         */
        explicit SimpleString(char const *c_string);

        /*
         * Special constructors
         */

        /**
         * @brief Copies the original string into the created one
         *
         * @param original string which should be copied into the created one
         * @note the created string will have no extra buffer space
         */
        SimpleString(const SimpleString &original);

        /**
         * @brief Moves the original string into the created one
         *
         * @param original string which should be moved into the created one
         */
        SimpleString(SimpleString &&original) noexcept;

        /*
         * Public destructor
         */

        /**
         * @brief Destroys this string freeing all dynamically allocated memory (i.e. {@code buffer_})
         */
        ~SimpleString();

        /*
         * Special operators
         */

        SimpleString &operator=(const SimpleString &original);

        SimpleString &operator=(SimpleString &&original) noexcept;

        SimpleString &operator=(SimpleString original) noexcept;
    };

    /*
     * Define `CustomString` as String
     */
    typedef SimpleString String;
}

#endif //SEM_2_LAB_1_SIMPLE_STRING_H
