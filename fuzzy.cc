#include "fuzzy.h"


namespace {
    std::string cut_zeros(real_t x) {
        std::string str = std::to_string(x);
        str.erase(str.find_last_not_of('0') + 1, std::string::npos);
        if (str.ends_with('.')) str.erase(str.end() - 1);
        return str;
    }
}  // namespace

TriFuzzyNum& TriFuzzyNum::operator=(TriFuzzyNum&& rhs) noexcept {
    l = rhs.l;
    m = rhs.m;
    u = rhs.u;
    rankX = rhs.rankX;
    rankY = rhs.rankY;
    rankZ = rhs.rankZ;

    return *this;
}

const TriFuzzyNum TriFuzzyNum::operator+(const TriFuzzyNum &rhs) const {
    return TriFuzzyNum(*this) += rhs;
}

const TriFuzzyNum TriFuzzyNum::operator-(const TriFuzzyNum &rhs) const {
    TriFuzzyNum result = TriFuzzyNum(*this) -= rhs;
    result.sort_args();
    return result;
}

const TriFuzzyNum TriFuzzyNum::operator*(const TriFuzzyNum &rhs) const {
    TriFuzzyNum result = TriFuzzyNum(*this) *= rhs;
    result.sort_args();
    return result;
}

TriFuzzyNum& TriFuzzyNum::operator+=(const TriFuzzyNum &rhs) {
    l += rhs.lower_value();
    m += rhs.modal_value();
    u += rhs.upper_value();

    calc_rank();

    return *this;
}

TriFuzzyNum& TriFuzzyNum::operator-=(const TriFuzzyNum &rhs) {
    l -= rhs.l;
    m -= rhs.l;
    u -= rhs.u;

    sort_args();
    calc_rank();

    return *this;
}

TriFuzzyNum& TriFuzzyNum::operator*=(const TriFuzzyNum &rhs) {
    l *= rhs.l;
    m *= rhs.m;
    u *= rhs.u;

    sort_args();
    calc_rank();

    return *this;
}

std::partial_ordering TriFuzzyNum::operator<=>(const TriFuzzyNum &rhs) const {
    std::partial_ordering comp = rankX <=> rhs.rankX;

    if (comp != std::partial_ordering::equivalent) {
        return comp;
    }

    comp = rankY <=> rhs.rankY;

    if (comp != std::partial_ordering::equivalent) {
        return comp;
    }

    return rankZ <=> rhs.rankZ;
}

std::ostream& operator<<(std::ostream& out, const TriFuzzyNum& num) {
    return out << "(" + cut_zeros(num.l) + ", "
                  + cut_zeros(num.m) + ", "
                  + cut_zeros(num.u) + ")";
}

void TriFuzzyNumSet::insert(TriFuzzyNum& num) {
    TriFuzzyNum copyNum = num;
    set.insert(copyNum);
}

void TriFuzzyNumSet::insert(TriFuzzyNum&& num) {
    TriFuzzyNum moveNum = num;
    set.insert(moveNum);
}

void TriFuzzyNumSet::remove(TriFuzzyNum& num) {
    if (!set.empty()) {
        set.erase(num);
    }
}

TriFuzzyNum TriFuzzyNumSet::arithmetic_mean() {
    if (set.empty()) {
        throw std::length_error(ERR);
    }

    real_t a = 0;
    real_t b = 0;
    real_t c = 0;

    for (TriFuzzyNum num : set) {
        a += num.lower_value();
        b += num.modal_value();
        c += num.upper_value();
    }

    size_t set_size = set.size();

    return {a / set_size, b / set_size, c / set_size};
}