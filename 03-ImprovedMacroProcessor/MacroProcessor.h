#pragma once

#include <memory>
#include <istream>
#include <ostream>

#include "StateMachine.h"

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

// The point of these pointers is to enable multiple inputs/outputs
// during the class lifetime. This class is just a viewer, it is
// **NOT** responsible for the lifetimes in any way.

class MacroProcessor
{
public:

         MacroProcessor  ();
         MacroProcessor  (std::istream* input, std::ostream* output);
    void SetInput        (std::istream* input);
    void SetOutput       (std::ostream* output);
    void Process         ();
    void AddMacro        (const std::string& identifier, const std::string& body);
private:
    void step            (char ch);

    StateMachine<ProcessorState, char>  m_stateMachine;
    std::istream*       m_input;
    std::ostream*       m_output;
};