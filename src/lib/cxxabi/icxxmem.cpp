//
//  icxxmem.cpp
//  GunwOS
//
//  Created by Artur Danielewski on 20.01.2018.
//

#include <stdgunw/types.h>

void *operator new(size_t size) {
    return 0; // requires system call - NOT IMPLEMENTED YET
    // return _mem_alloc(size);
}
 
void operator delete(void *p) {

}

void operator delete(void *p, size_t size) {

}
