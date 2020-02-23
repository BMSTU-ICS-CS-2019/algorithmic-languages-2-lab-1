#include "simple_string.h"

#include <cstdlib>
#include <cstring>
#include <cassert>
#include <utility>
#include <algorithm>

namespace lab {

    /*
     * Protected constructors
     */

    SimpleString::SimpleString(const size_t length)
            : buffer_(new wchar_t[length]), capacity_(length), length_(length) {}

    SimpleString::SimpleString(const size_t length, const size_t capacity) {
        assert((length <= capacity));

        buffer_ = new wchar_t[capacity_ = capacity];
        length_ = length;
    }

    /*
     * Public constructors
     */

    SimpleString::SimpleString() : SimpleString((size_t) 0) {}

    SimpleString::SimpleString(const size_t length, const wchar_t symbol) : SimpleString(length) {
        for (size_t i = 0; i < length; ++i) buffer_[i] = symbol;
    }

    SimpleString::SimpleString(char const *c_string) : SimpleString(strlen(c_string)) {
        // note: trailing '\0' is not copied
        mbstowcs(buffer_, c_string, length_);
    }

    /*
     * Special constructors
     */

    SimpleString::SimpleString(const SimpleString &original) : SimpleString(original.length_) {
        std::copy(original.buffer_, original.buffer_ + length_, buffer_);
    }

    SimpleString::SimpleString(SimpleString &&original) noexcept
            : buffer_(original.buffer_), capacity_(original.capacity_), length_(original.length_) {
        original.buffer_ = nullptr;
    }

    /*
     * Public destructor
     */

    SimpleString::~SimpleString() {
        delete[] buffer_;
    }

    /*
     * Special operators
     */

    SimpleString &SimpleString::operator=(const SimpleString &original) {
        if (this != &original) {
            const auto length = original.length_;
            if (length != length_) {
                // a new buffer should be allocated

                // free current buffer
                delete[] buffer_;

                // it is recommended to keep defaults in case something fails later
                buffer_ = nullptr;
                capacity_ = length_ = 0;

                // create needed copies and assign them to the fields
                buffer_ = new wchar_t[length];
                capacity_ = length_ = length;
            }

            std::copy(original.buffer_, original.buffer_ + length, length);
        }

        return *this;
    }

    SimpleString &SimpleString::operator=(SimpleString &&original) noexcept {
        if (this != &original) {
            // free current buffer
            delete[] buffer_;

            buffer_ = std::exchange(original.buffer_, nullptr);
            capacity_ = std::exchange(original.capacity_, 0);
            length_ = std::exchange(original.length_, 0);
        }

        return *this;
    }

    SimpleString &SimpleString::operator=(SimpleString original) noexcept {
        std::swap(buffer_, original.buffer_);
        std::swap(capacity_, original.capacity_);
        std::swap(length_, original.length_);

        return *this;
    }
}
