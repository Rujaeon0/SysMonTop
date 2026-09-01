#include "pkill.h"
#include <signal.h>
#include <errno.h>
#include <sys/types.h>

KillResult send_signal_to_process(long pid, int sig){

    if(pid <= 0)
        return PKILL_ERROR_INVALID_PID;

    int result = kill((pid_t)pid,sig);
    
    if(result == 0)
        return PKILL_SUCCESS;

    switch(errno) {
        case EPERM:
            return PKILL_ERROR_PERMISSION;
        case ESRCH:
            return PKILL_ERROR_NOT_FOUND;
        default:
            return PKILL_ERROR_UNKNOWN;
    }
}

KillResult terminate_process(long pid){
    
    return send_signal_to_process(pid, SIGTERM);

}

KillResult force_kill_process(long pid){
    return send_signal_to_process(pid, SIGTERM);
}

const char *pkill_result_to_string(KillResult result){
    switch(result){
        case PKILL_SUCCESS:
            return "Process terminated successfully";
        case PKILL_ERROR_INVALID_PID:
            return "Invalid PID";
        case PKILL_ERROR_PERMISSION:
            return "Permission denied - Please switch to root";
        case PKILL_ERROR_NOT_FOUND:
            return "Process no longer exists";
        case PKILL_ERROR_UNKNOWN:
            return "Unrecognized result code";
    }
}