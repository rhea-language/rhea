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

#include "rhea-std/Net.hpp"

#include <rhea/ast/TerminativeSignal.hpp>
#include <rhea/core/DynamicObject.hpp>

#include <map>
#include <vector>

#include <curl/curl.h>
#include <quoneq/http.hpp>
#include <quoneq/net.hpp>
#include <quoneq/smtp.hpp>
#include <quoneq/tor.hpp>

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

std::pair<bool, std::vector<std::string>> objectArray1DToMap(DynamicObject object) {
    if(!object.isArray())
        return std::make_pair(false, std::vector<std::string>{});

    std::vector<std::string> arrayValues;
    std::vector<DynamicObject> objects = *(object.getArray());

    for(auto& obj : objects) {
        if(!obj.isString())
            return std::make_pair(false, std::vector<std::string>{});

        arrayValues.emplace_back(obj.toString());
    }

    return std::make_pair(true, arrayValues);
}

RHEA_FUNC(net_init) {
    quoneq_net::init();
    return {};
}

RHEA_FUNC(net_deinit) {
    quoneq_net::cleanup();
    return {};
}

RHEA_FUNC(net_setCaCert) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " +
                std::to_string(args.size())
        );

    quoneq_net::set_ca_cert(args.at(0).toString());
    return {};
}

RHEA_FUNC(net_getCaCert) {
    return DynamicObject(quoneq_net::get_ca_cert());
}

RHEA_FUNC(net_http_get) {
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
    if(args.size() >= 6)
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

RHEA_FUNC(net_http_post) {
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

RHEA_FUNC(net_http_ping) {
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

RHEA_FUNC(net_http_downloadFile) {
    if(args.size() < 2 || args.size() > 8)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 to 7 argument(s), got " +
                std::to_string(args.size())
        );

    DynamicObject urlStr = args.at(0),
        outputFile = args.at(1);
    std::map<std::string, std::string> forms = {},
        headers = {},
        cookies = {},
        files = {};
    std::string proxy = "",
        username = "",
        password = "";

    if(args.size() >= 3) {
        auto result = objectArrayToMap(args.at(2));
        if(!result.first)
            throw TerminativeThrowSignal(
                std::move(address),
                "Form data map should be of array of string arrays (size=2) type."
            );

        forms = result.second;
    }
    
    if(args.size() >= 4) {
        auto result = objectArrayToMap(args.at(3));
        if(!result.first)
            throw TerminativeThrowSignal(
                std::move(address),
                "Header map should be of array of string arrays (size=2) type."
            );

        headers = result.second;
    }

    if(args.size() >= 5) {
        auto result = objectArrayToMap(args.at(4));
        if(!result.first)
            throw TerminativeThrowSignal(
                std::move(address),
                "Cookie map should be of array of string arrays (size=2) type."
            );

        cookies = result.second;
    }

    if(args.size() >= 6) {
        auto result = objectArrayToMap(args.at(5));
        if(!result.first)
            throw TerminativeThrowSignal(
                std::move(address),
                "File map should be of array of string arrays (size=2) type."
            );

        files = result.second;
    }

    if(args.size() >= 7)
        proxy = args.at(6).toString();
    if(args.size() >= 8)
        username = args.at(7).toString();
    if(args.size() >= 9)
        password = args.at(8).toString();

    return httpResponseToObject(*(quoneq_http_client::download_file(
        urlStr.toString(),
        outputFile.toString(),
        forms,
        headers,
        cookies,
        files,
        proxy,
        username,
        password
    )));
}

RHEA_FUNC(net_tor_get) {
    if(args.size() < 1 || args.size() > 5)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 to 5 argument(s), got " +
                std::to_string(args.size())
        );

    DynamicObject urlStr = args.at(0);
    std::map<std::string, std::string> headers = {},
        cookies = {};
    std::string username = "",
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
        username = args.at(3).toString();
    if(args.size() >= 5)
        password = args.at(4).toString();

    return httpResponseToObject(*(quoneq_tor_client::get(
        urlStr.toString(),
        headers,
        cookies,
        username,
        password
    )));
}

RHEA_FUNC(net_tor_post) {
    if(args.size() < 1 || args.size() > 6)
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
    std::string username = "",
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
        username = args.at(5).toString();
    if(args.size() >= 7)
        password = args.at(6).toString();

    return httpResponseToObject(*(quoneq_tor_client::post(
        urlStr.toString(),
        forms,
        headers,
        cookies,
        files,
        username,
        password
    )));
}

RHEA_FUNC(net_tor_ping) {
    if(args.size() < 1 || args.size() > 3)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 to 3 argument(s), got " +
                std::to_string(args.size())
        );

    DynamicObject urlStr = args.at(0);
    std::string username = "",
        password = "";

    if(args.size() >= 2)
        username = args.at(1).toString();
    if(args.size() >= 3)
        password = args.at(2).toString();

    return httpResponseToObject(*(quoneq_tor_client::ping(
        urlStr.toString(),
        username,
        password
    )));
}

RHEA_FUNC(net_tor_downloadFile) {
    if(args.size() < 2 || args.size() > 7)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 to 7 argument(s), got " +
                std::to_string(args.size())
        );

    DynamicObject urlStr = args.at(0),
        outputFile = args.at(1);
    std::map<std::string, std::string> forms = {},
        headers = {},
        cookies = {},
        files = {};
    std::string username = "",
        password = "";

    if(args.size() >= 3) {
        auto result = objectArrayToMap(args.at(2));
        if(!result.first)
            throw TerminativeThrowSignal(
                std::move(address),
                "Form data map should be of array of string arrays (size=2) type."
            );

        forms = result.second;
    }
    
    if(args.size() >= 4) {
        auto result = objectArrayToMap(args.at(3));
        if(!result.first)
            throw TerminativeThrowSignal(
                std::move(address),
                "Header map should be of array of string arrays (size=2) type."
            );

        headers = result.second;
    }

    if(args.size() >= 5) {
        auto result = objectArrayToMap(args.at(4));
        if(!result.first)
            throw TerminativeThrowSignal(
                std::move(address),
                "Cookie map should be of array of string arrays (size=2) type."
            );

        cookies = result.second;
    }

    if(args.size() >= 6) {
        auto result = objectArrayToMap(args.at(5));
        if(!result.first)
            throw TerminativeThrowSignal(
                std::move(address),
                "File map should be of array of string arrays (size=2) type."
            );

        files = result.second;
    }

    if(args.size() >= 7)
        username = args.at(6).toString();
    if(args.size() >= 8)
        password = args.at(7).toString();

    return httpResponseToObject(*(quoneq_tor_client::download_file(
        urlStr.toString(),
        outputFile.toString(),
        forms,
        headers,
        cookies,
        files,
        username,
        password
    )));
}

RHEA_FUNC(net_tor_isRunning) {
    if(args.size() != 0)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 0 argument, got " +
                std::to_string(args.size())
        );

    return DynamicObject(quoneq_tor_client::is_tor_running());
}

RHEA_FUNC(net_smtp_sendMail) {
    if(args.size() != 7)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 7 argument, got " +
                std::to_string(args.size())
        );

    std::string server = args.at(0).toString(),
        email = args.at(1).toString(),
        password = args.at(2).toString(),
        recipient = args.at(3).toString(),
        subject = args.at(4).toString(),
        message = args.at(5).toString();

    std::vector<std::string> files = {};
    auto result = objectArray1DToMap(args.at(1));
 
    if(!result.first)
        throw TerminativeThrowSignal(
            std::move(address),
            "File list map should be of array of string arrays (size=2) type."
        );

    files = result.second;
    return DynamicObject(quoneq_smtp_client::send_mail(
        server,
        email,
        password,
        recipient,
        subject,
        message,
        files
    ));
}

RHEA_FUNC(net_smtp_sendMailHtml) {
    if(args.size() != 7)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 7 argument, got " +
                std::to_string(args.size())
        );

    std::string server = args.at(0).toString(),
        email = args.at(1).toString(),
        password = args.at(2).toString(),
        recipient = args.at(3).toString(),
        subject = args.at(4).toString(),
        message = args.at(5).toString();
    std::vector<std::string> files = {};

    auto result = objectArray1DToMap(args.at(6)); 
    if(!result.first)
        throw TerminativeThrowSignal(
            std::move(address),
            "File list map should be of array of string arrays (size=2) type."
        );

    files = result.second;
    return DynamicObject(quoneq_smtp_client::send_mail_html(
        server,
        email,
        password,
        recipient,
        subject,
        message,
        files
    ));
}
