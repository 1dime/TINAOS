#include <vfs/vfs.h>
#ifndef FCNTL
#define FCNTL

vfs_node_t* file_open(const char *name, unsigned int flags)
{
    char* token = NULL;
    char* file_name = string_duplicate(name);
    char *free_file_name = file_name;
    char *save = string_duplicate(file_name);
    char *original_name = file_name;
    char* new_start = NULL;
    vfs_node_t* after_node = NULL;
    vfs_node_t* start_point = get_mount_point(&file_name);
    if(!start_point) return NULL;
    if(file_name != NULL && ((token = separate_string(&new_start, "/")) != NULL))
    {
        
    }
}

#endif