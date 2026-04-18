#include "expression.hpp"

#include <ostream>

std::ostream& operator<<(std::ostream& os, const Expression& expr) {
    expr.print(os);
    return os;
}

void Const::print(std::ostream& os) const {
    os << value_;
}

Int128 Variable::eval(const Variables& vars) const {
    auto it = vars.find(name_);
    if (it == vars.end()) return Int128{};
    return it->second;
}

void Variable::print(std::ostream& os) const {
    os << name_;
}

Negate::Negate(const Expression& expr) : expr_(expr.clone_ptr()) {}

Negate::Negate(const Negate& other) : expr_(other.expr_->clone_ptr()) {}

Negate& Negate::operator=(const Negate& other) {
    if (this == &other) return *this;
    expr_ = other.expr_->clone_ptr();
    return *this;
}

void Negate::print(std::ostream& os) const {
    os << "(-" << *expr_ << ")";
}

Add::Add(const Expression& a, const Expression& b) : a_(a.clone_ptr()), b_(b.clone_ptr()) {}

Add::Add(const Add& other) : a_(other.a_->clone_ptr()), b_(other.b_->clone_ptr()) {}

Add& Add::operator=(const Add& other) {
    if (this == &other) return *this;
    a_ = other.a_->clone_ptr();
    b_ = other.b_->clone_ptr();
    return *this;
}

void Add::print(std::ostream& os) const {
    os << "(" << *a_ << " + " << *b_ << ")";
}

Subtract::Subtract(const Expression& a, const Expression& b) : a_(a.clone_ptr()), b_(b.clone_ptr()) {}

Subtract::Subtract(const Subtract& other) : a_(other.a_->clone_ptr()), b_(other.b_->clone_ptr()) {}

Subtract& Subtract::operator=(const Subtract& other) {
    if (this == &other) return *this;
    a_ = other.a_->clone_ptr();
    b_ = other.b_->clone_ptr();
    return *this;
}

void Subtract::print(std::ostream& os) const {
    os << "(" << *a_ << " - " << *b_ << ")";
}

Multiply::Multiply(const Expression& a, const Expression& b) : a_(a.clone_ptr()), b_(b.clone_ptr()) {}

Multiply::Multiply(const Multiply& other) : a_(other.a_->clone_ptr()), b_(other.b_->clone_ptr()) {}

Multiply& Multiply::operator=(const Multiply& other) {
    if (this == &other) return *this;
    a_ = other.a_->clone_ptr();
    b_ = other.b_->clone_ptr();
    return *this;
}

void Multiply::print(std::ostream& os) const {
    os << "(" << *a_ << " * " << *b_ << ")";
}

Divide::Divide(const Expression& a, const Expression& b) : a_(a.clone_ptr()), b_(b.clone_ptr()) {}

Divide::Divide(const Divide& other) : a_(other.a_->clone_ptr()), b_(other.b_->clone_ptr()) {}

Divide& Divide::operator=(const Divide& other) {
    if (this == &other) return *this;
    a_ = other.a_->clone_ptr();
    b_ = other.b_->clone_ptr();
    return *this;
}

void Divide::print(std::ostream& os) const {
    os << "(" << *a_ << " / " << *b_ << ")";
}

Negate operator-(const Expression& e) { return Negate(e); }
Add operator+(const Expression& a, const Expression& b) { return Add(a, b); }
Subtract operator-(const Expression& a, const Expression& b) { return Subtract(a, b); }
Multiply operator*(const Expression& a, const Expression& b) { return Multiply(a, b); }
Divide operator/(const Expression& a, const Expression& b) { return Divide(a, b); }

