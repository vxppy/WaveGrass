#pragma once
#include "wgtoken.h"
#include <vector>
#define WGkeywordsLength 5


class Lexer
{
public:
    Lexer(const char *beg) noexcept : mBeg{beg} {};
    Token next() noexcept;
    const char* keywords[WGkeywordsLength] = { "if", "else", "for", "let", "while" };

private:
    Token identifier() noexcept;
    Token number() noexcept;
    Token slashOrComment() noexcept;
    Token atom(Token::Kind) noexcept;
    Token symbol() noexcept;
    Token string() noexcept;
    Token numberOrSymbol() noexcept;

    char peek() const noexcept { return *mBeg; }
    char get() noexcept { return *mBeg++; }

    std::vector<int> bStack;

private:
    const char *mBeg = nullptr;
};