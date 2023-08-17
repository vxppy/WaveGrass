#pragma once
#include "wgast.h"
#include "wgclasses.h"
#include <iostream>
#include <memory>
// #include <optional>

class Variable
{
    using Scope = std::unordered_map<std::string_view, wg::Object>;

public:
    Variable(Scope &sc, Scope::iterator i) : scope(sc), iter(i) {}

    bool undefined() { return iter == scope.end(); }
    void destroy() { scope.erase(iter); }
    wg::Object &value() { return iter->second; };

private:
    Scope &scope;
    Scope::iterator iter;
};

class Environment
{
public:
    Environment() noexcept : mstdout{std::cout.rdbuf()}, msdin{std::cin.rdbuf()}
    {
        this->vmap[0] = std::unordered_map<std::string_view, wg::Object>();

        wg::Dictionary console = wg::Dictionary();
        wg::InbuiltFunction writeFn = wg::InbuiltFunction("write");

        console.__set_property__("write", writeFn);

        this->vmap[0]["console"] = console;
    }

    Environment(std::ostream &out, std::istream &in) noexcept : mstdout{out.rdbuf()}, msdin{in.rdbuf()} {}

    void execute(AbstractSyntaxTree tree) noexcept;
    void setVariable(std::string_view, wg::Object) noexcept;
    Variable getVariable(std::string_view) noexcept;
    void deleteVariable(std::string_view) noexcept;
    // bool existVariable(std::string_view) noexcept;
    void operator<<(wg::Object) noexcept;
    void operator<<(std::string) noexcept;

private:
    std::ostream mstdout;
    std::istream msdin;
    std::vector<std::unordered_map<std::string_view, wg::Object>> vmap;
    // size_t scope{0};
};