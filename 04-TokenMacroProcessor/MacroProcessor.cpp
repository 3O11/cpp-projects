#include "MacroProcessor.h"

enum class State
{
    Pass,
    ReadMacroBody,
    Error,
};

std::size_t replace_all(std::string& inout, std::string_view what, std::string_view with)
{
    std::size_t count{};
    for (std::string::size_type pos{};
         inout.npos != (pos = inout.find(what.data(), pos, what.length()));
         pos += with.length(), ++count) {
        inout.replace(pos, what.length(), with.data(), with.length());
    }
    return count;
}

void unrollMacros(MacroMap& macros)
{
    for (auto&[identifier, body] : macros)
    {
        for (const auto&[unrolling_identifier, unrolling_body] : macros)
        {
            if (identifier == unrolling_identifier) continue;

            replace_all(body, unrolling_identifier, unrolling_body);
        }
    }
}

bool canBeIdentifier(const std::string& token)
{
    return token.length() >= 2 && token[0] == '#' && std::isalpha(token[1]);
}

void MacroProcessor::Process(std::istream& input, std::ostream& output, MacroMap&& existingMacros)
{
    MacroMap macros = existingMacros;
    State state = State::Pass;
    std::string macroIdentifier;
    std::string macroBody;

    std::string token;
    while (input >> token)
    {
        if (state == State::Pass)
        {
            if (canBeIdentifier(token))
            {
                macroIdentifier = token.substr(1, token.length() - 1);
                state = State::ReadMacroBody;
            }
            else if (token == "#")
            {
                output << " Error\n";
                state = State::Error;
                break;
            }
            else
            {
                if (macros.find(token) != macros.end())
                {
                    output << macros[token];
                }
                else
                {
                    output << " " << token;
                }
            }
        }
        else if (state == State::ReadMacroBody)
        {
            if (canBeIdentifier(token))
            {
                output << " Error\n";
                state = State::Error;
                break;
            }
            else if (token[0] == '#')
            {
                if (token.length() > 1)
                {
                    output << token.substr(1, token.length() - 1);
                }

                macros[macroIdentifier] = macroBody;
                unrollMacros(macros);
                macroBody = "";
                macroIdentifier = "";
                state = State::Pass;
            }
            else
            {
                macroBody.append(" " + token);
            }
        }
    }
}