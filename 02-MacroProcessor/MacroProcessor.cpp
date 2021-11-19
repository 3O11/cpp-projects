#include "MacroProcessor.h"

#include <iostream>

void MacroProcessor::AddMacro(const std::string& identifier, const std::string& body)
{
    m_macros[identifier] = body;
}

bool MacroProcessor::Push(char ch)
{
    std::cerr << "Current char: " << ch << std::endl;

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
    std::cout << m_identifierBuffer << m_prevChar;
}

void MacroProcessor::propagateNonIdentifier(char ch)
{
    std::cerr << "Reading generic-chars" << std::endl;

    std::cout << m_prevChar;
    m_prevChar = ch;

    if (std::isalpha(ch))
    {
        m_state = ProcessorState::ReadingIdentifier;
    }
    else if (ch == '#')
    {
        m_state = ProcessorState::ReadingMacroIdentifier;
    }
}

void MacroProcessor::readIdentifier(char ch)
{
    std::cerr << "Reading identifier" << std::endl;

    m_identifierBuffer.append(m_prevChar, 1);
    m_prevChar = ch;

    if (!std::isalnum(ch))
    {
        if (m_macros.find(m_identifierBuffer) != m_macros.end())
        {
            std::cout << m_macros[m_identifierBuffer];
        }
        else 
        {
            std::cout << m_identifierBuffer;
        }

        if (ch == '#')
        {
            m_state = ProcessorState::ReadingMacroIdentifier;
        }
        else
        {
            m_state = ProcessorState::PropagateNonIdentifier;
        }
    }
}

void MacroProcessor::readMacroDefinition(char ch)
{
    std::cerr << "Reading macro definition" << std::endl;

    if (ch != '#')
    {
        if (m_state == ProcessorState::ReadingMacroIdentifier)
        {
            m_identifierBuffer.append(m_prevChar, 1);
        }
        else
        {
            m_macroBodyBuffer.append(m_prevChar, 1);
        }
    }
    m_prevChar = ch;

    if (std::isspace(ch) && m_state == ProcessorState::ReadingMacroIdentifier)
    {
        m_state = ProcessorState::ReadingMacroBody;
    }
    else if (ch == '#' && m_state == ProcessorState::ReadingMacroBody)
    {
        m_state = ProcessorState::Begin;
        m_macros[m_identifierBuffer] = m_macroBodyBuffer;
        m_identifierBuffer = "";
        m_macroBodyBuffer = "";
    }
}

void MacroProcessor::processError()
{
    std::cerr << "Error" << std::endl;

    if (!m_processedError)
    {
        if (m_identifierBuffer.length() != 0)
        {
            std::cout << m_identifierBuffer;
            m_identifierBuffer = "";
        }
        std::cout << "Error";
        m_processedError = true;
    }
}
