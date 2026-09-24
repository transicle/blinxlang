#pragma once

#include "ast.hh"
#include "lexer.hh"

#include <format>
#include <stdexcept>
#include <vector>

// NOTE: Box<T> is an alias to std::unique_ptr<T>

constexpr std::string_view format_token_type(const TokenType token_type)
{
    switch (token_type)
    {
    case TokenType::Int:
        return "int";
    case TokenType::Plus:
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
        while (m_current.ty != TokenType::EOF_)
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

    Box<Expr> parse_expr()
    {
        return parse_additive();
    }

    Box<Expr> parse_additive() {}
    Box<Expr> parse_multiplicative() {}
    Box<Expr> parse_primary() {}
}