//
//  lbatochs.cpp
//  GunwOS Tests
//
//  Created by Artur Danielewski on 18.07.2022.
//

#include <gunwtest>

#include <cstdio>

SCENARIO(FlatQEMUScenario, LbaToChs, {
    SetIPAtSymbol(".io_read_disk_calculate_chs");
    SetAX(0);
    SetBX(0);
    SetCX(0);
    SetDX(0);

    ContinueUntilSymbol(".io_read_disk_read_chs", 1);
    CheckAX(0);
    CheckBX(0);
    CheckCX(0);
    CheckDX(0);
})
