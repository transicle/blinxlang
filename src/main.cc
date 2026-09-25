#include "lexer.hh"
#include "parser.hh"

#include <print>

constexpr std::string format(const Expr *expr, const int indent = 0)
{
    std::string fmt(indent * 2, ' ');

    if (auto *literal = dynamic_cast<const Literal *>(expr))
        return std::format("{}Literal({})\n", fmt, literal->value);

    if (auto *bin = dynamic_cast<const BinaryExpr *>(expr))
    {
        std::string op;

        switch (bin->op)
        {
        case BinaryOp::Add:
            op = "+";
            break;
        case BinaryOp::Mul:
            op = "*";
            break;
        case BinaryOp::Sub:
            op = "-";
            break;
        case BinaryOp::Div:
            op = "/";
            break;
        }

        return std::format("{}Binary({})\n", fmt, op) + format(bin->lhs.get(), indent + 1) + format(bin->rhs.get(), indent + 1);
    }

    return fmt + "?\n";
}

int main()
{
    ////////////////////////////////////////////////////////////////
    /// Parser stuff, uncomment if you want to test the parser. ///
    //////////////////////////////////////////////////////////////
    Parser parser{"1 + 2 * 5"};

    auto exprs = parser.parse();
    for (auto &expr : exprs)
        std::print("{}", format(expr.get()));
    ////////////////////////////////////////////////////////////

    std::print("\n");

    //////////////////////////////////////////////////////////////
    /// Lexer stuff, uncomment if you want to test the lexer. ///
    ////////////////////////////////////////////////////////////
    Lexer lexer{"1 + 2 * 5"};

    auto next{lexer.next()};

    while (next.token_type != TokenType::Eof)
    {
        std::println("{}, {}", static_cast<std::uint8_t>(next.token_type), next.lexeme);
        next = lexer.next();
    }
    ///////////////////////////////////////////////////////////
}