#include "wgenvironment.h"
#include <iostream>

wg::Object operate(std::vector<Token> tokens, Environment &env) noexcept;

wg::Object binaryOperation(std::string_view opp, wg::Object &lhs, wg::Object &rhs) noexcept
{
    if (opp == "*")
    {
        return lhs.__mul__(rhs);
    }
    else if (opp == "+")
    {
        return lhs.__add__(rhs);
    }
    else if (opp == "/")
    {
        return lhs.__div__(rhs);
    }
    else if (opp == "-")
    {
        return lhs.__sub__(rhs);
    }
    else if (opp == ".")
    {
        wg::Object val = lhs.__get_property__(rhs.__str__());
        std::cout << (int)val.__type__();
        if (wg::isType(val.__type__(), wg::Type::Reference))
        {
            val = *val.__get__();
        }
    }

    return wg::Int(0);
}

std::vector<wg::Object> parseArguments(std::vector<Token> tokens, Environment &env)
{
    std::vector<wg::Object> values{};

    size_t start = 0;
    for (size_t i = 0; i < tokens.size(); i++)
    {
        // if(tokens.at())
        auto token = tokens[i];
        // std::cout << token[i].value();
        if (token.is(Token::Kind::WGParameterEnd))
        {
            values.push_back(operate(std::vector<Token>(tokens.begin() + start, tokens.begin() + i), env));
        }
    }

    return values;
}

// // operate()
wg::Object operate(std::vector<Token> tokens, Environment &env) noexcept
{
    std::vector<wg::Object> value{};

    for (size_t i = 0; i < tokens.size(); i++)
    {
        auto token = tokens[i];
        std::cout << token.value();
        if (token.is(Token::Kind::WGSymbol))
        {
            auto rhs = value.back();
            value.pop_back();
            auto lhs = value.back();
            value.pop_back();
            value.push_back(binaryOperation(token.value(), lhs, rhs));
        }
        else
        {
            if (token.is(Token::Kind::WGIdentifier))
            {
                if (i < tokens.size() - 2)
                {
                    if (tokens[i + 1].is(Token::Kind::WGSymbol) && tokens[i + 1].value() == ".")
                    {
                        value.push_back(wg::RawAccessToken(std::string(token.value())));
                    }
                    else
                    {
                        auto val = env.getVariable(token.value());
                        if (!val.undefined())
                        {
                            value.push_back(val.value());
                        }
                        else
                        {
                            // Throw Error
                        }
                    }
                }
                else
                {
                    auto &val = env.getVariable(token.value());
                    if (!val.undefined())
                    {
                        value.push_back(val.value());
                    }
                    else
                    {
                        // Throw Error
                    }
                }
            }
            else if (token.is(Token::Kind::WGParameterStart))
            {
                auto function = value.back();

                size_t endToken = 0;
                bool found = false;
                for (size_t j = i + 1; j < tokens.size(); j++)
                {
                    if (tokens.at(j).is(Token::Kind::WGParameterEnd))
                    {
                        endToken = j;
                        found = true;
                        break;
                    }
                }

                if (!found)
                {
                    // throw error
                }
                
                auto args = parseArguments(std::vector<Token>(tokens.begin() + i + 1, tokens.begin() + endToken + 1), env);
                value.pop_back();

                if (function.__type__() == wg::Type::InbuiltFunction)
                {
                    if (function.__str__() == "write")
                    {
                        for (auto arg : args)
                        {
                            env << arg;
                            env << " ";
                        }
                        env << "\n";
                    }
                }
            }
            else
            {
                if (token.is(Token::Kind::WGInt))
                {
                    value.push_back(wg::Int(std::string(token.value())));
                }
                else if (token.is(Token::Kind::WGFloat))
                {
                    value.push_back(wg::Float(std::string(token.value())));
                }
                else if (token.is(Token::Kind::WGString))
                {
                    value.push_back(wg::String(std::string(token.value())));
                }
            }
        }
    }

    if (value.size())
    {
        return value.at(0);
    }

    return wg::Int(0);
}

void Environment::execute(AbstractSyntaxTree tree) noexcept
{
    if (tree.is(AbstractSyntaxTree::Kind::WGExpression))
    {

        operate(tree.expression(), *this);
        // std::vector<Token> tokens = tree.expression();
        // for (size_t i = 0; i < tokens.size(); i++)
        // {
        //     if (tokens.at(i).is(Token::Kind::WGParameterStart))
        //     {
        //         // Environment::mstdout << i << std::endl;
        //         Environment::mstdout << *(int*) Environment::getVariable(tokens.at(i + 1).value()).get()->__cvalue__() << std::endl;
        //     }
        // }
    }
    else if (tree.is(AbstractSyntaxTree::Kind::WGAssigment))
    {
        Environment::setVariable(tree.lhs().value(), operate(tree.expression(), *this));
        // Environment::mstdout << *(int *)Environment::getVariable(tree.lhs().value())->__cvalue__();
    }
}

void Environment::setVariable(std::string_view name, wg::Object value) noexcept
{
    for (size_t i = this->vmap.size() - 1; i-- > 0;)
    {
        auto scope = this->vmap.at(i);
        if (scope.find(name) != scope.end())
        {
            scope[name] = value;
            return;
        }
    }

    vmap[this->vmap.size() - 1][name] = value;
}

Variable Environment::getVariable(std::string_view name) noexcept
{
    for (size_t i = this->vmap.size() - 1; i-- > 0;)
    {
        auto scope = this->vmap.at(i);
        if (scope.find(name) != scope.end())
        {
            return Variable(scope, scope.find(name));
        }
    }

    auto scope = this->vmap.at(0);
    return Variable(scope, scope.find(name));
}

void Environment::deleteVariable(std::string_view name) noexcept
{
    for (size_t i = this->vmap.size() - 1; i-- > 0;)
    {
        auto scope = this->vmap.at(i);
        if (scope.find(name) != scope.end())
        {
            scope.erase(name);
            return;
        }
    }
}

void Environment::operator<<(wg::Object other) noexcept
{
    this->mstdout << other.__str__();
}

void Environment::operator<<(std::string other) noexcept
{
    this->mstdout << other;
}
