#pragma once 

#include "ChannelMode.h"

class SSMode: public ChannelMode{
    const uint32_t phase_length = 900;
    const uint32_t phase_1_period = 120;
    const uint32_t phase_2_period = 80;
    const uint32_t phase_3_period = 60;
    const uint32_t phase_4_period = 40;
    const uint32_t cooling_time = 9000;
    const uint32_t triac_on_time = 20;

    enum class SSState{
        SS_IDLE,
        PHASE_1,
        PHASE_2,
        PHASE_3,
        PHASE_4,
        TRIAC_OFF_RELAY_ON,
        TRIAC_OFF_RELAY_OFF,
        WAIT_FOR_COOLING,
    };

    private:
        SSState m_state;
    public:
        SSMode(uint16_t call_period): ChannelMode(call_period), m_state{SSState::SS_IDLE}{}

        void setRTState(SSState state){
            m_state = state;
        }

        SSState getSSState(){
            return m_state;
        }

        void stateMachine() override;
        void stateMachineSolver() override;
};