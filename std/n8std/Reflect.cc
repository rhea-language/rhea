/*
 * Copyright (c) 2024 - Nathanne Isip
 * This file is part of N8.
 * 
 * N8 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 * 
 * N8 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with N8. If not, see <https://www.gnu.org/licenses/>.
 */

#include "n8std/Reflect.hpp"

#include <n8/ast/ASTNodeException.hpp>
#include <n8/ast/TerminativeSignal.hpp>
#include <n8/core/Runtime.hpp>
#include <n8/parser/Parser.hpp>
#include <n8/parser/ParserException.hpp>
#include <n8/parser/Tokenizer.hpp>
#include <n8/util/Print.hpp>

N8_FUNC(reflect_get) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject name = args.at(0);
    const std::string symName = name.toString();

    return symtab.getSymbol(
        std::move(address),
        symName
    );
}

N8_FUNC(reflect_has) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject name = args.at(0);
    const std::string symName = name.toString();

    return DynamicObject(symtab.hasSymbol(symName));
}

N8_FUNC(reflect_typeOf) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject name = args.at(0);
    const std::string symName = name.toString();

    return symtab.getSymbol(
        std::move(address),
        symName
    ).objectType();
}

N8_FUNC(reflect_remove) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject name = args.at(0);
    symtab.removeSymbol(name.toString());

    return {};
}

N8_FUNC(reflect_invoke) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject name = args.at(0),
        params = args.at(1);

    if(!params.isArray())
        throw TerminativeThrowSignal(
            std::move(address),
            "Parameters must be of array type"
        );

    const std::string symName = name.toString();
    DynamicObject callable = symtab.getSymbol(
        std::move(address),
        symName
    );

    return callable.callFromNative(
        std::move(address),
        symtab,
        *params.getArray()
    );
}

N8_FUNC(reflect_exec) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject source = args.at(0);
    try {
        Tokenizer tokenizer(
            source.getString(),
            "<reflect.exec::" +
                address->toString() + ">"
        );
        tokenizer.scan();

        Parser parser(tokenizer.getTokens());
        parser.parse();

        DynamicObject value;
        for(const auto& statement : parser.getGlobalStatements())
            value = statement->visit(symtab);
        return value;
    }
    catch(const std::system_error& exc) {
        symtab.waitForTasks();
        Runtime::cleanUp();

        N8Util::printError("[\u001b[1;31mSystem Error\u001b[0m]: \u001b[3;37m");
        N8Util::printError(exc.what());
        N8Util::printError("\u001b[0m\r\n");
    }
    catch(const ASTNodeException& nodeExc) {
        symtab.waitForTasks();
        Runtime::cleanUp();

        N8Util::printError("[\u001b[1;31mRuntime Error\u001b[0m]: \u001b[3;37m");
        N8Util::printError(nodeExc.what());
        N8Util::printError("\u001b[0m\r\n                 ");
        N8Util::printError(nodeExc.getAddress()->toString());
        N8Util::printError("\r\n");
    }
    catch(const LexicalAnalysisException& lexAnlExc) {
        symtab.waitForTasks();
        Runtime::cleanUp();

        N8Util::printError("[\u001b[1;31mLexical Error\u001b[0m]:\r\n\t");
        N8Util::printError(lexAnlExc.what());
        N8Util::printError("\r\n");
    }
    catch(const ParserException& parserExc) {
        symtab.waitForTasks();
        Runtime::cleanUp();

        N8Util::printError("[\u001b[1;31mParser Error\u001b[0m]:  \u001b[3;37m");
        N8Util::printError(parserExc.what());
        N8Util::printError("\u001b[0m\r\n                 ");
        N8Util::printError(parserExc.getAddress()->toString());
        N8Util::printError("\r\n");
    }
    catch(const TerminativeBreakSignal& breakExc) {
        symtab.waitForTasks();
        Runtime::cleanUp();

        N8Util::printError(
            "[\u001b[1;31mRuntime Error\u001b[0m]: "
            "\u001b[3;37mInvalid break statement signal caught.\u001b[0m"
            "\r\n                 "
        );
        N8Util::printError(breakExc.getAddress().toString());
        N8Util::printError("\r\n");
    }
    catch(const TerminativeContinueSignal& continueExc) {
        symtab.waitForTasks();
        Runtime::cleanUp();

        N8Util::printError(
            "[\u001b[1;31mRuntime Error\u001b[0m]: "
            "\u001b[3;37mInvalid continue statement signal caught.\u001b[0m"
            "\r\n                 "
        );
        N8Util::printError(continueExc.getAddress().toString());
        N8Util::printError("\r\n");
    }
    catch(const TerminativeReturnSignal& retExc) {
        symtab.waitForTasks();
        Runtime::cleanUp();

        return retExc.getObject();
    }
    catch(const TerminativeThrowSignal& throwExc) {
        symtab.waitForTasks();
        Runtime::cleanUp();

        N8Util::printError("[\u001b[1;31mUncaught Error\u001b[0m]: \u001b[3;37m");
        N8Util::printError(throwExc.getObject().toString());
        N8Util::printError("\u001b[0m\r\n                  ");
        N8Util::printError(throwExc.getAddress()->toString());
        N8Util::printError("\r\n");
    }
    catch(const std::exception& exc) {
        symtab.waitForTasks();
        Runtime::cleanUp();

        N8Util::printError("[\u001b[1;31mRuntime Error\u001b[0m]: \u001b[3;37m");
        N8Util::printError(exc.what());
        N8Util::printError("\u001b[0m\r\n");
    }

    return {};
}

N8_FUNC(reflect_isTest) {
    return DynamicObject(Runtime::isTestMode());
}

N8_FUNC(reflect_isUnsafe) {
    return DynamicObject(Runtime::isUnsafeMode());
}
