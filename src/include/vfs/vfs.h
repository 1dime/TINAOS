#ifndef VFS
#include <stdint.h>
#include <vfs/vfs_tree.h>
#define VFS
#define PATH_SEPARATOR '/'
#define PATH_SEPARATOR_STRING "/"
#define PATH_UP ".."
#define PATH_DOT "."
#define FS_MAGIC 0xeeee2222

#define RDONLY 0x0000
#define WRONLY 0x0001
#define RDWR   0x0002
#define APPEND 0x0008
#define CREAT  0x0200
#define TRUNC  0x0400
#define EXCL   0x0800
#define NOFOLLOW 0x1000
#define PATH 0x2000

#define VFS_FILE 0x01
#define VFS_DIRECTORY 0x02
#define VFS_CHARDEVICE 0x04
#define VFS_BLOCKDEVICE 0x08
#define VFS_PIPE 0x20
#define VFS_SYMLINK 0x20
#define VFS_MOUNTPOINT 0x40


struct vfs_node;
typedef uint32_t (*get_file_size_callback)(struct vfs_node * node);
typedef uint32_t (*read_callback) (struct vfs_node*, uint32_t, uint32_t, char*);
typedef uint32_t (*write_callback) (struct vfs_node*, uint32_t, uint32_t, char*);
typedef void (*open_callback) (struct vfs_node*, uint32_t flags);
typedef void (*close_callback) (struct vfs_node *node);

typedef struct vfs_node
{
    char name[256];
    void* device;
    uint32_t mask;
    uint32_t prog_spec_chsm;
    uint32_t flags;
    uint32_t inode_num;
    uint32_t size;
    uint32_t fs_type;
    uint32_t open_flags;

    uint32_t create_time;
    uint32_t access_time;
    uint32_t modified_time;

    uint32_t offset;
    unsigned nlink;
    int refcount;

    get_file_size_callback get_file_size;
    read_callback read;
    write_callback write;
    open_callback open;
    close_callback close;
}vfs_node_t;

typedef struct entry
{
    char *entry_name;
    vfs_node_t *file;
} entry_t;

uint32_t fs_get_file_size(vfs_node_t* node);
uint32_t fs_read(vfs_node_t *node, uint32_t offset, uint32_t size, char *buffer);
uint32_t fs_write(vfs_node_t *node, uint32_t offset, uint32_t size, char *buffer);
void fs_open(vfs_node_t *node, uint32_t flags);
void fs_close(vfs_node_t *node);
void mount_fs_recursion(char *path, vfs_tree_node_t* sub, vfs_node_t *fs);
void mount_fs(char *path, vfs_node_t* fs);
#endif