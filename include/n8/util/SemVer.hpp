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

#ifndef N8_UTIL_SEMVER_HPP
#define N8_UTIL_SEMVER_HPP

#include <optional>
#include <string>

namespace N8Util {

class SemVer {
private:
    int major;
    int minor;
    int patch;
    std::optional<std::string> preRelease;
    std::optional<std::string> buildMetadata;

public:
    SemVer(
        int _major,
        int _minor,
        int _patch, 
        std::optional<std::string> _preRelease = std::nullopt, 
        std::optional<std::string> _buildMetadata = std::nullopt
    ) : major(_major),
        minor(_minor),
        patch(_patch),
        preRelease(_preRelease),
        buildMetadata(_buildMetadata) {}

    int getMajor() const;
    int getMinor() const;
    int getPatch() const;
    std::optional<std::string> getPreRelease() const;
    std::optional<std::string> getBuildMetadata() const;

    void setMajor(int value);
    void setMinor(int value);
    void setPatch(int value);
    void setPreRelease(const std::optional<std::string>& value);
    void setBuildMetadata(const std::optional<std::string>& value);

    std::string toString() const;

    static std::optional<SemVer> parse(const std::string& version);
    static bool validateSemVer(const std::string& version);
};

};

#endif
