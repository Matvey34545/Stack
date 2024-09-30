#ifndef BYNAR_TREE
#define BYNAR_TREE

#include <stdlib.h>

struct Top
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

void create_bynar_tree(BynarTree *tree, size_t size_element);
void insert_top(BynarTree *tree, void *value, int (*comparison)(const void*, const void*));
void* find_elem(BynarTree *tree, void *value, int (*comparison)(const void*, const void*));
bool delete_elem(BynarTree *tree, void *value, int (*comparison)(const void*, const void*));

#endif
