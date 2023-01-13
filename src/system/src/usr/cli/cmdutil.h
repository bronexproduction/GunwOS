//
//  cmdutil.h
//  GunwOS
//
//  Created by Artur Danielewski on 22.03.2020.
//

#include <stdgunw/types.h>
#include <stdgunw/string.h>

#define CMD_LEN_MAX 64

#define CMD_PARAM_SEPARATOR ' '

#define CMD_NOPARAM { if (params) { k_trm_puts("Error: Parameters not supported"); return; } }

#define CMD_PARAM_INT(INDEX, TYPE, PAR, PAR_DESC) \
    TYPE PAR = 0; \
    { \
        struct k_cmd_param desc = k_cmd_paramAt(params, INDEX); \
        if (!desc.length) { \
            k_trm_puts("Error: No "); \
            k_trm_puts(PAR_DESC); \
            k_trm_puts(" specified"); \
            return; \
        } \
        char curr [desc.length + 1]; \
        uint_8 err; \
        k_cmd_paramLoad(params, curr, desc); \
        PAR = (TYPE)str2int(curr, &err); \
        if (err) { \
            k_trm_puts("Error: Incorrect "); \
            k_trm_puts(PAR_DESC); \
            k_trm_puts(": \""); \
            k_trm_puts(curr); \
            k_trm_puts("\""); \
            return;\
        } \
    }

struct k_cmd_param {
    size_t offset;
    size_t length;
};

struct k_cmd_param k_cmd_paramAt(const char * const params, unsigned int loc);
void k_cmd_paramLoad(const char * const params, char * const loc, const struct k_cmd_param pDesc);
