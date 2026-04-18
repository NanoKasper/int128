#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "int128.hpp"

#include <iosfwd>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

class Expression {
public:
    using Variables = std::unordered_map<std::string, Int128>;

    virtual ~Expression() = default;

    virtual Int128 eval(const Variables& vars) const = 0;
    virtual Expression* clone() const = 0;
    virtual void print(std::ostream& os) const = 0;

    std::unique_ptr<Expression> clone_ptr() const { return std::unique_ptr<Expression>(clone()); }
};

std::ostream& operator<<(std::ostream& os, const Expression& expr);

class Const final : public Expression {
public:
    explicit Const(Int128 value) : value_(std::move(value)) {}
    explicit Const(int64_t value) : value_(value) {}
    explicit Const(std::string_view value) : value_(value) {}

    Int128 eval(const Variables&) const override { return value_; }
    Const* clone() const override { return new Const(*this); }
    void print(std::ostream& os) const override;

private:
    Int128 value_;
};

class Variable final : public Expression {
public:
    explicit Variable(std::string_view name) : name_(name) {}

    Int128 eval(const Variables& vars) const override;
    Variable* clone() const override { return new Variable(*this); }
    void print(std::ostream& os) const override;

private:
    std::string name_;
};

class Negate final : public Expression {
public:
    explicit Negate(const Expression& expr);
    explicit Negate(std::unique_ptr<Expression> expr) : expr_(std::move(expr)) {}

    Negate(const Negate& other);
    Negate& operator=(const Negate& other);
    Negate(Negate&&) noexcept = default;
    Negate& operator=(Negate&&) noexcept = default;

    Int128 eval(const Variables& vars) const override { return -(expr_->eval(vars)); }
    Negate* clone() const override { return new Negate(*this); }
    void print(std::ostream& os) const override;

private:
    std::unique_ptr<Expression> expr_;
};

class Add final : public Expression {
public:
    Add(const Expression& a, const Expression& b);
    Add(std::unique_ptr<Expression> a, std::unique_ptr<Expression> b) : a_(std::move(a)), b_(std::move(b)) {}

    Add(const Add& other);
    Add& operator=(const Add& other);
    Add(Add&&) noexcept = default;
    Add& operator=(Add&&) noexcept = default;

    Int128 eval(const Variables& vars) const override { return a_->eval(vars) + b_->eval(vars); }
    Add* clone() const override { return new Add(*this); }
    void print(std::ostream& os) const override;

private:
    std::unique_ptr<Expression> a_;
    std::unique_ptr<Expression> b_;
};

class Subtract final : public Expression {
public:
    Subtract(const Expression& a, const Expression& b);
    Subtract(std::unique_ptr<Expression> a, std::unique_ptr<Expression> b) : a_(std::move(a)), b_(std::move(b)) {}

    Subtract(const Subtract& other);
    Subtract& operator=(const Subtract& other);
    Subtract(Subtract&&) noexcept = default;
    Subtract& operator=(Subtract&&) noexcept = default;

    Int128 eval(const Variables& vars) const override { return a_->eval(vars) - b_->eval(vars); }
    Subtract* clone() const override { return new Subtract(*this); }
    void print(std::ostream& os) const override;

private:
    std::unique_ptr<Expression> a_;
    std::unique_ptr<Expression> b_;
};

class Multiply final : public Expression {
public:
    Multiply(const Expression& a, const Expression& b);
    Multiply(std::unique_ptr<Expression> a, std::unique_ptr<Expression> b) : a_(std::move(a)), b_(std::move(b)) {}

    Multiply(const Multiply& other);
    Multiply& operator=(const Multiply& other);
    Multiply(Multiply&&) noexcept = default;
    Multiply& operator=(Multiply&&) noexcept = default;

    Int128 eval(const Variables& vars) const override { return a_->eval(vars) * b_->eval(vars); }
    Multiply* clone() const override { return new Multiply(*this); }
    void print(std::ostream& os) const override;

private:
    std::unique_ptr<Expression> a_;
    std::unique_ptr<Expression> b_;
};

class Divide final : public Expression {
public:
    Divide(const Expression& a, const Expression& b);
    Divide(std::unique_ptr<Expression> a, std::unique_ptr<Expression> b) : a_(std::move(a)), b_(std::move(b)) {}

    Divide(const Divide& other);
    Divide& operator=(const Divide& other);
    Divide(Divide&&) noexcept = default;
    Divide& operator=(Divide&&) noexcept = default;

    Int128 eval(const Variables& vars) const override { return a_->eval(vars) / b_->eval(vars); }
    Divide* clone() const override { return new Divide(*this); }
    void print(std::ostream& os) const override;

private:
    std::unique_ptr<Expression> a_;
    std::unique_ptr<Expression> b_;
};

Negate operator-(const Expression& e);
Add operator+(const Expression& a, const Expression& b);
Subtract operator-(const Expression& a, const Expression& b);
Multiply operator*(const Expression& a, const Expression& b);
Divide operator/(const Expression& a, const Expression& b);

#endif // EXPRESSION_HPP
