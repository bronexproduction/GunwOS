//
//  devgetcountforoperator.c
//  GunwOS
//
//  Created by Artur Danielewski on 20.02.2025.
//

#include <dev/dev.h>

size_t k_scr_usr_devGetCountForOperator(const procId_t procId,
                                        const procId_t operatorProcId) {
    return k_dev_countForOperator(operatorProcId);
}
