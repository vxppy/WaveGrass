#include "wgast.h"

AbstractSyntaxTree AbstractSyntaxTree::Error() noexcept
{
    AbstractSyntaxTree tree = AbstractSyntaxTree();
    tree.mKind = AbstractSyntaxTree::Kind::WGError;

    return tree;
}

AbstractSyntaxTree AbstractSyntaxTree::End() noexcept
{
    AbstractSyntaxTree tree = AbstractSyntaxTree();
    tree.mKind = AbstractSyntaxTree::Kind::WGEnd;

    return tree;
}


AbstractSyntaxTree AbstractSyntaxTree::AssignmentAST(Token lhs, std::vector<Token> expression, bool constant) noexcept
{
    AbstractSyntaxTree tree = AbstractSyntaxTree();

    tree.mKind = AbstractSyntaxTree::Kind::WGAssigment;
    tree.mLhs = lhs;
    tree.mExpression = expression;
    tree.mConstant = constant;

    return tree;
}

AbstractSyntaxTree AbstractSyntaxTree::ValueAST(Token lhs) noexcept
{
    AbstractSyntaxTree tree = AbstractSyntaxTree();

    tree.mKind = AbstractSyntaxTree::Kind::WGValue;
    tree.mLhs = lhs;

    return tree;
}


AbstractSyntaxTree AbstractSyntaxTree::ExpressionAST(std::vector<Token> expression) noexcept
{
    AbstractSyntaxTree tree = AbstractSyntaxTree();

    tree.mKind = AbstractSyntaxTree::Kind::WGExpression;
    tree.mExpression = expression;

    return tree;
}