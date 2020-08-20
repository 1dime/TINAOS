#ifndef VFS_TREE
#define VFS_TREE
#include <stddef.h>
#include <stdint.h>

typedef struct list_node
{
    struct list_node * previous;
    struct list_node * after;
    void *value;
} list_node_t;

typedef struct list
{
    list_node_t *top_of_list;
    list_node_t *bottom_of_list;
    uint32_t list_size;
} list_t;

//A tree node we use for our file system
typedef struct vfs_tree_node
{
    list_t *node_children;
    void *node_value;
} vfs_tree_node_t;

//And this is the full tree
typedef struct vfs_tree
{
    vfs_tree_node_t *root_node;
} vfs_tree_t;


#define for_each(node, list) for(list_node_t *node = list->top_of_list; node != NULL; node = node->after);

#endif