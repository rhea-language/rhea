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

#ifndef RHEA_UTIL_PATH_HELPER_HPP
#define RHEA_UTIL_PATH_HELPER_HPP

#include <rhea/util/SemVer.hpp>

#include <string>
#include <vector>

namespace RheaUtil {

class PathHelper {
public:
    static std::string installationPath();
    static std::string findSharedLibrary(std::string name);

    static bool isLibraryInstalled(
        std::string libraryName,
        std::string libraryVersion
    );

    static std::vector<std::string> getLibraryFiles(
        std::string libraryName,
        std::string libraryVersion
    );
};

}

#endif
