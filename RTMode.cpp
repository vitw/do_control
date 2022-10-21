#include "RTMode.h"

void RTMode::stateMachine(){
	switch (m_state){
		case RTState::RT_IDLE:
			m_triac_signal = outputSignal::SIGNAL_OFF;
			if (m_real_output_state == outputState::STATE_ON){
				m_relay_signal = outputSignal::SIGNAL_ON;
				m_channel_status = channelStatus::DO_ON;
			}else{
				m_relay_signal = outputSignal::SIGNAL_OFF;
				m_channel_status = channelStatus::DO_OFF;
			}
			break;
		case RTState::TRIAC_ON:
			m_triac_signal = outputSignal::SIGNAL_ON;
			if (m_real_output_state == outputState::STATE_ON)
				m_relay_signal = outputSignal::SIGNAL_ON;
			else
				m_relay_signal = outputSignal::SIGNAL_OFF;

			m_channel_status = channelStatus::DO_TRANSITION;
			break;
		case RTState::RELAY_ON:
			m_triac_signal = outputSignal::SIGNAL_ON;
			m_relay_signal = outputSignal::SIGNAL_ON;
			m_channel_status = channelStatus::DO_TRANSITION;
			break;
		case RTState::RELAY_OFF:
			m_triac_signal = outputSignal::SIGNAL_ON;
			m_relay_signal = outputSignal::SIGNAL_OFF;
			m_channel_status = channelStatus::DO_TRANSITION;
			break;
		case RTState::TRIAC_OFF:
			m_triac_signal = outputSignal::SIGNAL_OFF;

			if (m_wanted_output_state == outputState::STATE_ON){
				m_relay_signal = outputSignal::SIGNAL_ON;
				m_channel_status = channelStatus::DO_ON;
			}else{
				m_relay_signal = outputSignal::SIGNAL_OFF;
				m_channel_status = channelStatus::DO_OFF;
			}

			break;
	}
	// // call callback function to set Relay and Triac signals
	// set_signals(channel->channel_num, signals);
    m_time += m_call_period;
}

void RTMode::stateMachineSolver(){
	switch (m_state){
		case RTState::RT_IDLE: 
		{
			if (m_wanted_output_state != m_real_output_state) {
				// turning on?
				// change output mode if needed
                
				// if (wanted_output_state && channel->user_channel_mode != channel->channel_mode){
				// 	// switch state machine
				// 	channel->channel_mode = channel->user_channel_mode;
				// }else{
                //     channel->channel_context.state.rt_state = TRIAC_ON;
                //     channel->channel_context.time = 0;
				// }
                m_state = RTState::TRIAC_ON;
                m_time = 0;
			}
		}break;
		case RTState::TRIAC_ON:
			if (m_time >= triac_on_time_b4_relay_toggle){
                (m_wanted_output_state == outputState::STATE_ON) ? m_state = RTState::RELAY_ON: m_state = RTState::RELAY_OFF;
			}
			break;
		case RTState::RELAY_ON:
			if (m_time >= triac_on_time_b4_triac_off){
				m_state = RTState::TRIAC_OFF;
			}
			break;
		case RTState::RELAY_OFF:
			if (m_time >= triac_on_time_b4_triac_off){
				m_state = RTState::TRIAC_OFF;
			}
			break;
		case RTState::TRIAC_OFF:
            m_real_output_state = m_wanted_output_state;
			m_time = 0;
			// switch back to Soft Start only if Relay Triac was called from Soft Start
			// switchBackToSS(channel);
			m_state = RTState::RT_IDLE;
			break;
		default:{
			;
		}
	}
}
