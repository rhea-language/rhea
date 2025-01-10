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

#include "n8std/Crypt.hpp"

#include <n8/ast/TerminativeSignal.hpp>

#include <iomanip>
#include <openssl/md4.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <regex>
#include <sstream>
#include <string>

std::string toHex(const unsigned char* hash, size_t length) {
    std::stringstream ss;
    for(size_t i = 0; i < length; ++i)
        ss << std::hex <<
            std::setw(2) <<
            std::setfill('0') <<
            static_cast<int>(hash[i]);

    return ss.str();
}

N8_FUNC(crypt_md5) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject input = args.at(0);
    std::string str = input.getString();

    MD5_CTX ctx;
    unsigned char hash[MD5_DIGEST_LENGTH];

    MD5_Init(&ctx);
    MD5_Update(
        &ctx,
        str.c_str(),
        str.size()
    );
    MD5_Final(hash, &ctx);

    return DynamicObject(
        toHex(hash, MD5_DIGEST_LENGTH)
    );
}

N8_FUNC(crypt_validateMd5) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject input = args.at(0);
    std::string md5 = input.toString();

    return DynamicObject(
        std::regex_match(
            md5,
            std::regex("^[a-fA-F0-9]{32}$")
        ) && md5.length() == 32
    );
}

N8_FUNC(crypt_sha256) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject input = args.at(0);
    std::string str = input.getString();

    SHA256_CTX ctx;
    unsigned char hash[SHA256_DIGEST_LENGTH];

    SHA256_Init(&ctx);
    SHA256_Update(
        &ctx,
        str.c_str(),
        str.size()
    );
    SHA256_Final(hash, &ctx);

    return DynamicObject(
        toHex(hash, SHA256_DIGEST_LENGTH)
    );
}

N8_FUNC(crypt_validateSha256) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject input = args.at(0);
    std::string sha256 = input.toString();

    return DynamicObject(
        std::regex_match(
            sha256,
            std::regex("^[a-fA-F0-9]{64}$")
        ) && sha256.length() == 64
    );
}

N8_FUNC(crypt_sha384) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject input = args.at(0);
    std::string str = input.getString();

    SHA512_CTX ctx;
    unsigned char hash[SHA384_DIGEST_LENGTH];

    SHA384_Init(&ctx);
    SHA384_Update(
        &ctx,
        str.c_str(),
        str.size()
    );
    SHA384_Final(hash, &ctx);

    return DynamicObject(
        toHex(hash, SHA384_DIGEST_LENGTH)
    );
}

N8_FUNC(crypt_validateSha384) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject input = args.at(0);
    std::string sha384 = input.toString();

    return DynamicObject(
        std::regex_match(
            sha384,
            std::regex("^[a-fA-F0-9]{96}$")
        ) && sha384.length() == 96
    );
}

N8_FUNC(crypt_sha512) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject input = args.at(0);
    std::string str = input.getString();

    SHA512_CTX ctx;
    unsigned char hash[SHA512_DIGEST_LENGTH];

    SHA512_Init(&ctx);
    SHA512_Update(
        &ctx,
        str.c_str(),
        str.size()
    );
    SHA512_Final(hash, &ctx);

    return DynamicObject(
        toHex(hash, SHA512_DIGEST_LENGTH)
    );
}

N8_FUNC(crypt_validateSha512) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject input = args.at(0);
    std::string sha512 = input.toString();

    return DynamicObject(
        std::regex_match(
            sha512,
            std::regex("^[a-fA-F0-9]{128}$")
        ) && sha512.length() == 128
    );
}
