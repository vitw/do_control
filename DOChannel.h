#pragma once 

#include <stdint.h>
#include "ChannelMode.h"


class DOChannel {
    private:
        uint8_t m_channel_num{};
        ChannelMode* m_current_channel_mode{};
        // ChannelMode* m_RT_channel_mode{};
        // ChannelMode* m_SS_channel_mode{};
        // ChannelMode* m_CL_channel_mode{};
         

    public:
        DOChannel(uint8_t channel_num, ChannelMode* channel_mode): 
            m_channel_num{channel_num},
            m_current_channel_mode{channel_mode}{}

        void setChannelNum(uint8_t channel_num){
            m_channel_num = channel_num;
        }

        uint8_t getChannelNum(){
            return m_channel_num;
        }

        void setChannelOutputState(ChannelMode::outputState output_state){
            m_current_channel_mode->setOutputState(output_state);
        }

        ChannelMode::outputState getChannelOutputState(){
            return m_current_channel_mode->getOutputState();
        }

        void setChannelMode(ChannelMode* channel_mode){
            m_current_channel_mode = channel_mode;
        }

        ChannelMode::outputSignal getTriacSignal(){
            return m_current_channel_mode->getTriacSignal();
        }

        ChannelMode::outputSignal getRelaySignal(){
            return m_current_channel_mode->getRelaySignal();
        }

        ChannelMode::channelStatus getChannelStatus(){
            return m_current_channel_mode->getChannelStatus();
        }

        void tickHandler(){
            m_current_channel_mode->stateMachine();
            m_current_channel_mode->stateMachineSolver();
        }
};