#include "FuncOfMain.h"

void printHelp(void) {
    std::cout << "Give me firstly sounds of 'sth.wav' after that give me man.txt and i give you result.wav ;)" << std::endl;
    std::cout << "List of actions:" << std::endl;
    std::cout << "mute 'begin' 'end' -- mute from 'begin' to 'end';" << std::endl;
    std::cout << "mix '$otherTrack' 'begin' -- play main and 'otherTrack' from 'begin';" << std::endl;
    std::cout << "mixAlt '$otherTrack' 'begin' -- play second from main and second from 'otherTrack' from 'begin';" << std::endl;
    std::cout << "slowed 'begin' 'end' 'degree' -- play sound slower by 'degree' times from 'begin' to 'end';" << std::endl;
    std::cout << "reverb 'begin' 'end' 'degree' -- play sound bigger by 'degree' times from 'begin' to 'end'." << std::endl;
}