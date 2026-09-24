#include "lexer.hh"

#include <print>

int main()
{
    Lexer lexer{"1 + 2 * 5"};

    auto next{lexer.next()};

    while (next.token_type != TokenType::Eof)
    {
        std::println("{}, {}", static_cast<std::uint8_t>(next.token_type), next.lexeme);
        next = lexer.next();
    }
}