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

#include "n8std/Net.hpp"

#include <n8/ast/TerminativeSignal.hpp>
#include <n8/core/DynamicObject.hpp>

#include <map>
#include <vector>

#include <curl/curl.h>
#include <quoneq/http.hpp>
#include <quoneq/net.hpp>

DynamicObject httpResponseToObject(quoneq_http_response response) {
    std::vector<DynamicObject> object;
    object.emplace_back(DynamicObject(
        static_cast<double>(response.status)
    ));

    object.emplace_back(DynamicObject(response.statusType));
    object.emplace_back(DynamicObject(response.errorMessage));
    object.emplace_back(DynamicObject(response.content));

    std::vector<DynamicObject> headers;
    for(const auto& header : response.header) {
        std::vector<DynamicObject> headerEntry;
        headerEntry.emplace_back(DynamicObject(header.first));
        headerEntry.emplace_back(DynamicObject(header.second));

        headers.emplace_back(DynamicObject(
            std::make_shared<std::vector<DynamicObject>>(
                headerEntry
            )
        ));
    }

    object.emplace_back(DynamicObject(
        std::make_shared<std::vector<DynamicObject>>(
            headers
        )
    ));

    std::vector<DynamicObject> cookies;
    for(const auto& cookie : response.cookies) {
        std::vector<DynamicObject> cookieEntry;
        cookieEntry.emplace_back(DynamicObject(cookie.first));
        cookieEntry.emplace_back(DynamicObject(cookie.second));

        cookies.emplace_back(DynamicObject(
            std::make_shared<std::vector<DynamicObject>>(
                cookieEntry
            )
        ));
    }

    object.emplace_back(DynamicObject(
        std::make_shared<std::vector<DynamicObject>>(
            cookies
        )
    ));

    return DynamicObject(std::make_shared<std::vector<DynamicObject>>(
        object
    ));
}

std::pair<bool, std::map<std::string, std::string>> objectArrayToMap(DynamicObject object) {
    if(!object.isArray())
        return std::make_pair(false, std::map<std::string, std::string>{});

    std::map<std::string, std::string> pairs;
    std::vector<DynamicObject> objects = *(object.getArray());

    for(const auto& obj : objects) {
        if(!obj.isArray())
            return std::make_pair(false, std::map<std::string, std::string>{});

        std::vector<DynamicObject> items = *(obj.getArray());
        if(items.size() != 2)
            return std::make_pair(false, std::map<std::string, std::string>{});

        pairs[items.at(0).toString()] = items.at(1).toString();
    }

    return std::make_pair(true, pairs);
}

N8_FUNC(net_init) {
    quoneq_net::init();
    return {};
}

N8_FUNC(net_deinit) {
    quoneq_net::cleanup();
    return {};
}

N8_FUNC(net_setCaCert) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    quoneq_net::set_ca_cert(args.at(0).toString());
    return {};
}

N8_FUNC(net_getCaCert) {
    return DynamicObject(quoneq_net::get_ca_cert());
}

N8_FUNC(net_http_get) {
    if(args.size() < 1 || args.size() > 6)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 to 6 argument(s), got " +
                std::to_string(args.size())
        );

    DynamicObject urlStr = args.at(0);
    std::map<std::string, std::string> headers = {},
        cookies = {};
    std::string proxy = "",
        username = "",
        password = "";

    if(args.size() >= 2) {
        auto result = objectArrayToMap(args.at(1));
        if(!result.first)
            throw TerminativeThrowSignal(
                std::move(address),
                "Header map should be of array of string arrays (size=2) type."
            );

        headers = result.second;
    }

    if(args.size() >= 3) {
        auto result = objectArrayToMap(args.at(2));
        if(!result.first)
            throw TerminativeThrowSignal(
                std::move(address),
                "Cookie map should be of array of string arrays (size=2) type."
            );

        cookies = result.second;
    }

    if(args.size() >= 4)
        proxy = args.at(3).toString();
    if(args.size() >= 5)
        username = args.at(4).toString();
    if(args.size() >= 5)
        password = args.at(5).toString();

    return httpResponseToObject(*(quoneq_http_client::get(
        urlStr.toString(),
        headers,
        cookies,
        proxy,
        username,
        password
    )));
}

N8_FUNC(net_http_post) {
    if(args.size() < 1 || args.size() > 7)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 to 6 argument(s), got " +
                std::to_string(args.size())
        );

    DynamicObject urlStr = args.at(0);
    std::map<std::string, std::string> forms = {},
        headers = {},
        cookies = {},
        files = {};
    std::string proxy = "",
        username = "",
        password = "";

    if(args.size() >= 2) {
        auto result = objectArrayToMap(args.at(1));
        if(!result.first)
            throw TerminativeThrowSignal(
                std::move(address),
                "Form data map should be of array of string arrays (size=2) type."
            );

        forms = result.second;
    }
    
    if(args.size() >= 3) {
        auto result = objectArrayToMap(args.at(2));
        if(!result.first)
            throw TerminativeThrowSignal(
                std::move(address),
                "Header map should be of array of string arrays (size=2) type."
            );

        headers = result.second;
    }

    if(args.size() >= 4) {
        auto result = objectArrayToMap(args.at(3));
        if(!result.first)
            throw TerminativeThrowSignal(
                std::move(address),
                "Cookie map should be of array of string arrays (size=2) type."
            );

        cookies = result.second;
    }

    if(args.size() >= 5) {
        auto result = objectArrayToMap(args.at(4));
        if(!result.first)
            throw TerminativeThrowSignal(
                std::move(address),
                "File map should be of array of string arrays (size=2) type."
            );

        files = result.second;
    }

    if(args.size() >= 6)
        proxy = args.at(5).toString();
    if(args.size() >= 7)
        username = args.at(6).toString();
    if(args.size() >= 8)
        password = args.at(7).toString();

    return httpResponseToObject(*(quoneq_http_client::post(
        urlStr.toString(),
        forms,
        headers,
        cookies,
        files,
        proxy,
        username,
        password
    )));
}

N8_FUNC(net_http_ping) {
    if(args.size() < 1 || args.size() > 4)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 to 4 argument(s), got " +
                std::to_string(args.size())
        );

    DynamicObject urlStr = args.at(0);
    std::string proxy = "",
        username = "",
        password = "";

    if(args.size() >= 2)
        proxy = args.at(1).toString();
    if(args.size() >= 3)
        username = args.at(2).toString();
    if(args.size() >= 4)
        password = args.at(3).toString();

    return httpResponseToObject(*(quoneq_http_client::ping(
        urlStr.toString(),
        proxy,
        username,
        password
    )));
}

N8_FUNC(net_http_downloadFile) {
    return {};
}
