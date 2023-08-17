#pragma once
#include "wgtoken.h"
#include <vector>

class AbstractSyntaxTree {
    public:
        enum class Kind {
            WGExpression,
            WGCall,
            WGAssigment,
            WGValue,
            WGASTNull,
            WGError,
            WGEnd
        };
        
        AbstractSyntaxTree() noexcept: mKind{AbstractSyntaxTree::Kind::WGASTNull} {}

        AbstractSyntaxTree::Kind kind() { return mKind; }
        Token lhs() { return mLhs; }
        std::vector<Token> expression() { return mExpression; }
        std::vector<std::vector<Token>> params() { return mParams; }
        bool is(AbstractSyntaxTree::Kind kind) { return mKind == kind; }
        

        static AbstractSyntaxTree Error() noexcept;
        static AbstractSyntaxTree End() noexcept;
        static AbstractSyntaxTree AssignmentAST(Token lhs, std::vector<Token> expression, bool constant) noexcept;
        static AbstractSyntaxTree ValueAST(Token value) noexcept;
        static AbstractSyntaxTree ExpressionAST(std::vector<Token> expression) noexcept;


    private:
        AbstractSyntaxTree::Kind mKind;
        Token mLhs{Token::Kind::WGNone};
        std::vector<Token> mExpression;
        std::vector<std::vector<Token>> mParams;
        bool mConstant{false};
};