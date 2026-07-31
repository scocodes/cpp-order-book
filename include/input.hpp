#pragma once
#include <iostream>
#include "orderBook.hpp"

namespace Input
{
    std::string Input::getLine();
    bool Input::inputFormat(const std::string& input);
    ParsedInput Input::orderCreate(const std::string& line);
};