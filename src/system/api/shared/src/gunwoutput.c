//
//  gunwoutput.c
//  GunwOS
//
//  Created by Artur Danielewski on 21.12.2023.
//

#include "../include/gunwoutput.h"
#include "../include/gunwipc.h"
#include "../include/gunwctrl.h"
#include "../_include/scl_user.h"
#include <string.h>
#include <defs.h>

#define OUTPUT_PATH_TERMINAL0   "t0"

enum target {
    T_LOG,
    T_TERMINAL
};

enum paramType {
    PT_I_DEFAULT,
    PT_U_DEFAULT,
    PT_U_DEFAULT_HEX,
    PT_UNKNOWN
};

#define IS_ESCAPE(CHARACTER) ((CHARACTER) == '\\')
#define IS_PARAM_START(CHARACTER) ((CHARACTER) == '{')
#define IS_PARAM_END(CHARACTER) ((CHARACTER) == '}')

#define _IS(STRING, EXPECTED) (!strcmpl(STRING, EXPECTED, strlen(EXPECTED)))
#define IS_SIGNED(STRING) _IS(STRING, "i")
#define IS_UNSIGNED(STRING) _IS(STRING, "u")
#define IS_UNSIGNED_HEX(STRING) _IS(STRING, "h")

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
    if (IS_UNSIGNED_HEX(msg + paramStartCharacterIndex + 1)) return PT_U_DEFAULT_HEX;
    
    return PT_UNKNOWN;
}

static size_t printSequence(enum target target,
                            const char * msg,
                            const size_t msgLength,
                            const size_t startIndex,
                            const size_t terminatorIndex) {

    if (!msg) {
        return 0;
    }
    if (terminatorIndex > msgLength) {
        return 0;
    }
    if (startIndex >= terminatorIndex) {
        return 0;
    }

    const size_t length = terminatorIndex - startIndex;
    size_t written = 0;

    switch (target) {
        case T_LOG:
#ifndef _GUNWAPI_KERNEL
            SYSCALL_USER_CALL(LOG, msg + startIndex, length, 0, 0);
            written = length;
#else
        {
            extern size_t k_log_logd(const data_t);
            const data_t data = { 
                /* ptr */ (byte_t *)msg + startIndex,
                /* bytes */ length
            };
            return k_log_logd(data);
        }
#endif // _GUNWAPI_KERNEL
            break;
        case T_TERMINAL:
#ifndef _GUNWAPI_KERNEL
        for (size_t index = startIndex; index < terminatorIndex; ++index) {
            enum gnwIpcError e = ipcSend(OUTPUT_PATH_TERMINAL0,
                                         (data_t){ (ptr_t)&msg[index], sizeof(char) },
                                         (data_t){ nullptr, 0 },
                                         (struct gnwIpcBindData){ GIBF_NONE, 0 });
            if (e == GIPCE_FULL) {
                yield();
                continue;
            } else if (e == GIPCE_NOT_FOUND) {
                yield();
                continue;
            } else if (e != GIPCE_NONE) {
                break;
            } else {
                ++written;
            }
        }
#else
        // TODO: Not implemented yet
#endif // _GUNWAPI_KERNEL
        break;
    }

    return written;
}

#define _PRINT_PARAM(TYPE, LENGTH, CONV) { \
    const TYPE p = __builtin_va_arg(*args, TYPE); \
    size_t l = LENGTH(p); \
    char b[l]; \
    CONV(p, b); \
    return printSequence(target, b, l, 0, l); \
}

static size_t printParameter(enum target target,
                             enum paramType paramType,
                             const __builtin_va_list * const args) {
    switch (paramType) {
        case PT_I_DEFAULT: _PRINT_PARAM(int, intlen, int2str); break;
        case PT_U_DEFAULT: _PRINT_PARAM(size_t, declen, dec2str); break;
        case PT_U_DEFAULT_HEX: _PRINT_PARAM(size_t, hexlen, hex2str); break;
        default: return 0;
    }
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

static size_t _print(enum target target, const char * const msg, const size_t msgLength, const __builtin_va_list * const args) {
    size_t sequenceStartIndex = 0;
    size_t totalCharacters = 0;

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

            totalCharacters += printSequence(target, msg, msgLength, sequenceStartIndex, index);
            totalCharacters += printParameter(target, paramType, args);

            index = paramEndCharacterIndex;
            sequenceStartIndex = paramEndCharacterIndex + 1;
        }
    }

    totalCharacters += printSequence(target, msg, msgLength, sequenceStartIndex, msgLength);
    return totalCharacters;
}

/*
    Public functions
*/

#define _PRINT_WRAPPER(CODE) { \
    size_t total = 0; \
    { CODE; } \
    return total; \
}
#define _PRINT_WRAPPER_ARGS(CODE, LAST_STATIC_PARAM) { \
    size_t total = 0; \
    __builtin_va_list args; \
    __builtin_va_start(args, LAST_STATIC_PARAM); \
    { CODE; } \
    __builtin_va_end(args); \
    return total; \
}
#define _PRINT_NEWLINE(TARGET) _print(TARGET, "\n", 1, nullptr)

size_t printc(const char c) {
    return _print(T_TERMINAL, &c, 1, nullptr);
}

size_t print(const char * const msg) {
    _PRINT_WRAPPER(
        total += _print(T_TERMINAL, msg, strlen(msg), nullptr);
    )
}

size_t printn(const char * const msg) {
    _PRINT_WRAPPER(
        total += _print(T_TERMINAL, msg, strlen(msg), nullptr);
        total += _PRINT_NEWLINE(T_TERMINAL);
    )
}

size_t printl(const char * const msg, const size_t l) {
    _PRINT_WRAPPER(
        total += _print(T_TERMINAL, msg, l, nullptr);
    )
}

size_t println(const char * const msg, const size_t l) {
    _PRINT_WRAPPER(
        total += _print(T_TERMINAL, msg, l, nullptr);
        total += _PRINT_NEWLINE(T_TERMINAL);
    )
}

size_t printf(const char * const msg, ...) {
    _PRINT_WRAPPER_ARGS(
        total += _print(T_TERMINAL, msg, strlen(msg), &args);
    , msg)
}

size_t printfn(const char * const msg, ...) {
    _PRINT_WRAPPER_ARGS(
        total += _print(T_TERMINAL, msg, strlen(msg), &args);
        total += _PRINT_NEWLINE(T_TERMINAL);
    , msg)
}

size_t printfl(const char * const msg, const size_t l, ...) {
    _PRINT_WRAPPER_ARGS(
        total += _print(T_TERMINAL, msg, l, &args);
    , l)
}

size_t printfln(const char * const msg, const size_t l, ...) {
    _PRINT_WRAPPER_ARGS(
        total += _print(T_TERMINAL, msg, l, &args);
        total += _PRINT_NEWLINE(T_TERMINAL);
    , l)
}

size_t logc(const char c) {
    return _print(T_LOG, &c, 1, nullptr);
}

size_t log(const char * const msg) {
    _PRINT_WRAPPER(
        total += _print(T_LOG, msg, strlen(msg), nullptr);
    )
}

size_t logn(const char * const msg) {
    _PRINT_WRAPPER(
        total += _print(T_LOG, msg, strlen(msg), nullptr);
        total += _PRINT_NEWLINE(T_LOG);
    )
}

size_t logl(const char * const msg, const size_t l) {
    _PRINT_WRAPPER(
        total += _print(T_LOG, msg, l, nullptr);
    )
}

size_t logln(const char * const msg, const size_t l) {
    _PRINT_WRAPPER(
        total += _print(T_LOG, msg, l, nullptr);
        total += _PRINT_NEWLINE(T_LOG);
    )
}

size_t logf(const char * const msg, ...) {
    _PRINT_WRAPPER_ARGS(
        total += _print(T_LOG, msg, strlen(msg), &args);
    , msg)
}

size_t logfn(const char * const msg, ...) {
    _PRINT_WRAPPER_ARGS(
        total += _print(T_LOG, msg, strlen(msg), &args);
        total += _PRINT_NEWLINE(T_LOG);
    , msg)
}

size_t logfl(const char * const msg, const size_t l, ...) {
    _PRINT_WRAPPER_ARGS(
        total += _print(T_LOG, msg, l, &args);
    , l)
}

size_t logfln(const char * const msg, const size_t l, ...) {
    _PRINT_WRAPPER_ARGS(
        total += _print(T_LOG, msg, l, &args);
        total += _PRINT_NEWLINE(T_LOG);
    , l)
}
