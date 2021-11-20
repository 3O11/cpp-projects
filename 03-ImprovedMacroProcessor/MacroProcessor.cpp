#include "MacroProcessor.h"

MacroProcessor::MacroProcessor ()
    : m_stateMachine(), m_input(nullptr), m_output(nullptr)
{
    // StateMachine setup
    m_stateMachine.SetState(ProcessorState::Begin);
    m_stateMachine.SetFailState(ProcessorState::Error);

    m_stateMachine.AddRule(
        ProcessorState::Begin,
        [](ProcessorState state, char ch)
        {
            return state;
        }
    );

    m_stateMachine.AddRule(
        ProcessorState::PropagateNonIdentifier,
        [](ProcessorState state, char ch)
        {
            return state;
        }
    );

    m_stateMachine.AddRule(
        ProcessorState::ReadingIdentifier,
        [](ProcessorState state, char ch)
        {
            return state;
        }
    );

    m_stateMachine.AddRule(
        ProcessorState::ReadingMacroIdentifier,
        [](ProcessorState state, char ch)
        {
            return state;
        }
    );

    m_stateMachine.AddRule(
        ProcessorState::EndingMacro,
        [](ProcessorState state, char ch)
        {
            return state;
        }
    );
}

MacroProcessor::MacroProcessor (std::istream* input, std::ostream* output)
{

}
void MacroProcessor::SetInput (std::istream* input)
{

}

void MacroProcessor::SetOutput (std::ostream* output)
{

}

void MacroProcessor::Process ()
{

}

void MacroProcessor::AddMacro (const std::string& identifier, const std::string& body)
{

}

void MacroProcessor::step (char ch)
{

}