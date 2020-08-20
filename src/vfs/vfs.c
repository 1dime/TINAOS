#include <vfs/vfs.h>
#include <string.h>
#ifndef VFS_C
#define VFS_C

vfs_tree_t* tree;
vfs_node_t* root;

uint32_t fs_get_file_size(vfs_node_t* node)
{
    if(node && node->get_file_size)
    {
        return node->get_file_size(node);
    }

    return 0;
}

uint32_t fs_read(vfs_node_t *node, uint32_t offset, uint32_t size, char *buffer)
{
    if(node && node->read)
    {
        return node->read(node, offset, size, buffer);
    }
    return -1;
}

uint32_t fs_write(vfs_node_t *node, uint32_t offset, uint32_t size, char *buffer)
{
    if(node && node->write)
    {
        return node->write(node, offset, size, buffer);
    }

    return -1;
}

void fs_open(struct vfs_node *node, uint32_t flags)
{
    if(!node) return;
    if(node->refcount >= 0) node->refcount++;
    node->open(node, flags);
}

void fs_close(vfs_node_t *node)
{
    if(!node || node == root || node->refcount == -1) return;
    node->refcount--;
    if(node->refcount == 0)
        node->close(node);
}

void mount_fs_recursion(char *path, vfs_tree_node_t* sub, vfs_node_t *fs)
{
    int found = 0;
    char *current_token = separate_string(&path, "/");
    if(current_token == NULL || !strcmp(current_token, ""))
    {
        struct entry *vfs_entry = (struct entry*) sub->node_value;
        if(vfs_entry->file)
        {
            write_error_string("mount_fs_recursion: Already mounted");
            return;
        }
        if(!strcmp(vfs_entry->entry_name, "/"))
            root = fs;
        vfs_entry->file = fs;
        return;
    }
}

void mount_fs(char *path, vfs_node_t* fs)
{
    fs->refcount = -1;
    fs->fs_type = 0;
    if(path[0] == '/' && strlen(path) == 1)
    {
        struct entry* vfs_entry = (struct entry*) tree->root_node->node_value;
        if(vfs_entry->file)
        {
            write_error_string("mount_fs: Already mounted");
            return;
        }

        root = fs;
        vfs_entry->file = fs;
        return;
    }

    mount_fs_recursion(path + 1, tree->root_node, fs);
}

#endif