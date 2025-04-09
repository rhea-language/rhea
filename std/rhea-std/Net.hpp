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

#ifndef RHEA_STDLIB_NET_CC
#define RHEA_STDLIB_NET_CC
 
#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif

#include "RheaLibrary.hpp"

RHEA_LIB_START

RHEA_FUNC(net_init);
RHEA_FUNC(net_deinit);

RHEA_FUNC(net_setCaCert);
RHEA_FUNC(net_getCaCert);

RHEA_FUNC(net_http_get);
RHEA_FUNC(net_http_post);
RHEA_FUNC(net_http_ping);
RHEA_FUNC(net_http_downloadFile);

RHEA_FUNC(net_tor_get);
RHEA_FUNC(net_tor_post);
RHEA_FUNC(net_tor_ping);
RHEA_FUNC(net_tor_downloadFile);
RHEA_FUNC(net_tor_isRunning);

RHEA_FUNC(net_smtp_sendMail);
RHEA_FUNC(net_smtp_sendMailHtml);

RHEA_LIB_END

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

#endif
