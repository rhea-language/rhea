#include <n8/core/RegexWrapper.hpp>

const std::string& RegexWrapper::getPattern() const {
    return this->pattern;
}

const std::regex RegexWrapper::getRegex() const {
    return *this->regex;
}
