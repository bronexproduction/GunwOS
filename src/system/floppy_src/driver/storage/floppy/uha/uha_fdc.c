//
//  uha_fdc.c
//  GunwOS
//
//  Created by Artur Danielewski on 26.01.2021.
//

#include "../../../../../../lib/stdgunw/types.h"
#include "../../../../../../lib/stdgunw/mem.h"
#include "../floppy.h"
#include "../shared/io.h"
#include "../../../../../include/gunwstor.h"
#include "../shared/defaults.h"
#include "../proc/proc.h"

// TODO: remove
extern int s_trm_puts(const char * const s);

extern struct fdc_fddConfig fdc_configFor(const uint_32 driveID);
extern uint_8 fdc_waitForInterrupt(const time_t);
extern uint_8 fdc_readyForNonDMARead(const uint_16 base);
extern uint_8 fdc_inNonDMAExecutionPhase(const uint_16 base);
extern uint_8 fdc_waitForNonDMAExecutionPhase(const uint_16 base);
