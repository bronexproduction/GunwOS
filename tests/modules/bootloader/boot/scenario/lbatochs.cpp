//
//  lbatochs.cpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 18.07.2022.
//

#include <scenarios>

SCENARIO(SystemQemuScenario, LbaToChs, {
    SetIPAtSymbol(".io_read_disk_calculate_chs");
    SetAX(0);
    SetBX(0);
    SetCX(0);
    SetDX(0);

    i386_regSet frame = ReadRegisters();

    ContinueUntilSymbol(".io_read_disk_read_chs", 1);
    
    frame.ax = 0x0000;
    frame.bx = 0x0000;
    frame.cx = 0x0000;
    frame.dx = 0x0000;

    CheckRegisters(frame);
})
