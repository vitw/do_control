#pragma once 

#include "ChannelMode.h"

class RTMode: public ChannelMode{
    const uint32_t triac_on_time_b4_relay_toggle = 40;
    const uint32_t triac_on_time_b4_triac_off = 80;

    enum class RTState {
        RT_IDLE,
        TRIAC_ON,
        RELAY_ON,
        RELAY_OFF,
        TRIAC_OFF,
    };

    private:
        RTState m_state;
    public:
        RTMode(uint16_t call_period): ChannelMode(call_period), m_state{RTState::RT_IDLE}{}

        // void setRTState(RTState state){
        //     m_state = state;
        // }

        RTState getRTState(){
            return m_state;
        }

        void stateMachine() override;
        void stateMachineSolver() override;
};