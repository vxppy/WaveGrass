#pragma once
#include <string_view>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace wg
{

    enum class Type
    {
        Object,
        Number,
        String,
        Boolean,
        Dictionary,
        Null,
        Array,
        InbuiltFunction,
        Reference
    };

    bool isType(wg::Type a, wg::Type b);

    class Object;
    class Int;
    class Float;
    class String;
    class Reference;
    class InbuiltFunction;
    class RawAccessToken;
    class Dictionary;
    class Null;

    class Object
    {
    public:
        Object() noexcept;
        ~Object() noexcept;

        wg::Type __type__() noexcept;
        virtual void *__cvalue__() noexcept;
        virtual std::string __str__() noexcept;
        virtual Object *__get__() noexcept;
        virtual bool __exists_property__(std::string) noexcept;
        virtual void __set_property__(std::string, Object) noexcept;
        virtual void __delete_property__(std::string) noexcept;
        virtual Object __get_property__(std::string) noexcept;
        virtual Object __add__(Object &) noexcept;
        virtual Object __mul__(Object &) noexcept;
        virtual Object __sub__(Object &) noexcept;
        virtual Object __div__(Object &) noexcept;

    protected:
        wg::Type type{wg::Type::Object};
    };

    class Int : public Object
    {
    public:
        Int() noexcept;
        Int(std::string) noexcept;
        Int(long long int) noexcept;
        Int(Int &) noexcept;

        void *__cvalue__() noexcept;
        std::string __str__() noexcept;
        Object __add__(Object &) noexcept;
        Object __mul__(Object &) noexcept;
        Object __sub__(Object &) noexcept;
        Object __div__(Object &) noexcept;

    private:
        void setValue(std::string) noexcept;
        long long int value{0};
    };

    class Float : public Object
    {
    public:
        Float() noexcept;
        Float(std::string) noexcept;
        Float(double) noexcept;
        Float(Float &) noexcept;

        void *__cvalue__() noexcept;
        std::string __str__() noexcept;
        Object __add__(Object &) noexcept;
        Object __mul__(Object &) noexcept;
        Object __sub__(Object &) noexcept;
        Object __div__(Object &) noexcept;

    private:
        void setValue(std::string) noexcept;
        double value{0};
    };

    class String : public Object
    {
    public:
        String() noexcept;
        String(std::string) noexcept;

        void *__cvalue__() noexcept;
        std::string __str__() noexcept;
        Object __add__(Object &) noexcept;

    private:
        std::string value;
    };

    class Dictionary : public Object
    {
    public:
        Dictionary() noexcept;
        std::string __str__() noexcept;
        bool __exists_property__(std::string) noexcept;
        void __set_property__(std::string, Object) noexcept;
        Object __get_property__(std::string) noexcept;
        void __delete_property__(std::string) noexcept;

    private:
        std::unordered_map<std::string, Object> value;
    };

    class InbuiltFunction : public Object
    {
    public:
        InbuiltFunction(std::string name) noexcept;
        std::string __str__() noexcept;

    private:
        std::string name;
    };

    class RawAccessToken : public Object
    {
    public:
        RawAccessToken(std::string name) noexcept;
        std::string __str__() noexcept;

    private:
        std::string name;
    };

    class Reference : public Object
    {
    public:
        Reference(Object *object) noexcept;
        Reference(Reference &) noexcept;
        Object *__get__() noexcept;

    private:
        std::shared_ptr<Object> ref;
    };

    class Null : public Object
    {
    public:
        Null() noexcept;
    };

    const wg::Null null = Null();
};