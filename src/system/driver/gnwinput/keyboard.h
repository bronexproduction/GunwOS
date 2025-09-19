//
//  keyboard.h
//  GunwOS
//
//  Created by Artur Danielewski on 30.07.2025.
//

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <types.h>

void init_keyboard();
void start_keyboard();
void emitKeyboardEvent(const char data);

#endif // KEYBOARD_H
