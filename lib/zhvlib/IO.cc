/*
 * Copyright (c) 2024 - Nathanne Isip
 * This file is part of Zhivo.
 * 
 * Zhivo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 * 
 * Zhivo is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Zhivo. If not, see <https://www.gnu.org/licenses/>.
 */

#include "zhvlib/IO.hpp"

#include <chrono>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#ifdef _WIN32
#   include <windows.h>
#else
#   include <sys/stat.h>
#   include <fcntl.h>
#endif

ZHIVO_FUNC(io_print) {
    if(args.size() == 0)
        return {};

    for(size_t i = 0; i < args.size(); i++) {
        DynamicObject arg = args.at(i);
        std::cout << arg.toString();
    }

    return {};
}

ZHIVO_FUNC(io_printLine) {
    if(args.size() == 0)
        return {};

    for(size_t i = 0; i < args.size(); i++) {
        DynamicObject arg = args.at(i);
        std::cout << arg.toString() << std::endl;
    }

    return {};
}

ZHIVO_FUNC(io_readString) {
    std::string str;
    std::getline(std::cin, str);

    return DynamicObject(str);
}

ZHIVO_FUNC(io_readNumber) {
    std::string str;
    std::getline(std::cin, str);

    return DynamicObject(::atof(str.c_str()));
}

ZHIVO_FUNC(io_readBoolean) {
    std::string str;
    std::getline(std::cin, str);

    bool boolVal = false;
    if(str == "true")
        boolVal = true;

    return DynamicObject(boolVal);
}

ZHIVO_FUNC(io_fileRead) {
    if(args.size() != 1)
        throw std::runtime_error(
            "Expecting 1 argument, got " +
                std::to_string(args.size()) +
                "."
        );

    DynamicObject fileName = args.at(0);
    std::ifstream file(fileName.toString());
    std::vector<DynamicObject> returnValues;

    if(!file) {
        returnValues.emplace_back(DynamicObject());
        returnValues.emplace_back(
            DynamicObject("Error: Could not open the file " + fileName.toString())
        );

        return DynamicObject(std::make_shared<std::vector<DynamicObject>>(returnValues));
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    returnValues.emplace_back(content);
    returnValues.emplace_back(DynamicObject());

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(returnValues));
}

ZHIVO_FUNC(io_fileWrite) {
    if(args.size() != 2)
        throw std::runtime_error(
            "Expecting 2 argument, got " +
                std::to_string(args.size()) +
                "."
        );

    DynamicObject fileName = args.at(0),
        fileContent = args.at(1);

    std::ofstream file(fileName.toString());
    if(!file)
        return DynamicObject(false);

    file << fileContent.toString();
    return DynamicObject(true);
}

ZHIVO_FUNC(io_fileSize) {
    if(args.size() != 1)
        throw std::runtime_error(
            "Expecting 1 argument, got " +
                std::to_string(args.size()) +
                "."
        );

    DynamicObject fileName = args.at(0);
    std::ifstream file(fileName.toString(), std::ios::binary | std::ios::ate);
    std::vector<DynamicObject> returnValues;

    if(!file) {
        returnValues.emplace_back(DynamicObject(0.0));
        returnValues.emplace_back(DynamicObject(false));

        return DynamicObject(std::make_shared<std::vector<DynamicObject>>(returnValues));
    }

    returnValues.emplace_back(DynamicObject(
        static_cast<double>(file.tellg())
    ));
    returnValues.emplace_back(DynamicObject(true));
    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(returnValues));
}

ZHIVO_FUNC(io_filePerms) {
    if(args.size() != 1)
        throw std::runtime_error(
            "Expecting 1 argument, got " +
                std::to_string(args.size()) +
                "."
        );

    DynamicObject fileName = args.at(0);
    std::vector<DynamicObject> returnValues;

    #ifdef _WIN32

    DWORD attributes = GetFileAttributes(fileName.toString().c_str());
    if(attributes == INVALID_FILE_ATTRIBUTES) {
        returnValues.emplace_back(DynamicObject(0.0));
        returnValues.emplace_back(DynamicObject("Could not get file attributes"));

        return DynamicObject(std::make_shared<std::vector<DynamicObject>>(returnValues));
    }

    unsigned int permissions = 0;
    if(attributes & FILE_ATTRIBUTE_READONLY)
        permissions |= 0x01;
    else permissions |= 0x02;

    #else

    struct stat fileStat;
    if(stat(fileName.toString().c_str(), &fileStat) != 0) {
        returnValues.emplace_back(DynamicObject(0.0));
        returnValues.emplace_back(DynamicObject("Could not get file status"));

        return DynamicObject(std::make_shared<std::vector<DynamicObject>>(returnValues));
    }

    unsigned int permissions = 0;
    permissions |= (fileStat.st_mode & S_IRUSR) ? 0x0100 : 0;
    permissions |= (fileStat.st_mode & S_IWUSR) ? 0x0200 : 0;
    permissions |= (fileStat.st_mode & S_IXUSR) ? 0x0400 : 0;
    permissions |= (fileStat.st_mode & S_IRGRP) ? 0x0010 : 0;
    permissions |= (fileStat.st_mode & S_IWGRP) ? 0x0020 : 0;
    permissions |= (fileStat.st_mode & S_IXGRP) ? 0x0040 : 0;
    permissions |= (fileStat.st_mode & S_IROTH) ? 0x0001 : 0;
    permissions |= (fileStat.st_mode & S_IWOTH) ? 0x0002 : 0;
    permissions |= (fileStat.st_mode & S_IXOTH) ? 0x0004 : 0;

    #endif

    returnValues.emplace_back(DynamicObject(static_cast<double>(permissions)));
    returnValues.emplace_back(DynamicObject());

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(returnValues));
}

ZHIVO_FUNC(io_fileCreationDate) {
    if(args.size() != 1)
        throw std::runtime_error(
            "Expecting 1 argument, got " +
                std::to_string(args.size()) +
                "."
        );

    DynamicObject fileName = args.at(0);
    std::vector<DynamicObject> returnValues;
    std::filesystem::path filePath(fileName.toString().c_str());

    if(!std::filesystem::exists(filePath)) {
        returnValues.emplace_back(DynamicObject(0.0));
        returnValues.emplace_back(DynamicObject("File does not exist"));
    }
    else {
        auto creationTime = std::filesystem::last_write_time(filePath);
        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            creationTime - std::filesystem::file_time_type::clock::now() + 
            std::chrono::system_clock::now()
        );
        auto ctime = std::chrono::system_clock::to_time_t(sctp);

        returnValues.emplace_back(DynamicObject(0.0));
        returnValues.emplace_back(DynamicObject(
            static_cast<double>(ctime)
        ));
    }

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(returnValues));
}

ZHIVO_FUNC(io_fileDelete) {
    if(args.size() != 1)
        throw std::runtime_error(
            "Expecting 1 argument, got " +
                std::to_string(args.size()) +
                "."
        );

    DynamicObject fileName = args.at(0);
    std::vector<DynamicObject> returnValues;

    #ifdef _WIN32

    if(DeleteFile(fileName.toString().c_str())) {
        returnValues.emplace_back(DynamicObject(true));
        returnValues.emplace_back(DynamicObject());
    }
    else {
        returnValues.emplace_back(DynamicObject(false));
        returnValues.emplace_back(DynamicObject("Could not delete file"));
    }

    #else

    struct stat buffer;
    if(stat(fileName.toString().c_str(), &buffer) == 0) {
        if(remove(fileName.toString().c_str()) == 0) {
            returnValues.emplace_back(DynamicObject(true));
            returnValues.emplace_back(DynamicObject());
        }
        else {
            returnValues.emplace_back(DynamicObject(false));
            returnValues.emplace_back(DynamicObject("Could not delete file"));
        }
    }
    else {
        returnValues.emplace_back(DynamicObject(false));
        returnValues.emplace_back(DynamicObject("File does not exist"));
    }

    #endif

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(returnValues));
}

ZHIVO_FUNC(io_folderCreate) {
    if(args.size() != 1)
        throw std::runtime_error(
            "Expecting 1 argument, got " +
                std::to_string(args.size()) +
                "."
        );

    DynamicObject folderName = args.at(0);
    std::vector<DynamicObject> returnValues;
    std::filesystem::path folderPath(folderName.toString().c_str());

    if(std::filesystem::exists(folderPath)) {
        returnValues.emplace_back(DynamicObject(false));
        returnValues.emplace_back(DynamicObject("Folder already exists"));

        return DynamicObject(std::make_shared<std::vector<DynamicObject>>(returnValues));
    }

    if(std::filesystem::create_directory(folderPath)) {
        returnValues.emplace_back(DynamicObject(true));
        returnValues.emplace_back(DynamicObject());

        return DynamicObject(std::make_shared<std::vector<DynamicObject>>(returnValues));
    }

    returnValues.emplace_back(DynamicObject(false));
    returnValues.emplace_back(DynamicObject("Could not create folder"));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(returnValues));
}

ZHIVO_FUNC(io_folderSize) {
    if(args.size() != 1)
        throw std::runtime_error(
            "Expecting 1 argument, got " +
                std::to_string(args.size()) +
                "."
        );

    DynamicObject folderName = args.at(0);
    std::vector<DynamicObject> returnValues;
    std::filesystem::path path(folderName.toString().c_str());
    unsigned long long totalSize = 0;

    if(!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
        returnValues.emplace_back(DynamicObject(0.0));
        returnValues.emplace_back(DynamicObject(
            "Provided path is not a directory or does not exist"
        ));

        return DynamicObject(std::make_shared<std::vector<DynamicObject>>(returnValues));
    }

    for(const auto& entry : std::filesystem::recursive_directory_iterator(path))
        if(std::filesystem::is_regular_file(entry))
            totalSize += std::filesystem::file_size(entry);

    returnValues.emplace_back(DynamicObject(static_cast<double>(totalSize)));
    returnValues.emplace_back(DynamicObject());

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(returnValues));
}

ZHIVO_FUNC(io_folderCreationDate) {
    if(args.size() != 1)
        throw std::runtime_error(
            "Expecting 1 argument, got " +
                std::to_string(args.size()) +
                "."
        );

    DynamicObject folderName = args.at(0);
    std::vector<DynamicObject> returnValues;
    std::filesystem::path filePath(folderName.toString().c_str());

    if(!std::filesystem::exists(filePath)) {
        returnValues.emplace_back(DynamicObject(0.0));
        returnValues.emplace_back(DynamicObject("File does not exist"));
    }
    else {
        auto creationTime = std::filesystem::last_write_time(filePath);
        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            creationTime - std::filesystem::file_time_type::clock::now() + 
            std::chrono::system_clock::now()
        );
        auto ctime = std::chrono::system_clock::to_time_t(sctp);

        returnValues.emplace_back(DynamicObject(0.0));
        returnValues.emplace_back(DynamicObject(
            static_cast<double>(ctime)
        ));
    }

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(returnValues));
}

ZHIVO_FUNC(io_folderDelete) {
    if(args.empty() || args.size() >= 2)
        throw std::runtime_error(
            "Expecting 1 or 2 argument, got " +
                std::to_string(args.size()) +
                "."
        );

    DynamicObject folderName = args.at(0);
    std::vector<DynamicObject> returnValues;
    std::filesystem::path filePath(folderName.toString().c_str());

    bool isRecursize = false;
    if(args.size() == 2) {
        DynamicObject recArg = args.at(1);
        isRecursize = recArg.booleanEquivalent();
    }

    if(!std::filesystem::exists(filePath) || !std::filesystem::is_directory(filePath))
        return DynamicObject(false);

    if(isRecursize)
        std::filesystem::remove_all(filePath);
    else std::filesystem::remove(filePath);

    return DynamicObject(true);
}

ZHIVO_FUNC(io_isFile) {
    if(args.size() != 1)
        throw std::runtime_error(
            "Expecting 1 argument, got " +
                std::to_string(args.size()) +
                "."
        );

    DynamicObject fileName = args.at(0);
    std::filesystem::path filePath(fileName.toString().c_str());

    return DynamicObject(
        std::filesystem::exists(filePath) &&
            std::filesystem::is_regular_file(filePath)
    );
}

ZHIVO_FUNC(io_isFolder) {
    if(args.size() != 1)
        throw std::runtime_error(
            "Expecting 1 argument, got " +
                std::to_string(args.size()) +
                "."
        );

    DynamicObject fileName = args.at(0);
    std::filesystem::path filePath(fileName.toString().c_str());

    return DynamicObject(
        std::filesystem::exists(filePath) &&
            std::filesystem::is_directory(filePath)
    );
}

ZHIVO_FUNC(io_listAllFiles) {
    if(args.size() != 1)
        throw std::runtime_error(
            "Expecting 1 argument, got " +
                std::to_string(args.size()) +
                "."
        );

    DynamicObject fileName = args.at(0);
    std::filesystem::path dirPath(fileName.toString().c_str());
    std::vector<DynamicObject> returnValues;

    if(!std::filesystem::exists(dirPath) || !std::filesystem::is_directory(dirPath))
        return DynamicObject();

    for(const auto& entry : std::filesystem::directory_iterator(dirPath))
        returnValues.emplace_back(DynamicObject(
            entry.path().filename().string()
        ));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(returnValues));
}

ZHIVO_FUNC(io_exit) {
    if(args.size() == 0)
        exit(0);

    DynamicObject exitCode = args.at(0);
    if(!exitCode.isNumber())
        throw std::runtime_error("Exit code is not a number.");

    exit(static_cast<int>(exitCode.getNumber()));
    return {};
}
