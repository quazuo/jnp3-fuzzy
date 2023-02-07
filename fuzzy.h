#ifndef ZAD3_FUZZY_H
#define ZAD3_FUZZY_H

#include <iostream>
#include <set>

using real_t = double;

class TriFuzzyNum {
private:
    real_t l, m, u;
    real_t rankX, rankY, rankZ;

    constexpr real_t sqrtNewtonRaphson(real_t x, real_t curr, real_t prev) {
        return curr == prev
               ? curr
               : sqrtNewtonRaphson(x, 0.5 * (curr + x / curr), curr);
    }

    constexpr real_t csqrt(real_t x) {
        return x >= 0 && x < std::numeric_limits<real_t>::infinity()
               ? sqrtNewtonRaphson(x, x, 0)
               : std::numeric_limits<real_t>::quiet_NaN();
    }

    constexpr void calc_rank() {
        real_t z = (u - l) + csqrt(1 + (u - m) * (u - m)) + csqrt(1 + (m - l) * (m - l));
        real_t x = ((u - l) * m + csqrt(1 + (u - m) * (u - m)) * l +
                    csqrt(1 + (m - l) * (m - l)) * u) / z;
        real_t y = (u - l) / z;

        rankX = (x - y) / 2;
        rankY = 1 - y;
        rankZ = m;
    }

    constexpr void sort_args() {
        if (l > m)
            std::swap(l, m);
        if (l > u)
            std::swap(l, u);
        if (m > u)
            std::swap(m, u);
    }

public:
    constexpr TriFuzzyNum(real_t a, real_t b, real_t c) : l(a), m(b), u(c),
                                                          rankX(0), rankY(0), rankZ(0) {
        sort_args();
        calc_rank();
    }

    constexpr TriFuzzyNum(const TriFuzzyNum &tfn) = default;

    constexpr TriFuzzyNum(TriFuzzyNum &&tfn) noexcept: l(tfn.l), m(tfn.m), u(tfn.u),
                                                       rankX(tfn.rankX), rankY(tfn.rankY), rankZ(tfn.rankZ) {}

    ~TriFuzzyNum() = default;

    [[nodiscard]] constexpr real_t lower_value() const {
        return l;
    }

    [[nodiscard]] constexpr real_t modal_value() const {
        return m;
    }

    [[nodiscard]] constexpr real_t upper_value() const {
        return u;
    }

    constexpr bool operator==(const TriFuzzyNum &rhs) const {
        return (l == rhs.l && m == rhs.m && u == rhs.u);
    }

    constexpr bool operator!=(const TriFuzzyNum &rhs) const {
        return !(l == rhs.l && m == rhs.m && u == rhs.u);
    }

    TriFuzzyNum &operator=(const TriFuzzyNum &rhs) = default;

    TriFuzzyNum &operator=(TriFuzzyNum &&rhs) noexcept;

    const TriFuzzyNum operator+(const TriFuzzyNum &rhs) const;

    const TriFuzzyNum operator-(const TriFuzzyNum &rhs) const;

    const TriFuzzyNum operator*(const TriFuzzyNum &rhs) const;

    TriFuzzyNum &operator+=(const TriFuzzyNum &rhs);

    TriFuzzyNum &operator-=(const TriFuzzyNum &rhs);

    TriFuzzyNum &operator*=(const TriFuzzyNum &rhs);

    std::partial_ordering operator<=>(const TriFuzzyNum &rhs) const;

    friend std::ostream &operator<<(std::ostream &out, const TriFuzzyNum &num);
};

class TriFuzzyNumSet {
private:
    const std::string ERR = "TriFuzzyNumSet::arithmetic_mean - the set is empty.";
    std::multiset<TriFuzzyNum> set;

public:
    TriFuzzyNumSet() = default;

    virtual ~TriFuzzyNumSet() = default;

    void insert(TriFuzzyNum &num);

    void insert(TriFuzzyNum &&num);

    void remove(TriFuzzyNum &num);

    TriFuzzyNum arithmetic_mean();
};

consteval TriFuzzyNum crisp_number(real_t v) {
    return {v, v, v};
}

const constinit static TriFuzzyNum crisp_zero = crisp_number(0);

#endif //ZAD3_FUZZY_H
