#pragma once

#include <string>
#include <unordered_map>
#include <istream>
#include <ostream>

using MacroMap = std::unordered_map<std::string, std::string>;

class MacroProcessor
{
public:
    static void Process(std::istream& input, std::ostream& output, MacroMap&& existingMacros);
private:
};