#include <iostream>
#include <unistd.h>
#include "DOChannel.h"
#include "RTMode.h"
#include "SSMode.h"

static void print_info(DOChannel& channel1){
    std::cout << "Channel num: " << static_cast<int>(channel1.getChannelNum()) << " ";
    std::cout << "Triac signal: " << static_cast<int>(channel1.getTriacSignal()) << " ";
    std::cout << "Relay signal: " << static_cast<int>(channel1.getRelaySignal()) << " ";
    std::cout << "Channel status: " << static_cast<int>(channel1.getChannelStatus()) << std::endl;
}

int main(){
    uint16_t tick_period = 10;
    uint8_t output1_num = 1;
    RTMode relay_triac_mode{tick_period};
    SSMode soft_start_mode{tick_period};

    DOChannel channel1{output1_num, &relay_triac_mode};
    channel1.setChannelOutputState(ChannelMode::outputState::STATE_ON);

    for (int i = 0; i < 15; i++){
        channel1.tickHandler();
        print_info(channel1);
    }

    channel1.setChannelMode(&soft_start_mode);
    channel1.setChannelOutputState(ChannelMode::outputState::STATE_ON);

    for (int i = 0; i < 400; i++){
        channel1.tickHandler();
        print_info(channel1);
    }

    return 0;
}