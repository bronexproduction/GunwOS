//
//  cmdutil.h
//  GunwOS
//
//  Created by Artur Danielewski on 22.03.2020.
//

#ifndef CMDUTIL_H
#define CMDUTIL_H

#include <types.h>
#include <string.h>
#include <gunwoutput.h>

#define CMD_LEN_MAX 64

#define CMD_PARAM_SEPARATOR ' '

#define CMD_NOPARAM { if (params) { print("Error: Parameters not supported"); return; } }

#define CMD_PARAM_INT(INDEX, TYPE, PAR, PAR_DESC) \
    TYPE PAR = 0; \
    { \
        struct user_cli_cmd_param desc = user_cli_cmd_paramAt(params, INDEX); \
        if (!desc.length) { \
            print("Error: No "); \
            print(PAR_DESC); \
            print(" specified"); \
            return; \
        } \
        char curr [desc.length + 1]; \
        uint_8 err; \
        user_cli_cmd_paramLoad(params, curr, desc); \
        PAR = (TYPE)str2int(curr, &err); \
        if (err) { \
            print("Error: Incorrect "); \
            print(PAR_DESC); \
            print(": \""); \
            print(curr); \
            print("\""); \
            return;\
        } \
    }

#define CMD_PARAM_STRING(INDEX, PAR, PAR_DESC) \
    struct user_cli_cmd_param desc_##PAR = user_cli_cmd_paramAt(params, INDEX); \
    if (!desc_##PAR.length) { \
        print("Error: No "); \
        print(PAR_DESC); \
        print(" specified"); \
        return; \
    } \
    char PAR [desc_##PAR.length + 1]; \
    user_cli_cmd_paramLoad(params, PAR, desc_##PAR);

struct user_cli_cmd_param {
    size_t offset;
    size_t length;
};

struct user_cli_cmd_param user_cli_cmd_paramAt(const char * const params, unsigned int loc);
void user_cli_cmd_paramLoad(const char * const params, char * const loc, const struct user_cli_cmd_param pDesc);

#endif // CMDUTIL_H
