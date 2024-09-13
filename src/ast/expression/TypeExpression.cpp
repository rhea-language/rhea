#include <ast/expression/TypeExpression.hpp>

DynamicObject TypeExpression::visit(SymbolTable& symbols) {
    DynamicObject value = this->expression->visit(symbols);

    if(value.isArray())
        return std::move(DynamicObject("array"));
    else if(value.isBool())
        return std::move(DynamicObject("bool"));
    else if(value.isString())
        return std::move(DynamicObject("string"));
    else if(value.isNumber())
        return std::move(DynamicObject("number"));
    else if(value.isNil())
        return std::move(DynamicObject("nil"));
    else if(value.isFunction())
        return std::move(DynamicObject("function"));
    else if(value.isRegex())
        return std::move(DynamicObject("regex"));

    throw std::runtime_error("Unknown dynamic object type.");
}
