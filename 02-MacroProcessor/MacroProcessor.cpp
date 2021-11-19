#include "MacroProcessor.h"

#include <iostream>
#include <string_view>

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

void printMacros(const std::unordered_map<std::string, std::string>& macros)
{
    for (auto[key, value] : macros)
    {
        std::cout << key << ":" << value << '\n';
    }
}

MacroProcessor::MacroProcessor(std::ostream& output)
    : m_output(output)
{
}

void MacroProcessor::AddMacro(const std::string& identifier, const std::string& body)
{
    m_macros[identifier] = body;
    //printMacros(m_macros);
}

bool MacroProcessor::Push(char ch)
{
    switch(m_state)
    {
        case ProcessorState::Begin:
            m_prevChar = ch;
            if (!std::isalpha(ch))
            {
                if (ch == '#')
                {
                    m_state = ProcessorState::ReadingMacroIdentifier;
                }
                else
                {
                    m_state = ProcessorState::PropagateNonIdentifier;
                }
            }
            else 
            {
                m_state = ProcessorState::ReadingIdentifier;
            }
            break;
        case ProcessorState::PropagateNonIdentifier:
            propagateNonIdentifier(ch);
            break;
        case ProcessorState::ReadingIdentifier:
            readIdentifier(ch);
            break;
        case ProcessorState::ReadingMacroIdentifier:
        case ProcessorState::ReadingMacroBody:
        case ProcessorState::EndingMacro:
            readMacroDefinition(ch);
            break;
        case ProcessorState::Error:
            processError();
            break;
    }

    return !(m_state == ProcessorState::Error);
}

bool MacroProcessor::operator<<(char ch)
{
    return Push(ch);
}

void MacroProcessor::Finish()
{
    m_output << m_identifierBuffer << m_prevChar;
}

void MacroProcessor::propagateNonIdentifier(char ch)
{
    m_output << m_prevChar;

    if (std::isalpha(ch))
    {
        m_state = ProcessorState::ReadingIdentifier;
    }
    else if (ch == '#' && std::isspace(m_prevChar))
    {
        m_state = ProcessorState::ReadingMacroIdentifier;
    }

    m_prevChar = ch;
}

void MacroProcessor::readIdentifier(char ch)
{
    m_identifierBuffer.append(1, m_prevChar);

    if (!std::isalnum(ch))
    {
        if (m_macros.find(m_identifierBuffer) != m_macros.end())
        {
            m_output << m_macros[m_identifierBuffer];
        }
        else 
        {
            m_output << m_identifierBuffer;
        }
        m_identifierBuffer = "";

        if (ch == '#' && std::isspace(m_prevChar))
        {
            m_state = ProcessorState::ReadingMacroIdentifier;
        }
        else
        {
            m_state = ProcessorState::PropagateNonIdentifier;
        }
    }

    m_prevChar = ch;
}

void MacroProcessor::readMacroDefinition(char ch)
{
    if (m_prevChar != '#')
    {
        if (m_state == ProcessorState::ReadingMacroIdentifier)
        {
            m_identifierBuffer.append(1, m_prevChar);
        }
        else
        {
            m_macroBodyBuffer.append(1, m_prevChar);
        }
    }

    if (std::isspace(ch) && m_state == ProcessorState::ReadingMacroIdentifier)
    {
        m_state = ProcessorState::ReadingMacroBody;
    }
    else if (ch == '#' && std::isspace(m_prevChar) && m_state == ProcessorState::ReadingMacroBody)
    {
        if (m_identifierBuffer == "")
        {
            m_state = ProcessorState::Error;
            return;
        }
        
        m_state = ProcessorState::EndingMacro;
        m_macros[m_identifierBuffer] = m_macroBodyBuffer;
        m_identifierBuffer = "";
        m_macroBodyBuffer = "";
        //printMacros(m_macros);
        unrollMacros();
    }
    else if (m_state == ProcessorState::EndingMacro)
    {
        if (m_prevChar == '#' && std::isalpha(ch))
        {
            m_state = ProcessorState::Error;
        }

        m_state = ProcessorState::Begin;
    }

    m_prevChar = ch;
}

void MacroProcessor::processError()
{
    if (!m_processedError)
    {
        if (m_identifierBuffer.length() != 0)
        {
            m_output << m_identifierBuffer;
            m_identifierBuffer = "";
        }
        m_output << "Error";
        m_processedError = true;
    }
}

void MacroProcessor::unrollMacros()
{
    for (auto&[identifier, body] : m_macros)
    {
        for (auto[unrolling_identifier, unrolling_body] : m_macros)
        {
            if (identifier == unrolling_identifier) continue;

            replace_all(body, unrolling_identifier, unrolling_body);
        }
    }
}
