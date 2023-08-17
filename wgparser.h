#pragma once
#include "tokenIter.h"
#include "wgast.h"

class Parser
{
public:
    Parser(TokenIterator iterator) noexcept { iterP = iterator; }
    Parser(TokenIterator iterator, Token::Kind stop) noexcept
    {
        iterP = iterator;
        stopAt = stop;
    }

    AbstractSyntaxTree next() noexcept;
    AbstractSyntaxTree next(AbstractSyntaxTree prev) noexcept;

private:
    TokenIterator iterP;
    std::vector<int> bStack;
    Token::Kind stopAt{Token::Kind::WGDelimiter};

    std::vector<Token> collect(Token end) noexcept;
};