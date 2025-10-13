//
//  gunwoutput.c
//  GunwOS
//
//  Created by Artur Danielewski on 21.12.2023.
//

#ifndef _GUNWAPI_KERNEL

#include "../include/gunwoutput.h"
#include "../_include/scl_user.h"
#include <string.h>

enum target {
    T_LOG,
    T_TERMINAL
};

enum paramType {
    PT_I_DEFAULT,
    PT_U_DEFAULT,
    PT_UNKNOWN
};

#define IS_ESCAPE(CHARACTER) ((CHARACTER) == '\\')
#define IS_PARAM_START(CHARACTER) ((CHARACTER) == '{')
#define IS_PARAM_END(CHARACTER) ((CHARACTER) == '}')

#define _IS(STRING, EXPECTED) (!strcmpl(STRING, EXPECTED, strlen(EXPECTED)))
#define IS_SIGNED(STRING) _IS(STRING, "i")
#define IS_UNSIGNED(STRING) _IS(STRING, "u")

static size_t locateParamEndCharacter(const char * const msg, const size_t msgLength, size_t paramStartIndex) {
    for (; paramStartIndex < msgLength; ++paramStartIndex) {
        if (IS_PARAM_END(msg[paramStartIndex])) return paramStartIndex;
    }
    
    return 0;
}

static enum paramType unsafe_decodeParamType(const char * const msg,
                                             const size_t paramStartCharacterIndex,
                                             const size_t paramEndCharacterIndex) {
    if (IS_SIGNED(msg + paramStartCharacterIndex + 1)) return PT_I_DEFAULT;
    if (IS_UNSIGNED(msg + paramStartCharacterIndex + 1)) return PT_U_DEFAULT;
    
    return PT_UNKNOWN;
}


static void printSequence(enum target target,
                          const char * msg,
                          const size_t msgLength,
                          const size_t startIndex,
                          const size_t terminatorIndex) {

    if (!msg) {
        return;
    }
    if (terminatorIndex > msgLength) {
        return;
    }
    if (startIndex >= terminatorIndex) {
        return;
    }
    
    switch (target) {
        case T_LOG:
            // TBD
            SYSCALL_USER_CALL(LOG, msg + startIndex, terminatorIndex - startIndex, 0, 0);
            break;
        case T_TERMINAL:
            // TBD
            break;
    }
}

static void printParameter(enum target target,
                           enum paramType paramType,
                           __builtin_va_list * const args) {
    char seqBuf[32] = {0}; // meh!
    size_t seqLen = 0;
    
    switch (paramType) {
        case PT_I_DEFAULT:
            seqLen = int2str(__builtin_va_arg(*args, int), seqBuf);
            break;
        case PT_U_DEFAULT:
            seqLen = uint2dec(__builtin_va_arg(*args, size_t), seqBuf);
            break;
        default:
            return;
    }
    
    printSequence(target, seqBuf, seqLen, 0, seqLen);
}

static void unsafe_handleEscapeCharacter(enum target target,
                                         const char * const msg,
                                         const size_t msgLength,
                                         size_t * const sequenceStartIndexPtr,
                                         size_t * const indexPtr) {
    printSequence(target, msg, msgLength, *sequenceStartIndexPtr, *indexPtr);
    (*indexPtr) += 1;
    (*sequenceStartIndexPtr) = (*indexPtr) + 1;
    printSequence(target, msg, msgLength, *indexPtr, *sequenceStartIndexPtr);
}

static void _print(enum target target, const char * const msg, const size_t msgLength, __builtin_va_list args) {
    size_t sequenceStartIndex = 0;

    for (size_t index = 0; index < msgLength; ++index) {
        if (IS_ESCAPE(msg[index])) {
            unsafe_handleEscapeCharacter(target, msg, msgLength, &sequenceStartIndex, &index);
        } else if (IS_PARAM_START(msg[index])) {
            const size_t paramEndCharacterIndex = locateParamEndCharacter(msg, msgLength, index);
            if (!paramEndCharacterIndex) {
                continue;
            }

            const enum paramType paramType = unsafe_decodeParamType(msg, index, paramEndCharacterIndex);
            if (paramType == PT_UNKNOWN) {
                continue;
            }

            printSequence(target, msg, msgLength, sequenceStartIndex, index);
            printParameter(target, paramType, &args);

            index = paramEndCharacterIndex;
            sequenceStartIndex = paramEndCharacterIndex + 1;
        }
    }

    printSequence(target, msg, msgLength, sequenceStartIndex, msgLength);
}

void print(const char * const msg, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, msg);
    _print(T_TERMINAL, msg, strlen(msg), args);
    __builtin_va_end(args);
}

void printl(const char * const msg, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, msg);
    _print(T_TERMINAL, msg, strlen(msg), args);
    __builtin_va_end(args);
    _print(T_TERMINAL, "\n", 1, args);
}

void log(const char * const msg, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, msg);
    _print(T_LOG, msg, strlen(msg), args);
    __builtin_va_end(args);
}

void logl(const char * const msg, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, msg);
    _print(T_LOG, msg, strlen(msg), args);
    __builtin_va_end(args);
    _print(T_LOG, "\n", 1, args);
}

#endif // _GUNWAPI_KERNEL
