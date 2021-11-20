#pragma once

#include <unordered_map>
#include <functional>

template <typename State, typename T>
class StateMachine
{
public:

           StateMachine ();
    void   AddRule      (State startState, std::function<State(State, T)> rule);
    State  Push         (T input);
    void   SetFailState (State state);
    void   SetState     (State state);
    State  GetState     ();
private:

    std::unordered_map<State, std::function<State(State, T)>>    m_transitions;
    State                                                        m_currentState;
    State                                                        m_failState;
};

template<typename State, typename T>
inline StateMachine<State, T>::StateMachine()
    : m_transitions{}, m_currentState{}, m_failState{}
{
}

template<typename State, typename T>
inline void StateMachine<State, T>::AddRule(State startState, std::function<State(State, T)> rule)
{
    m_transitions[startState] = rule;
}

template<typename State, typename T>
inline State StateMachine<State, T>::Push(T input)
{
    if (m_currentState == m_failState)
    {
        return m_failState;
    }

    if (m_transitions.find(input) == m_transitions.end())
    {
        m_currentState = m_failState;
        return m_failState;
    }

    m_currentState = m_transitions[m_currentState](m_currentState, input);
}

template<typename State, typename T>
inline void StateMachine<State, T>::SetFailState(State state)
{
    m_failState = state;
}

template<typename State, typename T>
void StateMachine<State, T>::SetState (State state)
{
    m_currentState = state;
}

template<typename State, typename T>
inline State StateMachine<State, T>::GetState()
{
    return m_currentState;
}
