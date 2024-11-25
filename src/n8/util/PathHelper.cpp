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

#include <N8.hpp>
#include <n8/util/PathHelper.hpp>

#include <cstdlib>
#include <filesystem>

namespace N8Util {

std::string PathHelper::installationPath() {
    return std::getenv(N8_ENV_PATH_NAME);
}

std::string PathHelper::findSharedLibrary(std::string name) {
    if(std::filesystem::exists(name))
        return name;

    namespace fs = std::filesystem;
    std::string directoryPath = PathHelper::installationPath() +
        FS_FILE_SEPARATOR + "modules";

    try {
        if(fs::exists(directoryPath) &&
            fs::is_directory(directoryPath))
            for(const auto& entry : fs::directory_iterator(directoryPath)) {
                std::string libPath = entry.path().string() +
                    FS_FILE_SEPARATOR + "lib" +
                    FS_FILE_SEPARATOR + name;

                if(fs::exists(libPath))
                    return libPath;
            }
    }
    catch(const std::exception& ex) {}

    return "";
}

std::vector<std::string> PathHelper::getLibraryFiles(std::string libraryName) {
    namespace fs = std::filesystem;
    std::vector<std::string> n8Files;

    std::string directoryPath = PathHelper::installationPath() +
        FS_FILE_SEPARATOR + "modules" + FS_FILE_SEPARATOR +
        libraryName + "src";

    try {
        if(fs::exists(directoryPath) &&
            fs::is_directory(directoryPath))
            for(const auto& entry : fs::directory_iterator(directoryPath))
                if(entry.is_regular_file() && entry.path().extension() == ".n8")
                    n8Files.push_back(entry.path().string());
    }
    catch(const std::exception& ex) {}

    return n8Files;
}

}