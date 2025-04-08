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

#include <rhea/util/Render.hpp>
#include <rhea/core/Runtime.hpp>

#include <iostream>

namespace RheaUtil {

void render(std::string text) {
    std::fprintf(stdout, "%s", text.c_str());
    std::fflush(stdout);
}

void renderError(std::string text) {
    std::fprintf(stderr, "%s", text.c_str());
    std::fflush(stderr);
}

}
