#pragma once
#include <vector>
#include "wgtoken.h"

class TokenIterator {
    public:
        TokenIterator() {};
        void append(Token token) noexcept { arrayT.push_back(token); }
        Token peek() noexcept { return arrayT.at(index); }
        Token next() noexcept { return arrayT.at(index++); } 
        bool isEnd() noexcept { return arrayT.size() == index; }
    private:
        size_t index{0};
        std::vector<Token> arrayT;
};