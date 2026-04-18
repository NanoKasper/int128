#ifndef INT128_HPP
#define INT128_HPP

#include <cstdint>
#include <string>
#include <string_view>
#include <iostream>
#include <climits>

class Int128 {
private:
    using i128 = __int128_t;
    using u128 = __uint128_t;

    static constexpr u128 kTwo64 = (u128{1} << 64);
    static constexpr u128 kMaxPos = (u128{1} << 127) - 1; //  2^127 - 1
    static constexpr u128 kMaxNegMag = (u128{1} << 127);  // |min| = 2^127

    i128 value_{0};

    static constexpr u128 abs_mag(i128 v) noexcept {
        const u128 u = static_cast<u128>(v);
        return (v < 0) ? (u128{0} - u) : u;
    }

    static constexpr Int128 from_raw(i128 v) noexcept {
        Int128 r;
        r.value_ = v;
        return r;
    }

public:
    // Конструирование без аргументов
    constexpr Int128() noexcept = default;

    // Конструирование от int64_t
    constexpr Int128(int64_t v) noexcept : value_(static_cast<i128>(v)) {}

    // Конструирование от std::string_view
    explicit Int128(std::string_view str);

    // Явное приведение к int64_t
    explicit operator int64_t() const;

    // Явное приведение к double
    explicit operator double() const;

    // Перевод в строку
    std::string str() const;

    // Сложение
    Int128 operator+(const Int128& other) const noexcept;
    Int128& operator+=(const Int128& other) noexcept;

    // Вычитание
    Int128 operator-(const Int128& other) const noexcept;
    Int128& operator-=(const Int128& other) noexcept;

    // Умножение
    Int128 operator*(const Int128& other) const noexcept;
    Int128& operator*=(const Int128& other) noexcept;

    // Деление
    Int128 operator/(const Int128& other) const noexcept;
    Int128& operator/=(const Int128& other) noexcept;

    // Унарный минус
    Int128 operator-() const noexcept;

    // Сравнение на равенство
    bool operator==(const Int128& other) const noexcept;
    bool operator!=(const Int128& other) const noexcept;
};

std::ostream& operator<<(std::ostream& os, const Int128& value);

#endif // INT128_HPP