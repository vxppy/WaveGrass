#include "wgclasses.h"
// #include <iostream>

// ////////////////////////////////////////////////////////////////////////////////////
// /////////////////////////////////// HELPERS ////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////

int getNumberFromString(const char c)
{
    switch (c)
    {
    case '0':
        return 0;
    case '1':
        return 1;
    case '2':
        return 2;
    case '3':
        return 3;
    case '4':
        return 4;
    case '5':
        return 5;
    case '6':
        return 6;
    case '7':
        return 7;
    case '8':
        return 8;
    case '9':
        return 9;
    }

    return -1;
}

// ////////////////////////////////////////////////////////////////////////////////////
// //////////////////////////////////// OBJECT ////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////

bool wg::isType(wg::Type a, wg::Type b) { return a == b; }

wg::Object::Object() noexcept {}
wg::Object::~Object() noexcept {}

wg::Type wg::Object::__type__() noexcept { return this->type; }

void *wg::Object::__cvalue__() noexcept { return nullptr; }

std::string wg::Object::__str__() noexcept { return "[Object object]"; }

wg::Object *wg::Object::__get__() noexcept { return nullptr; }

wg::Object wg::Object::__add__(wg::Object &other) noexcept { return wg::null; }

wg::Object wg::Object::__sub__(wg::Object &other) noexcept { return wg::null; }

wg::Object wg::Object::__mul__(wg::Object &other) noexcept { return wg::null; }

wg::Object wg::Object::__div__(wg::Object &other) noexcept { return wg::null; }

bool wg::Object::__exists_property__(std::string name) noexcept { return false; }

void wg::Object::__set_property__(std::string name, wg::Object value) noexcept {}

void wg::Object::__delete_property__(std::string name) noexcept {}

wg::Object wg::Object::__get_property__(std::string name) noexcept { return wg::null; }

// ////////////////////////////////////////////////////////////////////////////////////
// //////////////////////////////////// INT ///////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////

void wg::Int::setValue(std::string value) noexcept
{
    this->type = wg::Type::Number;
    for (const char c : value)
    {
        this->value = this->value * 10 + getNumberFromString(c);
    }
}

wg::Int::Int() noexcept
{
    this->type = wg::Type::Number;
    this->value = 0;
}

wg::Int::Int(wg::Int &other) noexcept
{
    this->type = wg::Type::Number;
    this->value = other.value;
}

wg::Int::Int(std::string value) noexcept
{
    this->type = wg::Type::Number;
    wg::Int::setValue(value);
}

wg::Int::Int(long long int value) noexcept
{
    this->type = wg::Type::Number;
    this->value = value;
}

void *wg::Int::__cvalue__() noexcept
{
    return &this->value;
}

std::string wg::Int::__str__() noexcept
{
    return std::to_string(this->value);
}

wg::Object wg::Int::__add__(wg::Object &other) noexcept
{
    if (wg::isType(other.__type__(), wg::Type::Number))
    {
        return wg::Int(*(int *)other.__cvalue__() + this->value);
    }

    return wg::null;
}

wg::Object wg::Int::__sub__(wg::Object &other) noexcept
{
    if (wg::isType(other.__type__(), wg::Type::Number))
    {
        return wg::Int(this->value - *(int *)other.__cvalue__());
    }

    return wg::null;
}

wg::Object wg::Int::__mul__(wg::Object &other) noexcept
{
    if (wg::isType(other.__type__(), wg::Type::Number))
    {
        return wg::Int(*(int *)other.__cvalue__() * this->value);
    }

    return wg::null;
}

wg::Object wg::Int::__div__(wg::Object &other) noexcept
{
    if (wg::isType(other.__type__(), wg::Type::Number))
    {
        double value = this->value / (double)*(long long int *)other.__cvalue__();

        if (std::abs(value - (long long int)value) < 1e-64)
        {
            return wg::Int((long long int)value);
        }
        return wg::Float(value);
    }

    return wg::null;
}

// ////////////////////////////////////////////////////////////////////////////////////
// /////////////////////////////////// FLOAT //////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////

void wg::Float::setValue(std::string value) noexcept
{
    this->type = wg::Type::Number;
    long div = 0;
    for (const char c : value)
    {
        if (c == '.')
        {
            div = 1;
        }

        if (div != 0)
            div *= 10;
        wg::Float::value = wg::Float::value * 10 + getNumberFromString(c);
    }

    wg::Float::value /= div;
}

wg::Float::Float() noexcept
{
    this->type = wg::Type::Number;
    wg::Float::value = 0;
}

wg::Float::Float(wg::Float &other) noexcept
{
    this->type = wg::Type::Number;
    this->value = other.value;
}

wg::Float::Float(std::string value) noexcept
{
    this->type = wg::Type::Number;
    wg::Float::setValue(value);
}

wg::Float::Float(double value) noexcept
{
    this->type = wg::Type::Number;
    wg::Float::value = value;
}

void *wg::Float::__cvalue__() noexcept
{
    return &this->value;
}

std::string wg::Float::__str__() noexcept
{
    return std::to_string(this->value);
}

wg::Object wg::Float::__add__(wg::Object &other) noexcept
{
    if (wg::isType(other.__type__(), wg::Type::Number))
    {
        return wg::Float(*(int *)other.__cvalue__() + this->value);
    }

    return wg::Float(0);
}

wg::Object wg::Float::__sub__(wg::Object &other) noexcept
{
    if (wg::isType(other.__type__(), wg::Type::Number))
    {
        return wg::Float(*(int *)other.__cvalue__() - this->value);
    }

    return wg::Float(0);
}

wg::Object wg::Float::__mul__(wg::Object &other) noexcept
{
    if (wg::isType(other.__type__(), wg::Type::Number))
    {
        return wg::Float(*(int *)other.__cvalue__() * this->value);
    }

    return wg::Float(0);
}

wg::Object wg::Float::__div__(wg::Object &other) noexcept
{
    if (wg::isType(other.__type__(), wg::Type::Number))
    {
        return wg::Float(*(int *)other.__cvalue__() / this->value);
    }

    return wg::Float(0);
}

// ////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////// STRINGS /////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////

wg::String::String() noexcept
{
    this->value = std::string("");
}

wg::String::String(std::string value) noexcept
{
    this->value = value;
}

void *wg::String::__cvalue__() noexcept
{
    return &this->value;
}

std::string wg::String::__str__() noexcept
{
    return this->value;
}

wg::Object wg::String::__add__(wg::Object &other) noexcept
{
    return wg::String(this->value + other.__str__());
}

// ////////////////////////////////////////////////////////////////////////////////////
// /////////////////////////////////// DICT ///////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////

wg::Dictionary::Dictionary() noexcept
{
    this->type = wg::Type::Dictionary;
}

std::string wg::Dictionary::__str__() noexcept
{
    return "[Object Object]";
}

void wg::Dictionary::__set_property__(std::string name, wg::Object value) noexcept
{
    this->value[name] = wg::Reference(&value);
}

bool wg::Dictionary::__exists_property__(std::string name) noexcept
{
    return this->value.find(name) != this->value.end();
}

wg::Object wg::Dictionary::__get_property__(std::string name) noexcept
{
    if (this->value.find(name) != this->value.end())
    {
        return this->value[name];
    }
    return wg::null;
}

void wg::Dictionary::__delete_property__(std::string name) noexcept
{
    if (this->__exists_property__(name))
    {
        this->value.erase(name);
    }
}

// ////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////// INBUILT /////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////

wg::InbuiltFunction::InbuiltFunction(std::string name) noexcept
{
    this->name = name;
    this->type = wg::Type::InbuiltFunction;
}

std::string wg::InbuiltFunction::__str__() noexcept
{
    return "[Function " + this->name + "]";
}

// ////////////////////////////////////////////////////////////////////////////////////
// //////////////////////////////// RawAccess /////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////

wg::RawAccessToken::RawAccessToken(std::string name) noexcept
{
    this->name = name;
}

std::string wg::RawAccessToken::__str__() noexcept
{
    return "[RawToken " + this->name + "]";
}

// ////////////////////////////////////////////////////////////////////////////////////
// //////////////////////////////// REFERENCE /////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////

wg::Reference::Reference(wg::Object *object) noexcept
{
    this->ref = std::shared_ptr<wg::Object>(object);
    this->type = wg::Type::Reference;
}

wg::Reference::Reference(Reference &other) noexcept
{
    this->ref = std::shared_ptr<wg::Object>(other.__get__());
}

wg::Object *wg::Reference::__get__() noexcept
{
    return this->ref.get();
}

// ////////////////////////////////////////////////////////////////////////////////////
// /////////////////////////////////// NULL ///////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////

wg::Null::Null() noexcept
{
    this->type = wg::Type::Null;
}