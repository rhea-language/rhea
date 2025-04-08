/*
 * Copyright (c) 2024 - Nathanne Isip
 * This file is part of Rhea.
 * 
 * Rhea is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 * 
 * Rhea is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Rhea. If not, see <https://www.gnu.org/licenses/>.
 */

#include <rhea/ast/expression/LoopExpression.hpp>

DynamicObject LoopExpression::visit(SymbolTable& symbols) {
    DynamicObject value;
    this->initial->visit(symbols);

    while(this->condition->visit(symbols).booleanEquivalent()) {
        try {
            value = this->body->visit(symbols);
        }
        catch(const TerminativeBreakSignal& breakSig) {
            break;
        }
        catch(const TerminativeContinueSignal& continueSig) {
            this->postexpr->visit(symbols);
            continue;
        }

        this->postexpr->visit(symbols);
    }

    return value;
}
