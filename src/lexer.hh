#pragma once

#include <cstdint>
#include <string>

enum class TokenType : std::uint8_t
{
    Int,
    Plus,
    Mul,
    Sub,
    Div,
    LParen,
    RParen,
    Eof,
};

struct Token
{
    TokenType token_type;
    std::string lexeme;
};

class Lexer
{
public:
    explicit Lexer(std::string_view source) : m_source{source} {}

    Token next()
    {
        while (!is_eof() && current() == ' ')
            advance();

        switch (const auto curr{current()}; curr)
        {
        case '+':
            advance();
            return Token{TokenType::Plus, "+"};
        case '*':
            advance();
            return Token{TokenType::Mul, "*"};
        case '-':
            advance();
            return Token{TokenType::Sub, "-"};
        case '/':
            advance();
            return Token{TokenType::Div, "/"};
        case '(':
            advance();
            return Token{TokenType::LParen, "("};
        case ')':
            advance();
            return Token{TokenType::RParen, ")"};
        default:
            if (std::isdigit(curr))
            {
                const auto start{m_position};
                while (!is_eof() && std::isdigit(m_source[m_position]))
                    advance();

                const auto end{m_position};
                const std::string value{m_source.substr(start, end - start)};

                return Token{TokenType::Int, value};
            }

            return Token{TokenType::Eof, ""};
        }
    }

private:
    std::string_view m_source{};
    std::size_t m_position{};

    void advance()
    {
        if (!is_eof())
            m_position++;
    }

    char current() const
    {
        return is_eof() ? '\0' : m_source[m_position];
    }

    bool is_eof() const
    {
        return m_position >= m_source.length();
    }
};