#pragma once

#include <stdint.h>

class ChannelMode{
    public:
        enum class channelStatus{
            DO_OFF = 0,        
            DO_COOLING,         
            DO_SOFT_START,  
            DO_TRANSITION, 
            DO_ON,
        };

        enum class outputSignal{
            SIGNAL_OFF,
            SIGNAL_ON,
        };

        enum class outputState{
            STATE_OFF,
            STATE_ON,
        };

    protected:
        uint32_t        m_time;
        uint16_t        m_call_period;
        outputState     m_real_output_state; // on/off
        outputState     m_wanted_output_state; // on/off
        outputSignal    m_triac_signal; // on/off
        outputSignal    m_relay_signal; // on/off
        channelStatus   m_channel_status;


    public:
        ChannelMode(uint16_t call_period): 
            m_time{0}, 
            m_call_period{call_period}, 
            m_real_output_state{outputState::STATE_OFF}, 
            m_wanted_output_state{outputState::STATE_OFF} {}

        void setOutputState(outputState output_state){
            m_wanted_output_state = output_state;
        }

        outputState getOutputState(){
            return m_real_output_state;
        }

        outputSignal getTriacSignal(){
            return m_triac_signal;
        }

        outputSignal getRelaySignal(){
            return m_relay_signal;
        }

        channelStatus getChannelStatus(){
            return m_channel_status;
        }

        virtual void stateMachine(){}
        virtual void stateMachineSolver(){}

};