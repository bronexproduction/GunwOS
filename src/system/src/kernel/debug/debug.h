//
//  debug.h
//  GunwOS
//
//  Created by Artur Danielewski on 07.02.2021.
//

#include <stdgunw/string.h>
#warning that's pretty bad
#include <terminal/video.h>

#define DBG_PREVIEW_FRAME {\
    uint_32 eax;\
    uint_32 ebx;\
    uint_32 ecx;\
    uint_32 edx;\
    uint_32 esi;\
    uint_32 edi;\
    uint_32 esp;\
    uint_32 ebp;\
    __asm__ volatile ("pushal");\
    __asm__ volatile ("popl %edx");\
    __asm__ volatile ("movl %%edx, %0" : "=r"(edi));\
    __asm__ volatile ("popl %edx");\
    __asm__ volatile ("movl %%edx, %0" : "=r"(esi));\
    __asm__ volatile ("popl %edx");\
    __asm__ volatile ("movl %%edx, %0" : "=r"(ebp));\
    __asm__ volatile ("popl %edx");\
    __asm__ volatile ("movl %%edx, %0" : "=r"(esp));\
    __asm__ volatile ("popl %edx");\
    __asm__ volatile ("movl %%edx, %0" : "=r"(ebx));\
    __asm__ volatile ("popl %edx");\
    __asm__ volatile ("movl %%edx, %0" : "=r"(edx));\
    __asm__ volatile ("popl %edx");\
    __asm__ volatile ("movl %%edx, %0" : "=r"(ecx));\
    __asm__ volatile ("popl %edx");\
    __asm__ volatile ("movl %%edx, %0" : "=r"(eax));\
\
    k_vid_fill((point_t){ 0, 0 },\
               (point_t){ 79, 2 },\
               (struct k_vid_character){ 0, 0x0F });\
    *((char *)0xb8000) = ((ms % 1000000) / 100000) + 48;\
    *((char *)0xb8002) = ((ms % 100000) / 10000) + 48;\
    *((char *)0xb8004) = ((ms % 10000) / 1000) + 48;\
    *((char *)0xb8006) = ((ms % 1000) / 100) + 48;\
    *((char *)0xb8008) = ((ms % 100) / 10) + 48;\
    *((char *)0xb800A) = (ms % 10) + 48;\
\
    char buf[32];\
    struct k_vid_character characters[32];\
    size_t len = uint2hex(eax, buf);\
    k_vid_strToChar(buf, characters, 0x0F, len);\
    for (size_t i = 0; i < len; ++i) {\
        k_vid_draw(characters[i], 10 + i, 0);\
    }\
\
    len = uint2hex(ebx, buf);\
    k_vid_strToChar(buf, characters, 0x0F, len);\
    for (size_t i = 0; i < len; ++i) {\
        k_vid_draw(characters[i], 20 + i, 0);\
    }\
\
    len = uint2hex(ecx, buf);\
    k_vid_strToChar(buf, characters, 0x0F, len);\
    for (size_t i = 0; i < len; ++i) {\
        k_vid_draw(characters[i], 30 + i, 0);\
    }\
\
    len = uint2hex(edx, buf);\
    k_vid_strToChar(buf, characters, 0x0F, len);\
    for (size_t i = 0; i < len; ++i) {\
        k_vid_draw(characters[i], 40 + i, 0);\
    }\
\
    len = uint2hex(esp, buf);\
    k_vid_strToChar(buf, characters, 0x0F, len);\
    for (size_t i = 0; i < len; ++i) {\
        k_vid_draw(characters[i], 10 + i, 1);\
    }\
\
    len = uint2hex(ebp, buf);\
    k_vid_strToChar(buf, characters, 0x0F, len);\
    for (size_t i = 0; i < len; ++i) {\
        k_vid_draw(characters[i], 20 + i, 1);\
    }\
\
    len = uint2hex(esi, buf);\
    k_vid_strToChar(buf, characters, 0x0F, len);\
    for (size_t i = 0; i < len; ++i) {\
        k_vid_draw(characters[i], 30 + i, 1);\
    }\
\
    len = uint2hex(edi, buf);\
    k_vid_strToChar(buf, characters, 0x0F, len);\
    for (size_t i = 0; i < len; ++i) {\
        k_vid_draw(characters[i], 40 + i, 1);\
    }\
}
