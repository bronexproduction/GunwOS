//
//  cpu.c
//  GunwOS
//
//  Created by Artur Danielewski on 12.01.2024.
//

void cpu_cli() {
    __asm__ volatile ("cli");
}
