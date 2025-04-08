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

#include <rhea/util/SemVer.hpp>

#include <regex>
#include <sstream>

namespace RheaUtil {

int SemVer::getMajor() const {
    return this->major;
}

int SemVer::getMinor() const {
    return this->minor;
}

int SemVer::getPatch() const {
    return this->patch;
}

std::optional<std::string> SemVer::getPreRelease() const {
    return this->preRelease;
}

std::optional<std::string> SemVer::getBuildMetadata() const {
    return this->buildMetadata;
}

void SemVer::setMajor(int value) {
    this->major = value;
}

void SemVer::setMinor(int value) {
    this->minor = value;
}

void SemVer::setPatch(int value) {
    this->patch = value;
}

void SemVer::setPreRelease(const std::optional<std::string>& value) {
    this->preRelease = value;
}

void SemVer::setBuildMetadata(const std::optional<std::string>& value) {
    this->buildMetadata = value;
}

std::string SemVer::toString() const {
    std::ostringstream oss;
    oss << this->major << "."
        << this->minor << "."
        << this->patch;

    if(this->preRelease.has_value())
        oss << "-" << this->preRelease.value();
    if(this->buildMetadata.has_value())
        oss << "+" << this->buildMetadata.value();

    return oss.str();
}

std::optional<SemVer> SemVer::parse(const std::string& version) {
    if(!SemVer::validateSemVer(version))
        return std::nullopt;

    std::regex semverRegex(
        R"(^(\d+)\.(\d+)\.(\d+)(?:-([0-9A-Za-z\-\.]+))?(?:\+([0-9A-Za-z\-\.]+))?$)"
    );
    std::smatch match;

    if(std::regex_match(version, match, semverRegex)) {
        int major = std::stoi(match[1]);
        int minor = std::stoi(match[2]);
        int patch = std::stoi(match[3]);

        std::optional<std::string> preRelease =
            match[4].matched ?
                std::optional<std::string>(match[4]) :
                std::nullopt;
        std::optional<std::string> buildMetadata =
            match[5].matched ?
                std::optional<std::string>(match[5]) :
                std::nullopt;

        return SemVer(major, minor, patch, preRelease, buildMetadata);
    }

    return std::nullopt;
}

bool SemVer::validateSemVer(const std::string& version) {
    return std::regex_match(version, std::regex(
        R"(^(\d+)\.(\d+)\.(\d+)(?:-([0-9A-Za-z\-\.]+))?(?:\+([0-9A-Za-z\-\.]+))?$)"
    ));
}

}
