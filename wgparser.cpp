#include "wgparser.h"
#include <iostream>

int OpperatorPrecdence(std::string_view opp)
{
    if (opp == ".")
    {
        return 4;
    }
    else if (opp == "/" || opp == "*")
    {
        return 3;
    }
    else if (opp == "-" || opp == "+")
    {
        return 2;
    }

    return 0;
}

std::vector<Token> infixToPostFix(std::vector<Token> tokens) noexcept
{
    std::vector<Token> stack;
    std::vector<Token> value;
    for (int i = 0; i < tokens.size(); i++)
    {
        Token &token = tokens.at(i);
        if (token.is(Token::Kind::WGSymbol))
        {
            if (token.value() == ".")
            {
                if(i < tokens.size() - 1 && tokens[i + 1].is(Token::Kind::WGIdentifier)) {
                    value.push_back(tokens[i + 1]);
                    value.push_back(token);
                    i++;
                }
            }
            else
            {
                while (stack.size() && OpperatorPrecdence(stack.back().value()) >= OpperatorPrecdence(token.value()))
                {
                    value.push_back(stack.back());
                    stack.pop_back();
                }
                stack.push_back(tokens.at(i));
            }
        }
        else if (token.is(Token::Kind::WGLeftParenthesis))
        {
            if (value.back().is(Token::Kind::WGIdentifier) || (value.back().is(Token::Kind::WGSymbol) && value.back().value() == "."))
            {
                size_t j = i + 1;
                value.push_back(Token{Token::Kind::WGParameterStart});
                while (j < tokens.size() && !tokens.at(j).is(Token::Kind::WGRightParenthesis))
                {
                    value.push_back(tokens.at(j++));
                }
                value.push_back(Token{Token::Kind::WGParameterEnd});
                i = j;
            }
            else
            {
                stack.push_back(tokens.at(i));
            }
        }
        else
        {
            value.push_back(tokens.at(i));
        }
    }

    while (stack.size())
    {
        value.push_back(stack.back());
        stack.pop_back();
    }

    return value;
}

AbstractSyntaxTree Parser::next() noexcept
{
    return Parser::next(AbstractSyntaxTree());
}

AbstractSyntaxTree Parser::next(AbstractSyntaxTree prev) noexcept
{

    if (Parser::iterP.isEnd())
    {
        return AbstractSyntaxTree::End();
    }

    Token current = Parser::iterP.peek();
    Parser::iterP.next();

    if (current.is(Parser::stopAt))
    {
        return prev;
    }

    if (current.isOneOf(Token::Kind::WGInt, Token::Kind::WGFloat, Token::Kind::WGString, Token::Kind::WGBoolean, Token::Kind::WGIdentifier))
    {
        return Parser::next(AbstractSyntaxTree::ValueAST(current));
    }

    if (current.is(Token::Kind::WGSymbol))
    {
        if (current.value() == "=")
        {
            if (prev.is(AbstractSyntaxTree::Kind::WGASTNull))
            {
                return AbstractSyntaxTree::Error();
            }
            std::vector<Token> tokens = Parser::collect(Token::Kind::WGDelimiter);
            return Parser::next(AbstractSyntaxTree::AssignmentAST(prev.lhs(), infixToPostFix(tokens), false));
        }
        else
        {
            std::vector<Token> tokens = Parser::collect(Token::Kind::WGDelimiter);

            tokens.insert(tokens.begin(), current);
            if (prev.is(AbstractSyntaxTree::Kind::WGValue))
            {
                tokens.insert(tokens.begin(), prev.lhs());
            }
            return Parser::next(AbstractSyntaxTree::ExpressionAST(infixToPostFix(tokens)));
        }
    }
    else
    {
        std::vector<Token> tokens = Parser::collect(Token::Kind::WGDelimiter);

        tokens.insert(tokens.begin(), current);
        if (prev.is(AbstractSyntaxTree::Kind::WGValue))
        {
            tokens.insert(tokens.begin(), prev.lhs());
        }
        return Parser::next(AbstractSyntaxTree::ExpressionAST(infixToPostFix(tokens)));
    }

    return prev;
}

std::vector<Token> Parser::collect(Token end) noexcept
{
    std::vector<Token> tokens;
    int i = 0;
    while (!Parser::iterP.peek().is(end.kind()))
    {
        tokens.push_back(Parser::iterP.next());
    }

    return tokens;
}
