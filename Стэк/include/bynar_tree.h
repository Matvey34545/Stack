#ifndef BYNAR_TREE
#define BYNAR_TREE

#include <stdlib.h>

enum ErrorBynarTree
{
    BYNAR_OK               = 0,
    BYNAR_ERROR_ALLOCATION = 1
};

struct Top // Node
{
    Top* left;
    Top* right;
    void* element;
};

struct BynarTree
{
    Top* root;
    size_t size_element;
};

ErrorBynarTree create_bynar_tree(BynarTree *tree, size_t size_element);
ErrorBynarTree insert_top(BynarTree *tree, const void *value, int (*comparison)(const void*, const void*));
void* find_elem(const BynarTree *tree, const void *value, int (*comparison)(const void*, const void*));
bool delete_elem(BynarTree *tree, const void *value, int (*comparison)(const void*, const void*));
void destroy_bynar_tree(BynarTree *tree);

#endif
