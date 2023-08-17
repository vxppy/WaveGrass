#include "wglexer.h"

bool isSpace(char c)
{
    switch (c)
    {
    case ' ':
    case '\t':
    case '\r':
        return true;
    default:
        return false;
    }
}

bool isIdentifierChar(char c)
{
    return (c > 64 && c < 91) || (c > 96 && c < 123);
}

bool isNumberChar(char c)
{
    return c > 47 && c < 58;
}

bool isNewLine(char c)
{
    return c == '\n';
}

Token Lexer::identifier() noexcept
{
    const char *start = mBeg;
    Lexer::get();

    while (isIdentifierChar(peek()))
        Lexer::get();

    return Token{Token::Kind::WGIdentifier, start, mBeg};
}

Token Lexer::number() noexcept
{
    const char *start = mBeg;
    bool point = false;
    Lexer::get();

    while (isNumberChar(Lexer::peek()) || Lexer::peek() == '.')
    {
        Lexer::get();
        if (Lexer::peek() == '.')
        {
            if (!point)
                point = true;
            else
            {
                return Lexer::atom(Token::Kind::WGUnexpected);
            }
        }
    }

    if(point) {
        return Token{Token::Kind::WGFloat, start, mBeg};
    }
    return Token{Token::Kind::WGInt, start, mBeg};
}

Token Lexer::slashOrComment() noexcept
{
    Lexer::get();
    if(Lexer::peek() == '*') {
        Lexer::get();
        const char* start = mBeg;
        while(Lexer::peek()) {
            if(Lexer::get() == '/') {
                return Token{Token::Kind::WGComment, start, mBeg - 2};
            }
        }
        Lexer::atom(Token::Kind::WGUnexpected);
    } else if(Lexer::peek() == '/') {
        Lexer::get();
        return Token{Token::Kind::WGSymbol, mBeg - 2, mBeg};
    }

    return Token{Token::Kind::WGSymbol, mBeg - 1, mBeg};
}

Token Lexer::atom(Token::Kind kind) noexcept
{
    Lexer::get();
    return Token{kind};
}

Token Lexer::symbol() noexcept
{
    const char *start = mBeg;
    char last{Lexer::peek()};
    Lexer::get();

    if (Lexer::peek() == last)
    {
        Lexer::get();
        return Token(Token::Kind::WGSymbol, start, 2);
    }
    else
    {
        return Token(Token::Kind::WGSymbol, start, 1);
    }
}

Token Lexer::string() noexcept
{
    const char *start = mBeg + 1;
    char delim = Lexer::get();
    bool end = false;

    while (Lexer::peek())
    {
        if (Lexer::get() == delim)
        {
            end = true;
            break;
        }
    }

    if (!end)
    {
        Lexer::atom(Token::Kind::WGUnexpected);
    }
    return Token(Token::Kind::WGString, start, mBeg - 1);
}

Token Lexer::numberOrSymbol() noexcept
{
    Lexer::get();
    if (isNumberChar(Lexer::peek()))
    {
        const char *start = mBeg - 1;
        while (isNumberChar(peek()))
            Lexer::get();

        return Token{Token::Kind::WGFloat, start, mBeg};
    }
    else
    {
        return Token{Token::Kind::WGSymbol, mBeg - 1, 1};
    }
}

Token Lexer::next() noexcept
{
    while (isSpace(Lexer::peek()))
    {
        Lexer::get();
    }

    switch (Lexer::peek())
    {
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
        return Lexer::identifier();
    case '=':
    case '+':
    case '*':
    case '-':
    case '^':
    case '&':
    case '|':
        return Lexer::symbol();
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return Lexer::number();
    case '(':
        Lexer::bStack.push_back(0);
        return Lexer::atom(Token::Kind::WGLeftParenthesis);
    case ')':
        if (Lexer::bStack.back() == 0)
        {
            Lexer::bStack.pop_back();
            return Lexer::atom(Token::Kind::WGRightParenthesis);
        }
        else
        {
            return Lexer::atom(Token::Kind::WGUnexpected);
        }
    case '[':
        Lexer::bStack.push_back(1);
        return Lexer::atom(Token::Kind::WGLeftBracket);
    case ']':
        if (Lexer::bStack.back() == 1)
        {
            Lexer::bStack.pop_back();
            return Lexer::atom(Token::Kind::WGRightBracket);
        }
        else
        {
            return Lexer::atom(Token::Kind::WGUnexpected);
        }
    case '{':
        Lexer::bStack.push_back(2);
        return Lexer::atom(Token::Kind::WGLeftBraces);
    case '}':
        if (Lexer::bStack.back() == 1)
        {
            Lexer::bStack.pop_back();
            return Lexer::atom(Token::Kind::WGRightBraces);
        }
        else
        {
            return Lexer::atom(Token::Kind::WGUnexpected);
        }
    case '\n':
        Lexer::get();
        return Token({Token::Kind::WGDelimiter});
    case '\'':
    case '"':
    case '`':
        return Lexer::string();
    case '.':
        return Lexer::numberOrSymbol();
    case '/':
        return Lexer::slashOrComment();
        case ',':
        return Lexer::atom(Token::Kind::WGSeperator);
    }

    if (Lexer::bStack.size())
    {
        return Lexer::atom(Token::Kind::WGUnexpected);
    }

    return Token{Token::Kind::WGEnd};
}