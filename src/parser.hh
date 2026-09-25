#pragma once

#include "ast.hh"
#include "lexer.hh"

#include <format>
#include <utility>
#include <stdexcept>
#include <vector>

// NOTE: Box<T> is an alias to std::unique_ptr<T>

constexpr std::string_view format_token_type(const TokenType token_type)
{
    switch (token_type)
    {
    case TokenType::Int:
        return "int";
    case TokenType::Add:
        return "+";
    case TokenType::Mul:
        return "*";
    case TokenType::Sub:
        return "-";
    case TokenType::Div:
        return "/";
    case TokenType::Eof:
        return "eof";
    }

    return "?";
}

class Parser
{
public:
    explicit Parser(std::string_view input) : m_lexer{input}
    {
        m_current = m_lexer.next();
    }

    std::vector<Box<Expr>> parse()
    {
        std::vector<Box<Expr>> exprs;
        while (m_current.token_type != TokenType::Eof)
            exprs.push_back(parse_expr());

        return exprs;
    }

private:
    Lexer m_lexer;
    Token m_current;

    Token advance()
    {
        return std::exchange(m_current, m_lexer.next());
    }

    Token expect(const TokenType token_type)
    {
        if (m_current.token_type != token_type)
        {
            throw std::runtime_error(
                std::format("Expected {}, got {}", format_token_type(token_type), format_token_type(m_current.token_type)));
        }

        return advance();
    }

    template <typename... T>
    bool matches(T... types) const {
        return ((m_current.token_type == types) || ...)}

    Box<Expr> parse_expr()
    {
        return parse_additive();
    }

    Box<Expr> parse_additive()
    {
        auto lhs = parse_multiplicative();

        while (matches(TokenType::Add, TokenType::Sub))
        {
            auto op = m_current.token_type == TokenType::Add ? BinaryOp::Add : BinaryOp::Sub;
            advance();

            auto rhs = parse_primary();
            lhs = std::make_unique<BinaryExpr>(op, std::move(lhs), std::move(rhs));
        }

        return lhs;
    }

    Box<Expr> parse_multiplicative()
    {
        auto lhs = parse_primary();

        while (matches(TokenType::Mul, TokenType::Div))
        {
            auto op = m_current.token_type == TokenType::Mul ? BinaryOp::Mul : BinaryOp::Div;
            advance();

            auto rhs = parse_primary();
            lhs = std::make_unique<BinaryExpr>(op, std::move(lhs), std::move(rhs));
        }

        return lhs;
    }

    Box<Expr> parse_primary()
    {
        switch (m_current.token_type)
        {
        case TokenType::Int:
        {
            int64_t value{};
            auto [pointer, ec] = std::from_chars(
                m_current.lexeme.data(),
                m_current.lexeme.data() + m_current.lexeme.size(),
                value);

            if (ec != std::errc())
                throw std::runtime_error("Failed to parse integer literal");

            advance();
            return std::make_unique<Literal>(value);
        }

        case TokenType::LParen:
        {
            advance();
            auto expr = parse_expr();
            expect(TokenType::RParen);

            return expr;
        }

        default:
            throw std::runtime_error(std::format("Expected primary, got {}", format_token_type(m_current.token_type)));
        }
    }
};