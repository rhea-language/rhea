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

#include <rhea/ast/expression/FunctionDeclarationExpression.hpp>
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

RHEA_FUNC(archive_zip_addFromData) {
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

    std::shared_ptr<std::vector<DynamicObject>> buf = buffer.getArray();
    size_t dataSize = buf->size();

    uint8_t* data = static_cast<uint8_t*>(malloc(dataSize));
    for(size_t i = 0; i < dataSize; i++) {
        DynamicObject obj = buf->at(i);
        if(!obj.isNumber())
            throw TerminativeThrowSignal(
                std::move(address),
                "Buffer parameter must be an array of numbers"
            );

        data[i] = static_cast<uint8_t>(obj.getNumber());
    }

    zip_source_t* src = zip_source_buffer(
        zip,
        data,
        dataSize,
        1
    );

    std::vector<DynamicObject> values = {};
    if(src == NULL) {
        free(data);

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
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0),
        fileName = args.at(1);
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

    int64_t index = zip_name_locate(
        zip,
        fileName.toString().c_str(),
        0
    );

    return index < 0 ?
        DynamicObject(false) :
        DynamicObject(zip_delete(zip, index) == 0);
}

RHEA_FUNC(archive_zip_deleteDir) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0),
        dirName = args.at(1);
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

    int64_t num_entries = zip_get_num_entries(zip, 0);
    for(int64_t i = num_entries - 1; i >= 0; i--) {
        const char *name = zip_get_name(zip, i, 0);
        if(!name)
            continue;

        if(std::string(name).rfind(
            dirName.toString()
        ) != 0) continue;

        if(zip_delete(zip, i) != 0)
            return DynamicObject(false);
    }

    return DynamicObject(true);
}

RHEA_FUNC(archive_zip_getIndex) {
    if(args.size() != 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 3 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0),
        fileName = args.at(1),
        flags = args.at(2);
    std::string keyStr = key.toString();

    if(!flags.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Flags parameter should be of number type, got " +
                flags.objectType()
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

    return DynamicObject(static_cast<float>(
        zip_name_locate(
            zip,
            fileName.toString().c_str(),
            static_cast<int>(flags.getNumber())
        )
    ));
}

RHEA_FUNC(archive_zip_hasFile) {
    if(args.size() != 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 3 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0),
        fileName = args.at(1),
        flags = args.at(2);
    std::string keyStr = key.toString();

    if(!flags.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Flags parameter should be of number type, got " +
                flags.objectType()
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

    return DynamicObject(zip_name_locate(
        zip,
        fileName.toString().c_str(),
        static_cast<int>(flags.getNumber())
    ) >= 0);
}

RHEA_FUNC(archive_zip_renameFile) {
    if(args.size() != 4)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 4 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0),
        index = args.at(1),
        newName = args.at(2),
        flags = args.at(3);
    std::string keyStr = key.toString();

    if(!index.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Index parameter should be of number type, got " +
                index.objectType()
        );

    if(!flags.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Flags parameter should be of number type, got " +
                flags.objectType()
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

    return DynamicObject(zip_file_rename(
        zip,
        static_cast<uint64_t>(index.getNumber()),
        newName.toString().c_str(),
        static_cast<int>(flags.getNumber())
    ) == 0);
}

RHEA_FUNC(archive_zip_entryCount) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0),
        flags = args.at(1);
    std::string keyStr = key.toString();

    if(!flags.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Flags parameter should be of number type, got " +
                flags.objectType()
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

    return DynamicObject(static_cast<float>(
        zip_get_num_entries(
            zip,
            static_cast<int>(flags.getNumber())
        )
    ));
}

RHEA_FUNC(archive_zip_listEntries) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0),
        flags = args.at(1);
    std::string keyStr = key.toString();

    if(!flags.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Flags parameter should be of number type, got " +
                flags.objectType()
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

    std::shared_ptr<std::vector<DynamicObject>> entries =
        std::make_shared<std::vector<DynamicObject>>();
    size_t entryCount = zip_get_num_entries(zip, 0);

    for(size_t i = 0; i < entryCount; i++) {
        const char* name = zip_get_name(
            zip,
            i,
            static_cast<int>(flags.getNumber())
        );

        if(!name)
            continue;

        entries->emplace_back(DynamicObject(
            std::string(name)
        ));
    }

    return DynamicObject(entries);
}

RHEA_FUNC(archive_zip_setComment) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0),
        comment = args.at(1);
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

    std::string commentStr = comment.toString();
    return DynamicObject(zip_set_archive_comment(
        zip,
        commentStr.c_str(),
        commentStr.size()
    ) == 0);
}

RHEA_FUNC(archive_zip_getComment) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0),
        flags = args.at(1);
    std::string keyStr = key.toString();

    if(!flags.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Flags parameter should be of number type, got " +
                flags.objectType()
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

    int lenp = 0;
    return DynamicObject(std::string(
        zip_get_archive_comment(
            zip,
            &lenp,
            static_cast<int>(flags.getNumber())
        )
    ));
}

RHEA_FUNC(archive_zip_getFileComment) {
    if(args.size() != 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 3 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0),
        index = args.at(1),
        flags = args.at(2);
    std::string keyStr = key.toString();

    if(!index.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Index parameter should be of number type, got " +
                index.objectType()
        );

    if(!flags.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Flags parameter should be of number type, got " +
                flags.objectType()
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

    int lenp = 0;
    return DynamicObject(std::string(zip_get_file_comment(
        zip,
        static_cast<uint64_t>(index.getNumber()),
        &lenp,
        static_cast<int>(flags.getNumber())
    )));
}

RHEA_FUNC(archive_zip_setFileComment) {
    if(args.size() != 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 3 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0),
        index = args.at(1),
        comment = args.at(2);
    std::string keyStr = key.toString();

    if(!index.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Index parameter should be of number type, got " +
                index.objectType()
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

    std::string commentStr = comment.toString();
    return DynamicObject(zip_set_file_comment(
        zip,
        static_cast<uint64_t>(index.getNumber()),
        commentStr.c_str(),
        commentStr.size()
    ) == 0);
}

RHEA_FUNC(archive_zip_fileDosTime) {
    if(args.size() != 5)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 5 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0),
        index = args.at(1),
        dosTime = args.at(2),
        dosDate = args.at(3),
        flags = args.at(4);
    std::string keyStr = key.toString();

    if(!index.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Index parameter should be of number type, got " +
                index.objectType()
        );

    if(!dosTime.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "DOS time parameter should be of number type, got " +
                dosTime.objectType()
        );

    if(!dosDate.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "DOS date parameter should be of number type, got " +
                dosDate.objectType()
        );

    if(!index.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Index parameter should be of number type, got " +
                index.objectType()
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

    return DynamicObject(zip_file_set_dostime(
        zip,
        static_cast<uint64_t>(index.getNumber()),
        static_cast<uint16_t>(dosTime.getNumber()),
        static_cast<uint16_t>(dosDate.getNumber()),
        static_cast<int>(flags.getNumber())
    ) == 0);
}

RHEA_FUNC(archive_zip_discard) {
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

    zip_discard(zip);
    return DynamicObject();
}

RHEA_FUNC(archive_zip_registerProgressCallback) {
    if(args.size() != 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 3 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0),
        precision = args.at(1),
        callback = args.at(2);
    std::string keyStr = key.toString();

    if(!precision.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Precision parameter should be of number type, got " +
                precision.objectType()
        );

    if(!callback.isFunction())
        throw TerminativeThrowSignal(
            std::move(address),
            "Callback parameter should be of function type, got " +
                callback.objectType()
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

    static SymbolTable& symbols = symtab;
    DynamicObject* cbPtr = new DynamicObject(callback);

    zip_register_progress_callback_with_state(
        zip,
        static_cast<double>(precision.getNumber()),
        [](zip_t* cbz, double step, void* data) {
            DynamicObject* cbObj = static_cast<DynamicObject*>(data);

            std::vector<DynamicObject> params;
            params.emplace_back(DynamicObject(
                static_cast<float>(step)
            ));

            cbObj->getCallable()->call(symbols, params);
        },
        [](void* data) {
            DynamicObject* cbObj = static_cast<DynamicObject*>(data);
            delete cbObj;
        },
        static_cast<void*>(cbPtr)
    );

    return DynamicObject();
}

RHEA_FUNC(archive_zip_registerCancelCallback) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0),
        callback = args.at(1);
    std::string keyStr = key.toString();

    if(!callback.isFunction())
        throw TerminativeThrowSignal(
            std::move(address),
            "Callback parameter should be of function type, got " +
                callback.objectType()
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

    static SymbolTable& symbols = symtab;
    DynamicObject* cbPtr = new DynamicObject(callback);

    zip_register_cancel_callback_with_state(
        zip,
        [](zip_t* cbz, void* data) {
            DynamicObject* cbObj = static_cast<DynamicObject*>(data);
            cbObj->getCallable()->call(symbols, {});

            return 0;
        },
        [](void* data) {
            DynamicObject* cbObj = static_cast<DynamicObject*>(data);
            delete cbObj;
        },
        static_cast<void*>(cbPtr)
    );

    return DynamicObject();
}

RHEA_FUNC(archive_zip_readAsData) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0),
        fileName = args.at(1);
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

    std::shared_ptr<std::vector<DynamicObject>> buf =
        std::make_shared<std::vector<DynamicObject>>();

    std::string actualFileNameStr = fileName.toString();
    const char* actualFileName = actualFileNameStr.c_str();

    struct zip_stat st;
    if(zip_stat(
        zip,
        actualFileName,
        0, &st
    ) != 0) throw TerminativeThrowSignal(
        std::move(address),
        "Cannot find specified file name in zip: " +
            fileName.toString()
    );

    zip_int64_t idx = zip_name_locate(
        zip,
        actualFileName,
        0
    );
    if(idx < 0)
        throw TerminativeThrowSignal(
            std::move(address),
            "Cannot read specified file name in zip: " +
                fileName.toString()
        );

    zip_file_t *zf = zip_fopen_index(zip, idx, 0);
    if(!zf) throw TerminativeThrowSignal(
        std::move(address),
        "Cannot read specified file name in zip: " +
            fileName.toString()
    );

    uint8_t *buffer = static_cast<uint8_t*>(malloc(st.size));
    if(!buffer) {
        zip_fclose(zf);
        throw TerminativeThrowSignal(
            std::move(address),
            "Failed to allocate memory buffer"
        );
    }

    int64_t bytes_read = zip_fread(zf, buffer, st.size);
    if(bytes_read < 0 ||
        static_cast<uint64_t>(bytes_read) != st.size
    ) {
        free(buffer);
        zip_fclose(zf);

        throw TerminativeThrowSignal(
            std::move(address),
            "Cannot read specified file name in zip: " +
                fileName.toString()
        );
    }

    for(size_t i = 0; i < st.size; i++)
        buf->emplace_back(DynamicObject(
            static_cast<float>(buffer[i])
        ));

    free(buffer);
    zip_fclose(zf);

    return DynamicObject(buf);
}

RHEA_FUNC(archive_zip_readAsString) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0),
        fileName = args.at(1);
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

    std::string actualFileNameStr = fileName.toString();
    const char* actualFileName = actualFileNameStr.c_str();
    
    struct zip_stat st;
    if(zip_stat(
        zip,
        actualFileName,
        0, &st
    ) != 0) throw TerminativeThrowSignal(
        std::move(address),
        "Cannot find specified file name in zip: " +
            fileName.toString()
    );

    zip_int64_t idx = zip_name_locate(
        zip,
        actualFileName,
        0
    );
    if(idx < 0)
        throw TerminativeThrowSignal(
            std::move(address),
            "Cannot read specified file name in zip: " +
                fileName.toString()
        );

    zip_file_t *zf = zip_fopen_index(zip, idx, 0);
    if(!zf) throw TerminativeThrowSignal(
        std::move(address),
        "Cannot read specified file name in zip: " +
            fileName.toString()
    );

    uint8_t *buffer = static_cast<uint8_t*>(malloc(st.size));
    if(!buffer) {
        zip_fclose(zf);
        throw TerminativeThrowSignal(
            std::move(address),
            "Failed to allocate memory buffer"
        );
    }

    int64_t bytes_read = zip_fread(zf, buffer, st.size);
    if(bytes_read < 0 ||
        static_cast<uint64_t>(bytes_read) != st.size
    ) {
        free(buffer);
        zip_fclose(zf);

        throw TerminativeThrowSignal(
            std::move(address),
            "Cannot read specified file name in zip: " +
                fileName.toString()
        );
    }

    std::string buf;
    for(size_t i = 0; i < st.size; i++)
        buf += static_cast<char>(buffer[i]);

    zip_fclose(zf);
    return DynamicObject(buf);
}

RHEA_FUNC(archive_zip_fetchInfo) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0),
        fileName = args.at(1);
    std::string keyStr = key.toString();

    if(zipMap.find(keyStr) == zipMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Zip key does not exist"
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

    std::string actualFileNameStr = fileName.toString();
    const char* actualFileName = actualFileNameStr.c_str();

    struct zip_stat st;
    if(zip_stat(
        zip,
        actualFileName,
        0, &st
    ) != 0) throw TerminativeThrowSignal(
        std::move(address),
        "Cannot find specified file name in zip: " +
            fileName.toString()
    );

    std::shared_ptr<std::vector<DynamicObject>> infos =
        std::make_shared<std::vector<DynamicObject>>();

    infos->emplace_back(DynamicObject(static_cast<float>(st.index)));
    infos->emplace_back(DynamicObject(static_cast<float>(st.size)));
    infos->emplace_back(DynamicObject(static_cast<float>(st.comp_size)));
    infos->emplace_back(DynamicObject(static_cast<float>(st.mtime)));
    infos->emplace_back(DynamicObject(static_cast<float>(st.crc)));
    infos->emplace_back(DynamicObject(static_cast<float>(st.comp_method)));
    infos->emplace_back(DynamicObject(static_cast<float>(st.encryption_method)));

    return DynamicObject(infos);
}

RHEA_FUNC(archive_zip_unchange) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 argument, got " +
                std::to_string(args.size())
        );

    DynamicObject key = args.at(0),
        index = args.at(1);
    std::string keyStr = key.toString();

    if(!index.isNumber())
        throw TerminativeThrowSignal(
            std::move(address),
            "Index argument must be of number type, got " +
                index.objectType()
        );

    if(zipMap.find(keyStr) == zipMap.end())
        throw TerminativeThrowSignal(
            std::move(address),
            "Zip key does not exist"
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

    return DynamicObject(zip_unchange(
        zip,
        static_cast<uint64_t>(index.getNumber())
    ) == 0);
}

RHEA_FUNC(archive_zip_unchangeAll) {
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

    return DynamicObject(zip_unchange_all(zip) == 0);
}

RHEA_FUNC(archive_zip_unchangeArchive) {
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

    return DynamicObject(zip_unchange_archive(zip) == 0);
}
