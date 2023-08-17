#pragma once
#include <string_view>

class Token
{
public:
    enum class Kind
    {
        WGDelimiter,
        WGNull,
        WGKeyword,
        WGIdentifier,
        WGBoolean,
        WGInt,
        WGFloat,
        WGString,
        WGLeftParenthesis,
        WGRightParenthesis,
        WGLeftBracket,
        WGRightBracket,
        WGLeftBraces,
        WGRightBraces,
        WGSymbol,
        WGUnexpected,
        WGComment,
        WGSeperator,
        WGParameterStart,
        WGParameterEnd,
        WGEnd,
        WGNone
    };

    Token(Kind kind) noexcept : mKind{kind} {}

    Token(Kind kind, const char *beg, std::size_t len) noexcept : mKind{kind}, mLexeme(beg, len) {}

    Token(Kind kind, const char* beg, const char* end) noexcept
      : mKind{kind}, mLexeme(beg, std::distance(beg, end)) {}

    Kind kind() noexcept { return mKind; }

    std::string_view value() noexcept { return mLexeme; }

    bool is(Kind k1) const noexcept { return k1 == mKind; };

    bool isOneOf(Kind k1, Kind k2) const noexcept { return is(k1) || is(k2); }

    template <typename... Ts>
    bool isOneOf(Kind k1, Kind k2, Ts... ks) const noexcept {
        return is(k1) || isOneOf(k2, ks...);
    }

private:
    Kind mKind{};
    std::string_view mLexeme{};
};