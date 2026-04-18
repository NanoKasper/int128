#include "int128.hpp"
#include <algorithm>
#include <cctype>
#include <limits>

Int128::Int128(std::string_view s) {
    size_t i = 0;
    while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
    if (i == s.size()) {
        value_ = 0;
        return;
    }

    bool neg = false;
    if (s[i] == '+' || s[i] == '-') {
        neg = (s[i] == '-');
        ++i;
    }

    while (i < s.size() && s[i] == '0') ++i;
    if (i == s.size() || s[i] < '0' || s[i] > '9') {
        value_ = 0;
        return;
    }

    u128 mag = 0;
    const u128 limit = neg ? kMaxNegMag : kMaxPos;

    for (; i < s.size(); ++i) {
        const unsigned char ch = static_cast<unsigned char>(s[i]);
        if (ch < '0' || ch > '9') break;
        const u128 digit = static_cast<u128>(ch - '0');

        if (mag > (limit - digit) / 10) {
            mag = limit;
            break;
        }
        mag = mag * 10 + digit;
    }

    if (!neg) {
        value_ = static_cast<i128>(mag);
    } else {
        const u128 u = u128{0} - mag;
        value_ = static_cast<i128>(u);
    }
}

Int128::operator int64_t() const {
    if (value_ < static_cast<i128>(std::numeric_limits<int64_t>::min()) ||
        value_ > static_cast<i128>(std::numeric_limits<int64_t>::max())) {
        return 0;
    }
    return static_cast<int64_t>(value_);
}

Int128::operator double() const {
    return static_cast<double>(static_cast<long double>(value_));
}

Int128 Int128::operator+(const Int128& other) const noexcept {
    return from_raw(static_cast<i128>(static_cast<u128>(value_) + static_cast<u128>(other.value_)));
}

Int128& Int128::operator+=(const Int128& other) noexcept {
    value_ = static_cast<i128>(static_cast<u128>(value_) + static_cast<u128>(other.value_));
    return *this;
}

Int128 Int128::operator-(const Int128& other) const noexcept {
    return from_raw(static_cast<i128>(static_cast<u128>(value_) - static_cast<u128>(other.value_)));
}

Int128& Int128::operator-=(const Int128& other) noexcept {
    value_ = static_cast<i128>(static_cast<u128>(value_) - static_cast<u128>(other.value_));
    return *this;
}

Int128 Int128::operator*(const Int128& other) const noexcept {
    return from_raw(static_cast<i128>(static_cast<u128>(value_) * static_cast<u128>(other.value_)));
}

Int128& Int128::operator*=(const Int128& other) noexcept {
    value_ = static_cast<i128>(static_cast<u128>(value_) * static_cast<u128>(other.value_));
    return *this;
}

Int128 Int128::operator/(const Int128& other) const noexcept {
    if (other.value_ == 0) return Int128{};
    return from_raw(value_ / other.value_);
}

Int128& Int128::operator/=(const Int128& other) noexcept {
    if (other.value_ == 0) {
        value_ = 0;
    } else {
        value_ /= other.value_;
    }
    return *this;
}

Int128 Int128::operator-() const noexcept {
    return from_raw(static_cast<i128>(u128{0} - static_cast<u128>(value_)));
}

bool Int128::operator==(const Int128& other) const noexcept {
    return value_ == other.value_;
}

bool Int128::operator!=(const Int128& other) const noexcept {
    return !(*this == other);
}

std::string Int128::str() const {
    if (value_ == 0) return "0";

    const bool neg = value_ < 0;
    u128 mag = abs_mag(value_);

    std::string out;
    while (mag != 0) {
        const unsigned digit = static_cast<unsigned>(mag % 10);
        out.push_back(static_cast<char>('0' + digit));
        mag /= 10;
    }
    if (neg) out.push_back('-');
    std::reverse(out.begin(), out.end());
    return out;
}

std::ostream& operator<<(std::ostream& os, const Int128& value) {
    os << value.str();
    return os;
}
