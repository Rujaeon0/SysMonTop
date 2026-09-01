#ifndef STORAGE_H
#define STORAGE_H
#define MAX_STORAGE_DEVICES 32
#define STORAGE_PATH_LEN 256

typedef struct {
    char mount_point[STORAGE_PATH_LEN];
    char device_name[STORAGE_PATH_LEN];
    char fs_type[64];
} StorageInfo;


double get_storage_usage_percentage(const char *path);
int storage_list_devices(StorageInfo devices[], int max_devices);

#endif