//  gunwipc.h
//  GunwOS
//
//  Created by Artur Danielewski on 15.04.2023.
//

#ifndef GUNWOS_GUNWIPC_H
#define GUNWOS_GUNWIPC_H

#include <types.h>

/*
    IPC path structure:
        
        * Global:
            pathComponent0 | (GNW_PATH_IPC_COMPONENT_SEPARATOR | pathComponentN) - if needed
            Examples:
                * banana
                * banana/potato/strawberry/pizza
        
        * Direct:
            GNW_PATH_IPC_DIRECT_PREFIX | GNW_PATH_IPC_COMPONENT_SEPARATOR | pathComponent0 | (GNW_PATH_IPC_COMPONENT_SEPARATOR | pathComponentN) - if needed
            Examples:
                * :/banana
                * :/banana/potato
        
        * Notification:
            GNW_PATH_IPC_NOTIFICATION_PREFIX | GNW_PATH_IPC_COMPONENT_SEPARATOR | pathComponent0 | (GNW_PATH_IPC_COMPONENT_SEPARATOR | pathComponentN) - if needed
            Examples:
                * _/banana
                * _/banana/potato
    
    Allowed characters: a-z A-Z 0-9
*/
#define GNW_PATH_IPC_NOTIFICATION_PREFIX '_'
#define GNW_PATH_IPC_BROADCAST_PREFIX '*'
#define GNW_PATH_IPC_DIRECT_PREFIX ':'
#define GNW_PATH_IPC_COMPONENT_SEPARATOR '/'
#define GNW_PATH_IPC_MAX_LENGTH 64

#define GNW_PATH_IPC_BINDING_NOTIFICATION_SESSION_DESTROYED "_/ipc/sd"

enum gnwIpcError {
    GIPCE_NONE                          = 0,
    GIPCE_INVALID_PATH                  = -2,
    GIPCE_INVALID_PARAMETER             = -3,
    GIPCE_ALREADY_EXISTS                = -4,
    GIPCE_NOT_FOUND                     = -5,
    GIPCE_IGNORED                       = -6,
    GIPCE_FORBIDDEN                     = -7,
    GIPCE_FULL                          = -8,
    GIPCE_PRECONDITION_NOT_SATISFIED    = -9,
    GIPCE_UNKNOWN                       = -1
};

enum gnwIpcBindFlag {
    GIBF_NONE,
    GIBF_BIND,
    GIBF_UPDATE,
    GIBF_UNBIND
};

struct gnwIpcBindData {
    enum gnwIpcBindFlag flag;
    size_t permissions;
};

struct gnwIpcEndpointQuery {
    procId_t sourceProcId;
    data_t data;
    size_t replySizeBytes;
    size_t token;
    bool bound;
    size_t permissions;
};

typedef void (*gnwIpcListener)(const struct gnwIpcEndpointQuery * const);

typedef void (*gnwIpcEndpointQueryDecoder)(const ptr_t, struct gnwIpcEndpointQuery * const);

#ifndef _GUNWAPI_KERNEL

extern void gnwIpcEndpointQuery_decode(const ptr_t, struct gnwIpcEndpointQuery * const);

/*
    Default listener used by APIs

    To be set in order to use APIs listening for events

    ipcSessionDestroyListener - GNW_PATH_IPC_BINDING_NOTIFICATION_SESSION_DESTROYED
*/
extern gnwIpcListener ipcSessionDestroyListener;

/*
    Registers the process as global IPC receiver for given path

    For all types of paths, although for notification paths
    (with GNW_PATH_IPC_NOTIFICATION_PREFIX prefix)
    usage of ipcRegisterNotification function is recommended

    Params:
        * path - IPC path (see line 14)
        * handler - IPC message handler
        * bindingRequired - marks that the endpoint requires previous binding (ignored in kernel notifications)
        * permissionMask - sets the required permission set to call the endpoint (ignored in kernel notifications)
*/
extern enum gnwIpcError ipcRegister(const char * const path,
                                    const gnwIpcListener handler,
                                    const bool bindingRequired,
                                    const size_t permissionMask);

/*
    Registers the process as IPC notification receiver for given path

    For paths prefixed with GNW_PATH_IPC_NOTIFICATION_PREFIX

    Params:
        * path - IPC path (see line 14)
        * handler - IPC message handler
*/
extern enum gnwIpcError ipcRegisterNotification(const char * const path,
                                                const gnwIpcListener handler);

/*
    Sends query to specified IPC path

    Use for global paths (without prefix)

    Params:
        * path - IPC path (see line 14)
        * data - message data
        * replyData - reply data (buffer ptr, size in bytes)
        * bindData - determines binding update mode (or GIBF_NONE otherwise) and binding permissions
          
          On-send binding enables the sender process (client) to receive messages from the receiver process (server)
*/
extern enum gnwIpcError ipcSend(const char * const path,
                                const data_t data,
                                const data_t replyData,
                                const struct gnwIpcBindData bindData);

/*
    Sends query to specified IPC path of given process

    Use for direct paths (prefixed with ':')

    Params:
        * procId - receiver process identifier
        * path - IPC path (see line 14)
        * data - message data
        * replyData - reply data (buffer ptr, size in bytes)
        * bindData - determines binding update mode (or GIBF_NONE otherwise) and binding permissions
          
          On-send binding enables the sender process (client) to receive messages from the receiver process (server)
*/
extern enum gnwIpcError ipcSendDirect(const procId_t procId,
                                      const char * const path,
                                      const data_t data,
                                      const data_t replyData,
                                      const struct gnwIpcBindData bindData);

/*
    Sends response for the IPC message with provided token

    Params:
        * replyData - reply data
        * token - token of the currently handled message (see gnwIpcEndpointQuery)
        * bindData - determines binding update mode (or GIBF_NONE otherwise) and binding permissions
        
          On-reply binding enables the sender process (client) to send messages to the receiver process (server)
*/
extern enum gnwIpcError ipcReply(const data_t replyData,
                                 const size_t token,
                                 const struct gnwIpcBindData bindData);

#endif // _GUNWAPI_KERNEL

#endif // GUNWOS_GUNWIPC_H
