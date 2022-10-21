#include <iostream>
#include "SSMode.h"


void SSMode::stateMachine(){
	// ChannelStatus current_status = DO_OFF; 
 	switch (m_state){
		case SSState::SS_IDLE:
            // softStartSolver(channel, signals);
			// current_status = channel->output_state? DO_ON: DO_OFF;
			// if (channel->channel_context.state.ss_state == PHASE_1){
            //     m_triac_signal = 1;
            //     m_relay_signal = 0;
			// 	current_status = DO_SOFT_START;
			// }
			m_triac_signal = outputSignal::SIGNAL_OFF;
			if (m_real_output_state == outputState::STATE_ON){
				m_relay_signal = outputSignal::SIGNAL_ON;
				m_channel_status = channelStatus::DO_ON;
			}else{
				m_relay_signal = outputSignal::SIGNAL_OFF;
				m_channel_status = channelStatus::DO_OFF;
			}
			break;
		case SSState::PHASE_1:
			m_channel_status = channelStatus::DO_SOFT_START;
			if ((m_time % phase_1_period) < triac_on_time){
                m_triac_signal = outputSignal::SIGNAL_ON;
                m_relay_signal = outputSignal::SIGNAL_OFF;
			} else{
                m_triac_signal = outputSignal::SIGNAL_OFF;
                m_relay_signal = outputSignal::SIGNAL_OFF;
			}
			break;
		case SSState::PHASE_2:
			m_channel_status = channelStatus::DO_SOFT_START;
			if ((m_time % phase_2_period) < triac_on_time){
                m_triac_signal = outputSignal::SIGNAL_ON;
                m_relay_signal = outputSignal::SIGNAL_OFF;
			} else{
                m_triac_signal = outputSignal::SIGNAL_OFF;
                m_relay_signal = outputSignal::SIGNAL_OFF;
			}
			break;
		case SSState::PHASE_3:
			m_channel_status = channelStatus::DO_SOFT_START;
			if ((m_time % phase_3_period) < triac_on_time){
                m_triac_signal = outputSignal::SIGNAL_ON;
                m_relay_signal = outputSignal::SIGNAL_OFF;
			} else{
                m_triac_signal = outputSignal::SIGNAL_OFF;
                m_relay_signal = outputSignal::SIGNAL_OFF;
			}
			break;
		case SSState::PHASE_4:
			m_channel_status = channelStatus::DO_SOFT_START;
			if ((m_time % phase_4_period) < triac_on_time){
                m_triac_signal = outputSignal::SIGNAL_ON;
                m_relay_signal = outputSignal::SIGNAL_OFF;
			} else{
                m_triac_signal = outputSignal::SIGNAL_OFF;
                m_relay_signal = outputSignal::SIGNAL_OFF;
			}
			break;
		case SSState::TRIAC_OFF_RELAY_ON:
			m_channel_status = channelStatus::DO_ON;
			m_triac_signal = outputSignal::SIGNAL_OFF;
			m_relay_signal = outputSignal::SIGNAL_ON;
			break;
		case SSState::TRIAC_OFF_RELAY_OFF:
			m_channel_status = channelStatus::DO_COOLING;
			m_triac_signal = outputSignal::SIGNAL_OFF;
			m_relay_signal = outputSignal::SIGNAL_OFF;
			break;
		case SSState::WAIT_FOR_COOLING:
			m_channel_status = (m_real_output_state == outputState::STATE_ON)? channelStatus::DO_ON: channelStatus::DO_COOLING;
			break;
	}
	m_time += m_call_period;
}

void SSMode::stateMachineSolver(){
	// ChannelStatus current_status = DO_OFF; 
	switch (m_state){
		case SSState::SS_IDLE:
		{
			if (m_wanted_output_state != m_real_output_state) {
				// turning on?
				if (m_wanted_output_state == outputState::STATE_ON){
					// change output mode if needed
					// if ( channel->user_channel_mode != channel->channel_mode){
					// 	channel->channel_mode = channel->user_channel_mode; 
                    //     // switchToRTSM();
					// }else{
						m_state = SSState::PHASE_1;
						m_time = 0;
					// }
				}else{
					// have to turn off the line with relay triac state machine
					// switchToRTSM();
					m_state = SSState::TRIAC_OFF_RELAY_OFF;
				}
			}
		}break;
		case SSState::PHASE_1:
			if (m_time >= phase_length){
				m_state = SSState::PHASE_2;
				m_time = 0;
			}
			if (m_wanted_output_state == outputState::STATE_OFF && m_triac_signal != ChannelMode::outputSignal::SIGNAL_ON){
				m_state = SSState::TRIAC_OFF_RELAY_OFF;
				m_time = 0;
			}
			break;
		case SSState::PHASE_2:
			if (m_time >= phase_length){
				m_state = SSState::PHASE_3;
				m_time = 0;
			}
			if (m_wanted_output_state == outputState::STATE_OFF && m_triac_signal != ChannelMode::outputSignal::SIGNAL_ON){
				m_state = SSState::TRIAC_OFF_RELAY_OFF;
				m_time = 0;
			}
			break;
		case SSState::PHASE_3:
			if (m_time >= phase_length){
				m_state = SSState::PHASE_4;
				m_time = 0;
			}
			if (m_wanted_output_state == outputState::STATE_OFF && m_triac_signal != ChannelMode::outputSignal::SIGNAL_ON){
				m_state = SSState::TRIAC_OFF_RELAY_OFF;
				m_time = 0;
			}
			break;
		case SSState::PHASE_4:
			if (m_time >= phase_length){
				m_state = SSState::TRIAC_OFF_RELAY_ON;
				m_time = 0;
			}
			if (m_wanted_output_state == outputState::STATE_OFF && m_triac_signal != ChannelMode::outputSignal::SIGNAL_ON){
				m_state = SSState::TRIAC_OFF_RELAY_OFF;
				m_time = 0;
			}
			break;
		case SSState::TRIAC_OFF_RELAY_ON:
            m_state = SSState::WAIT_FOR_COOLING;
            // switchToRTForOneRun(channel, channel->user_output_state);
			m_real_output_state = m_wanted_output_state;
			break;
		case SSState::TRIAC_OFF_RELAY_OFF:
            m_state = SSState::WAIT_FOR_COOLING;
            // switchToRTForOneRun(channel, channel->user_output_state);
			m_real_output_state = m_wanted_output_state;
			break;
		case SSState::WAIT_FOR_COOLING:
			if ( m_time >= cooling_time){
				m_state = SSState::SS_IDLE;
				m_time = 0;
			}
			else if ((m_wanted_output_state != m_real_output_state) && 
					(m_wanted_output_state == outputState::STATE_OFF)){
				// switchToRTSM();
				m_state = SSState::TRIAC_OFF_RELAY_OFF;
			}
			else if ((m_wanted_output_state != m_real_output_state) && 
					(m_wanted_output_state == outputState::STATE_ON)){
				// change output mode if needed
				// if ( channel->user_channel_mode != channel->channel_mode){
				// 	channel->channel_mode = channel->user_channel_mode;
                //     // switchToRTSM();
				// }else{
				// 	channel->output_state = OFF;
				// }
			}
			else {
				m_real_output_state = m_wanted_output_state;
			}
			break;
		default:{
			;
		}
	}
}