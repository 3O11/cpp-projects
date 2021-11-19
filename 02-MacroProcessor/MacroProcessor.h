#pragma once

#include <string>
#include <unordered_map>
#include <ostream>

// The way this class behaves is this:
// 1) the previous character is processed
// 2) state transition is decided

enum class ProcessorState
{
    Begin,
    PropagateNonIdentifier,
    ReadingIdentifier,
    ReadingMacroIdentifier,
    ReadingMacroBody,
    EndingMacro,
    Error,
};

class MacroProcessor
{
public:
    MacroProcessor(std::ostream& output);
    void AddMacro(const std::string& identifier, const std::string& body);

    // Returns false if there was an error with processing the character.
    bool Push(char ch);
    bool operator<<(char ch);
    void Finish();

private:

    void propagateNonIdentifier(char ch);
    void readIdentifier(char ch);
    void readMacroDefinition(char ch);
    void processError();
    void unrollMacros();

    std::ostream&                                 m_output;
    std::string                                   m_identifierBuffer = "";
    std::string                                   m_macroBodyBuffer  = "";
    char                                          m_prevChar         = ' ';
    bool                                          m_processedError   = false;
    ProcessorState                                m_state            = ProcessorState::Begin;
    std::unordered_map<std::string, std::string>  m_macros;
};