#include <ast/expression/TypeExpression.hpp>

DynamicObject TypeExpression::visit(SymbolTable& symbols) {
    DynamicObject value = this->expression->visit(symbols);

    if(value.isBool())
        return std::move(DynamicObject("bool"));
    else if(value.isString())
        return std::move(DynamicObject("string"));
    else if(value.isNumber())
        return std::move(DynamicObject("number"));
    else if(value.isFunction())
        return std::move(DynamicObject("function"));

    throw std::runtime_error("Unknown dynamic object type.");
}
