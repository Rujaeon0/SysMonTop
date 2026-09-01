#ifndef PKILL_H
#define PKILL_H


typedef enum {
    PKILL_SUCCESS,
    PKILL_ERROR_INVALID_PID,
    PKILL_ERROR_PERMISSION,
    PKILL_ERROR_NOT_FOUND,
    PKILL_ERROR_UNKNOWN
} KillResult;

KillResult terminate_process(long pid);

KillResult force_kill_process(long pid);

KillResult send_signal_to_process(long pid, int sig);

const char *pkill_result_to_string(KillResult result);

#endif