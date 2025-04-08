/*
 * Copyright (c) 2025 - Nathanne Isip
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

#include "rhea-std/Lang.hpp"

#include <Rhea.hpp>
#include <rhea/ast/TerminativeSignal.hpp>

RHEA_FUNC(lang_buildPlatform) {
    return DynamicObject(
        std::string(RHEA_BUILD_PLATFORM)
    );
}

RHEA_FUNC(lang_buildType) {
    return DynamicObject(
        std::string(RHEA_BUILD_TYPE)
    );
}

RHEA_FUNC(lang_buildTime) {
    return DynamicObject(
        std::string(RHEA_BUILD_TIME)
    );
}

RHEA_FUNC(lang_version) {
    return DynamicObject(
        std::string(RHEA_VERSION)
    );
}
