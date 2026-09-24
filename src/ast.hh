#pragma once

#include <cstdint>
#include <memory>

template <typename T>
using Box = std::unique_ptr<T>;

struct Expr
{
    virtual ~Expr() = default;
};

struct Literal : Expr
{
    int64_t value{};

    explicit Literal(const int64_t v) : value{v} {}
};

enum class BinaryOp : uint8_t
{
    Add,
    Mul,
    Sub,
    Div,
};

struct BinaryExpr : Expr
{
    BinaryOp op{};
    Box<Expr> lhs, rhs;

    BinaryExpr(BinaryOp op, Box<Expr> lhs, Box<Expr> rhs)
        : op{op},
          lhs{std::move(lhs)},
          rhs{std::move(rhs)} {}
};
