#include "storage.h"
#include <sys/statvfs.h>
#include <stdio.h>
#include <string.h>


double get_storage_usage_percentage(const char *path){
    struct statvfs stat;
    if(statvfs(path, &stat)!=0)
        return 0.0;

    unsigned long long total = (unsigned long long)stat.f_blocks * stat.f_frsize;
    unsigned long long free_bytes = (unsigned long long)stat.f_bavail * stat.f_frsize;

    if(total == 0)
        return 0.0;


    unsigned long long used = total - free_bytes;
    return ((double)used/(double)total)*100;
}

static int is_real_filesystem(const char *fs_type){
    static const char *skip_list[] = {
         "proc", "sysfs", "devtmpfs", "tmpfs", "devpts", "cgroup", "cgroup2",
        "pstore", "bpf", "tracefs", "debugfs", "securityfs", "configfs",
        "fusectl", "mqueue", "hugetlbfs", "autofs", "binfmt_misc",
        "rpc_pipefs", "efivarfs", "overlay", "squashfs", NULL
    };

    for(int i =0; skip_list[i] != NULL; i++){
        if(strcmp(fs_type, skip_list[i]) == 0)
            return 0;
    }
    return 1;
}

int storage_list_devices(StorageInfo devices[], int max_devices){
    FILE *mounts = fopen("/proc/mounts", "r");
    if(!mounts)
        return 0;

    char line[512];
    int count = 0;

    while(fgets(line, sizeof(line),mounts) && count < max_devices){
        char device[STORAGE_PATH_LEN];
        char mount_point[STORAGE_PATH_LEN];
        char fs_type[64];

        if(sscanf(line, "%255s %255s %63s",device,mount_point,fs_type) != 3)
            continue;

        if(!is_real_filesystem(fs_type))
            continue;

        if(strncmp(mount_point, "/snap/",6) == 0)
            continue;
        if(strncmp(device, "/dev/loop", 9) == 0)
            continue;

        snprintf(devices[count].device_name, STORAGE_PATH_LEN, "%s", device);
        snprintf(devices[count].mount_point, STORAGE_PATH_LEN, "%s", mount_point);
        snprintf(devices[count].fs_type, sizeof(devices[count].fs_type), "%s", fs_type);
        count++;
    }
    fclose(mounts);
    return count;
}