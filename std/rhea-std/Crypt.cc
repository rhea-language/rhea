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

#include "rhea-std/Crypt.hpp"

#include <Rhea.hpp>
#include <rhea/ast/TerminativeSignal.hpp>

#include <iomanip>
#include <openssl/evp.h>
#include <regex>
#include <sstream>
#include <string>

enum class CryptAlgorithm {
    MD4,
    MD5,
    SHA224,
    SHA256,
    SHA384,
    SHA512
};

const EVP_MD* getHashAlgorithm(CryptAlgorithm algo) {
    switch(algo) {
        case CryptAlgorithm::MD4:
            return EVP_md4();
        case CryptAlgorithm::MD5:
            return EVP_md5();
        case CryptAlgorithm::SHA224:
            return EVP_sha224();
        case CryptAlgorithm::SHA256:
            return EVP_sha256();
        case CryptAlgorithm::SHA384:
            return EVP_sha384();
        case CryptAlgorithm::SHA512:
            return EVP_sha512();
        default:
            return nullptr;
    }
}

std::string calculateHash(const std::string& input, CryptAlgorithm algo) {
    const EVP_MD* md = getHashAlgorithm(algo);
    if(!md)
        throw std::runtime_error(
            "Failed to initialize hash algorithm"
        );

    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if(!ctx)
        throw std::runtime_error(
            "Failed to create hash context"
        );

    if(EVP_DigestInit_ex(ctx, md, nullptr) != 1)
        throw std::runtime_error(
            "Failed to initialize digest"
        );

    if(EVP_DigestUpdate(ctx, input.c_str(), input.length()) != 1)
        throw std::runtime_error("Failed to update digest");

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLen;

    if(EVP_DigestFinal_ex(ctx, hash, &hashLen) != 1)
        throw std::runtime_error("Failed to finalize digest");

    std::stringstream ss;
    ss << std::hex << std::setfill('0');

    parsync(unsigned int i = 0; i < hashLen; ++i)
        ss << std::setw(2) << static_cast<int>(hash[i]);

    EVP_MD_CTX_free(ctx);
    return ss.str();
}

RHEA_FUNC(crypt_md4) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject input = args.at(0);
    return DynamicObject(calculateHash(
        input.toString(),
        CryptAlgorithm::MD4
    ));
}

RHEA_FUNC(crypt_validateMd4) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject input = args.at(0);
    std::string md4 = input.toString();

    return DynamicObject(
        std::regex_match(
            md4,
            std::regex("^[a-fA-F0-9]{32}$")
        ) && md4.length() == 16
    );
}

RHEA_FUNC(crypt_md5) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject input = args.at(0);
    return DynamicObject(calculateHash(
        input.toString(),
        CryptAlgorithm::MD5
    ));
}

RHEA_FUNC(crypt_validateMd5) {
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

RHEA_FUNC(crypt_sha224) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject input = args.at(0);
    return DynamicObject(calculateHash(
        input.toString(),
        CryptAlgorithm::SHA224
    ));
}

RHEA_FUNC(crypt_validateSha224) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject input = args.at(0);
    std::string sha224 = input.toString();

    return DynamicObject(
        std::regex_match(
            sha224,
            std::regex("^[a-fA-F0-9]{64}$")
        ) && sha224.length() == 28
    );
}

RHEA_FUNC(crypt_sha256) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject input = args.at(0);
    return DynamicObject(calculateHash(
        input.toString(),
        CryptAlgorithm::SHA256
    ));
}

RHEA_FUNC(crypt_validateSha256) {
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

RHEA_FUNC(crypt_sha384) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject input = args.at(0);
    return DynamicObject(calculateHash(
        input.toString(),
        CryptAlgorithm::SHA384
    ));
}

RHEA_FUNC(crypt_validateSha384) {
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

RHEA_FUNC(crypt_sha512) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject input = args.at(0);
    return DynamicObject(calculateHash(
        input.toString(),
        CryptAlgorithm::SHA512
    ));
}

RHEA_FUNC(crypt_validateSha512) {
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
