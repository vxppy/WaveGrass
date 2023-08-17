#include <iostream>
#include <iomanip>
#include "wglexer.h"
#include "wgparser.h"
#include "wgenvironment.h"
#include <string>
#include <fstream>

std::ostream &operator<<(std::ostream &os, const Token::Kind &kind)
{
    static const char *const names[]{
        "WGDelimiter",
        "WGNull",
        "WGKeyword",
        "WGIdentifier",
        "WGBoolean",
        "WGNumber",
        "WGString",
        "WGLeftParenthesis",
        "WGRighParenthesis",
        "WGLeftBracket",
        "WGRightBracket",
        "WGLeftBraces",
        "WGRightBraces",
        "WGSymbol",
        "WGUnexpected",
        "WGComment",
        "WGSeperator",
        "WGParameterStart",
        "WGParameterEnd",
        "WGEnd",
        "WGNone"};
    return os << names[static_cast<int>(kind)];
}

std::ostream &operator<<(std::ostream &os, const AbstractSyntaxTree::Kind &kind)
{
    static const char *const names[]{
        "WGExpression",
        "WGCall",
        "WGAssigment",
        "WGValue",
        "WGASTNull",
        "WGError"};
    return os << names[static_cast<int>(kind)];
}

int main()
{

    std::string line, text;
    std::ifstream data("./tests/main.wg");

    while (std::getline(data, line))
    {
        text += line + '\n';
    }

    data.close();

    const char *code = text.c_str();
    Lexer lexer{code};
    TokenIterator iter{};

    for (auto token = lexer.next(); !token.is(Token::Kind::WGEnd); token = lexer.next())
    {
        iter.append(token);
        // std::cout << token.kind() << " | " << token.value() << std::endl;
        if (token.is(Token::Kind::WGUnexpected))
        {
            break;
        }
    }


    Parser parser{iter, Token::Kind::WGDelimiter};
    Environment env{};

    std::cout << "here";

    for (auto value = parser.next(); value.kind() != AbstractSyntaxTree::Kind::WGEnd; value = parser.next())
    {
        env.execute(value);
        // if (value.is(AbstractSyntaxTree::Kind::WGAssigment))
        // {
        //     std::cout << value.kind() << " | " << value.lhs().kind() << " | " << value.lhs().value() << std::endl;
        //     std::cout << "Expression: " << std::endl;
        //     for (auto &token : value.expression())
        //     {
        //         std::cout << std::setw(20) << token.kind() << " | " << token.value() << std::endl;
        //     }
        // } else if(value.is(AbstractSyntaxTree::Kind::WGExpression)) {
        //     std::cout << value.kind() << std::endl;
        //     for (auto &token : value.expression())
        //     {
        //         std::cout << std::setw(20) << token.kind() << " | " << token.value() << std::endl;
        //     }
        // }
    }

    return 0;
}