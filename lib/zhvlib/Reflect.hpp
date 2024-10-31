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

#ifndef ZHIVO_STDLIB_REFLECT_CC
#define ZHIVO_STDLIB_REFLECT_CC

#include "ZhivoLibrary.hpp"

ZHIVO_LIB_START

ZHIVO_FUNC(reflect_get);
ZHIVO_FUNC(reflect_has);
ZHIVO_FUNC(reflect_type);

ZHIVO_FUNC(reflect_declare);
ZHIVO_FUNC(reflect_delete);

ZHIVO_LIB_END

#endif
