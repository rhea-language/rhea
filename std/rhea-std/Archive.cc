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

#include "rhea-std/Archive.hpp"

#include <Rhea.hpp>

RHEA_FUNC(archive_zip_open) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject path = args.at(0),
        flags = args.at(1);
    std::vector<DynamicObject> values = {};

    if(!flags.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting number argument for flags, got " +
                flags.objectType()
        );

    int error = 0;
    zip_t* zip = zip_open(
        path.toString().c_str(),
        static_cast<int>(flags.getNumber()),
        &error
    );

    std::string key = RheaUtil::uniqueKey();
    if(zip == NULL || error != 0) {
        values.emplace_back(DynamicObject(key));
        values.emplace_back(DynamicObject(static_cast<float>(error)));

        return DynamicObject(std::make_shared<std::vector<DynamicObject>>(
            values
        ));
    }

    zipMap[key] = zip;

    values.emplace_back(DynamicObject(key));
    values.emplace_back(DynamicObject(
        static_cast<float>(error))
    );

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(
        values
    ));
}

RHEA_FUNC(archive_zip_close) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    auto it = zipMap.find(keyStr);
    if(it == zipMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Zip key does not exist"
        );

    zip_t* zip = it->second;
    if(zip == NULL)
        throw TerminativeThrowSignal(
            std::move(address),
            "Invalid zip handle"
        );

    int result = zip_close(zip);
    zip = nullptr;

    zipMap.erase(it);
    return DynamicObject(static_cast<float>(result));
}

RHEA_FUNC(archive_zip_compressionMethod) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject method = args.at(0),
        compress = args.at(1);

    if(!method.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Method parameter should be of number type, got " +
                method.objectType()
        );

    if(!compress.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Compress parameter should be of number type, got " +
                compress.objectType()
        );

    return DynamicObject(zip_compression_method_supported(
        static_cast<int>(method.getNumber()),
        static_cast<int>(compress.getNumber())
    ) == 1);
}

RHEA_FUNC(archive_zip_setPassword) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0),
        password = args.at(1);
    std::string keyStr = key.toString();

    if(zipMap.find(keyStr) == zipMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Zip key does not exist"
        );

    return DynamicObject(zip_set_default_password(
        zipMap[keyStr],
        password.toString().c_str()
    ) == 0);
}

RHEA_FUNC(archive_zip_setEncryption) {
    if(args.size() != 4)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 4 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0),
        index = args.at(1),
        method = args.at(2),
        password = args.at(3);
    std::string keyStr = key.toString();

    if(!index.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Index parameter should be of number type, got " +
                index.objectType()
        );

    if(!method.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Method parameter should be of number type, got " +
                method.objectType()
        );

    if(zipMap.find(keyStr) == zipMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Zip key does not exist"
        );

    return DynamicObject(zip_file_set_encryption(
        zipMap[keyStr],
        static_cast<uint16_t>(index.getNumber()),
        static_cast<uint16_t>(method.getNumber()),
        password.toString().c_str()
    ) == 0);
}

RHEA_FUNC(archive_zip_addFromFile) {
    if(args.size() != 5)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 5 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0),
        flags = args.at(1),
        compression = args.at(2),
        fileName = args.at(3),
        originFile = args.at(4);
    std::string keyStr = key.toString();

    if(!flags.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Flags parameter should be of number type, got " +
                flags.objectType()
        );

    if(!compression.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Compression parameter should be of number type, got " +
                compression.objectType()
        );

    auto it = zipMap.find(keyStr);
    if(it == zipMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Zip key does not exist"
        );

    zip_t* zip = it->second;
    if(zip == NULL)
        throw TerminativeThrowSignal(
            std::move(address),
            "Invalid zip handle"
        );

    zip_source_t* src = zip_source_file(
        zip,
        originFile.toString().c_str(),
        0, 0
    );

    std::vector<DynamicObject> values = {};
    if(src == NULL) {
        values.emplace_back(DynamicObject(0.0));
        values.emplace_back(DynamicObject(
            std::string(zip_strerror(zip))
        ));

        return DynamicObject(std::make_shared<std::vector<DynamicObject>>(
            values
        ));
    }

    zip_int64_t idx = zip_file_add(
        zip,
        fileName.toString().c_str(),
        src,
        static_cast<uint32_t>(flags.getNumber())
    );

    if(idx < 0) {
        zip_source_free(src);

        values.emplace_back(DynamicObject(
            static_cast<float>(idx)
        ));
        values.emplace_back(DynamicObject(
            std::string(zip_strerror(zip))
        ));
    }
    else {
        int comp = zip_set_file_compression(
            zip,
            idx,
            static_cast<int32_t>(compression.getNumber()),
            0
        );

        values.emplace_back(DynamicObject(
            static_cast<float>(idx)
        ));

        if(comp != 0)
            values.emplace_back(DynamicObject(
                std::string(zip_strerror(zip))
            ));
        else values.emplace_back(DynamicObject());
    }

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(
        values
    ));
}

RHEA_FUNC(archive_zip_addFromData) {
    return DynamicObject();
}

RHEA_FUNC(archive_zip_addFromDir) {
    return DynamicObject();
}

RHEA_FUNC(archive_zip_addFromString) {
    if(args.size() != 5)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 5 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0),
        flags = args.at(1),
        compression = args.at(2),
        fileName = args.at(3),
        buffer = args.at(4);
    std::string keyStr = key.toString();

    if(!flags.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Flags parameter should be of number type, got " +
                flags.objectType()
        );

    if(!compression.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Compression parameter should be of number type, got " +
                compression.objectType()
        );

    auto it = zipMap.find(keyStr);
    if(it == zipMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Zip key does not exist"
        );

    zip_t* zip = it->second;
    if(zip == NULL)
        throw TerminativeThrowSignal(
            std::move(address),
            "Invalid zip handle"
        );

    std::string buf = buffer.toString();
    size_t dataSize = buf.size();

    char* dataCopy = static_cast<char*>(malloc(dataSize));
    if(dataCopy == nullptr)
        throw TerminativeThrowSignal(
            std::move(address),
            "Memory allocation failed"
    );
    memcpy(dataCopy, buf.c_str(), dataSize);

    zip_source_t* src = zip_source_buffer(
        zip,
        dataCopy,
        dataSize,
        1
    );

    std::vector<DynamicObject> values = {};
    if(src == NULL) {
        free(dataCopy);

        values.emplace_back(DynamicObject(0.0));
        values.emplace_back(DynamicObject(
            std::string(zip_strerror(zip))
        ));

        return DynamicObject(std::make_shared<std::vector<DynamicObject>>(
            values
        ));
    }

    zip_int64_t idx = zip_file_add(
        zip,
        fileName.toString().c_str(),
        src,
        static_cast<uint32_t>(flags.getNumber())
    );

    if(idx < 0) {
        zip_source_free(src);

        values.emplace_back(DynamicObject(
            static_cast<float>(idx)
        ));
        values.emplace_back(DynamicObject(
            std::string(zip_strerror(zip))
        ));
    }
    else {
        int comp = zip_set_file_compression(
            zip,
            idx,
            static_cast<int32_t>(compression.getNumber()),
            0
        );

        values.emplace_back(DynamicObject(
            static_cast<float>(idx)
        ));

        if(comp != 0)
            values.emplace_back(DynamicObject(
                std::string(zip_strerror(zip))
            ));
        else values.emplace_back(DynamicObject());
    }

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(
        values
    ));
}

RHEA_FUNC(archive_zip_deleteFile) {
    return DynamicObject();
}

RHEA_FUNC(archive_zip_deleteDir) {
    return DynamicObject();
}
