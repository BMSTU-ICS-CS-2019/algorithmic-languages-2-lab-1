#include "simple_string.h"

#include <cstdlib>
#include <cstring>
#include <cassert>
#include <utility>
#include <algorithm>

namespace lab {

    /*
     * Static functions
     */

    /**
     * @brief Calculates the new capacity of the string's buffer based on the current one and the minimal required
     *
     * @param current_capacity current capacity of the string's buffer
     * @param required_capacity minimal required capacity of the string's buffer
     * @return new capacity of the string's buffer
     */
    static inline size_t calculate_new_capacity(size_t current_capacity, size_t required_capacity) {
        if (current_capacity == SIZE_MAX) throw std::overflow_error("No more space available in this string");

        // multiply the current size by averagely 1.5
        const auto new_capacity = current_capacity + (current_capacity >> 1u);

        return new_capacity >= required_capacity ? new_capacity : required_capacity;
    }

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
     * Internal methods
     */

    void SimpleString::check_index(const size_t index) const noexcept(false) {
        if (index >= length_) throw std::out_of_range("Index " + std::to_string(index) + " exceeds string length");
    }

    inline void SimpleString::ensure_capacity(size_t required_capacity) {
        const auto capacity = capacity_;
        if (capacity < required_capacity) resize_to(calculate_new_capacity(capacity, required_capacity));
    }

    void SimpleString::resize_to(const size_t new_capacity) {
        if (capacity_ != new_capacity) {
            auto new_buffer = new wchar_t[new_capacity];
            std::copy(buffer_, buffer_ + std::min(length_, new_capacity), new_buffer);
            buffer_ = new_buffer;

            capacity_ = new_capacity;
        }
    }

    /*
     * Public constructors
     */

    SimpleString::SimpleString() : SimpleString((size_t) 0) {}

    SimpleString::SimpleString(const size_t length, const wchar_t symbol) : SimpleString(length) {
        for (size_t i = 0; i < length; ++i) buffer_[i] = symbol;
    }

    SimpleString::SimpleString(char const *wide_c_string) : SimpleString(strlen(wide_c_string)) {
        // note: trailing '\0' is not copied
        mbstowcs(buffer_, wide_c_string, length_);
    }

    SimpleString::SimpleString(wchar_t const *wide_c_string) : SimpleString(wcslen(wide_c_string)) {
        // note: trailing '\0' is not copied
        std::copy(wide_c_string, wide_c_string + length_, buffer_);
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
     * Constant public methods
     */

    size_t SimpleString::length() const noexcept {
        return length_;
    }

    bool SimpleString::empty() const noexcept {
        return length_ == 0;
    }

    std::optional<size_t> SimpleString::index_of(const wchar_t character) const noexcept {
        for (auto i = 0; i < length_; ++i) if (buffer_[i] == character) return i;
        return std::optional<size_t>();
    }

    std::optional<size_t> SimpleString::index_of(char character) const noexcept {
        return index_of(wchar_t(character));
    }

    std::optional<size_t> SimpleString::index_of(const SimpleString &other) const noexcept {
        if (other.empty()) return 0;

        const auto length = length_, other_length = other.length_;
        if (other_length > length) return std::optional<size_t>();

        size_t most_possible_index = length - other_length;
        for (size_t start_index = 0; start_index < most_possible_index; ++start_index) {
            size_t matched_characters = 0;
            for (size_t index = start_index, other_index = 0; index < length; ++index, ++other_index) {
                if (buffer_[index] == other.buffer_[other_index]) {
                    if (++matched_characters == other_length) return index;
                } else break;
            }
        }

        return std::optional<size_t>();
    }

    wchar_t SimpleString::at(const size_t index) const noexcept(false) {
        check_index(index);

        return buffer_[index];
    }

    wchar_t &SimpleString::at(const size_t index) noexcept(false) {
        check_index(index);

        return *(buffer_ + index);
    }

    bool SimpleString::equals(const SimpleString &other) const noexcept {
        const auto length = length_;
        if (length != other.length_) return false;

        for (size_t i = 0; i < length; ++i) if (buffer_[i] != other.buffer_[i]) return false;

        return true;
    }

    int SimpleString::compare(const SimpleString &other) const noexcept {
        const auto length = other.length(), other_length = other.length_;

        if (length == other_length) {
            for (size_t i = 0; i < length; ++i) {
                const auto character = buffer_[i], other_character = other.buffer_[i];
                if (character != other_character) return character > other_character ? 1 : -1;
            }

            return 0;
        }

        return length > other_length ? 1 : -1;
    }

    /*
     * Modifying public methods
     */

    void SimpleString::shrink() {
        resize_to(length_);
    }

    void SimpleString::append(const wchar_t character) {
        const auto length = length_, new_length = length + 1;
        ensure_capacity(new_length);

        buffer_[length] = character;
        length_ = new_length;
    }

    void SimpleString::append(const char character) {
        append(wchar_t(character));
    }

    void SimpleString::append(const SimpleString &other) {
        const auto length = length_, other_length = other.length_, new_length = length + other_length;
        ensure_capacity(new_length);

        const auto other_buffer = other.buffer_;
        std::copy(other_buffer, other_buffer + length, buffer_ + length);
        length_ = new_length;
    }

    void SimpleString::set(const size_t index, const wchar_t character) {
        check_index(index);

        buffer_[index] = character;
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

            std::copy(original.buffer_, original.buffer_ + length, buffer_);
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

    /*
     * Indexed access operators
     */

    wchar_t SimpleString::operator[](const size_t index) const noexcept(false) {
        return at(index);
    }

    wchar_t &SimpleString::operator[](const size_t index) noexcept(false) {
        return at(index);
    }

    /*
     * Modification operators
     */

    SimpleString SimpleString::operator+(const SimpleString &other) const {
        const auto length = length_, other_length = other.length_;

        if (length == 0) return other_length == 0 ? SimpleString() : other /* explicit copy */;
        if (other_length == 0) return *this /* explicit copy */;

        SimpleString result(length + other_length);
        {
            // reuse `buffer` for `this->buffer_`
            auto buffer = buffer_, result_buffer = result.buffer_;
            std::copy(buffer, buffer + length, result_buffer);
            // reuse `buffer` for `other.buffer_`
            buffer = other.buffer_;
            std::copy(buffer, buffer + other_length, result_buffer + length);
        }

        return result;
    }

    SimpleString SimpleString::operator*(const size_t count) const {
        const auto length = length_;
        if (length == 0) return SimpleString();

        if (count > SIZE_MAX / length) throw std::overflow_error("The resulting string is too big");

        SimpleString result(length * count);
        {
            const auto start = buffer_, end = buffer_ + length;
            auto result_buffer = result.buffer_;
            for (size_t i = 0; i < count; ++i, result_buffer += length) std::copy(start, end, result_buffer);
        }

        return result;
    }

    bool SimpleString::operator==(const SimpleString &other) const noexcept {
        return equals(other);
    }

    bool SimpleString::operator!=(const SimpleString &other) const noexcept {
        return !equals(other);
    }

    bool SimpleString::operator>(const SimpleString &other) const noexcept {
        return compare(other) > 0;
    }

    bool SimpleString::operator>=(const SimpleString &other) const noexcept {
        return compare(other) >= 0;
    }

    bool SimpleString::operator<(const SimpleString &other) const noexcept {
        return compare(other) < 0;
    }

    bool SimpleString::operator<=(const SimpleString &other) const noexcept {
        return compare(other) <= 0;
    }

#ifdef __cpp_lib_three_way_comparison
    [[nodiscard]] bool operator<=>(const SimpleString &other) const noexcept {
        return compare(other);
    }
#endif

    std::ostream &operator<<(std::ostream &out, const SimpleString &string) {
        for (size_t i = 0; i < string.length_; ++i) out << char(string.buffer_[i]);

        return out;
    }

    std::wostream &operator<<(std::wostream &out, const SimpleString &string) {
        for (size_t i = 0; i < string.length_; ++i) out << string.buffer_[i];

        return out;
    }

    std::istream &operator>>(std::istream &in, SimpleString &string) {
        while (in.peek() != '\n') string.append(char(in.get()));
        return in;
    }

    std::wistream &operator>>(std::wistream &in, SimpleString &string) {
        while (in.peek() != '\n') string.append(wchar_t(in.get()));
        return in;
    }
}
