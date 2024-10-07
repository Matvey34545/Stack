#include "bynar_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void dtor_tree(Top *top);

ErrorBynarTree create_bynar_tree(BynarTree *tree, size_t size_element)
{
    tree->root = NULL;
    tree->size_element = size_element;

    return BYNAR_OK;
}

ErrorBynarTree insert_top(BynarTree *tree, const void *value, int (*comparison)(const void*, const void*))
{
    if (tree->root == NULL)
    {
        tree->root = (Top*)calloc(sizeof(Top), 1);
        tree->root->element = calloc(1, tree->size_element);

        if (tree->root == NULL || tree->root->element == NULL)
            return BYNAR_ERROR_ALLOCATION;

        memcpy(tree->root->element, value, tree->size_element);
        return BYNAR_OK;
    }

    Top *top_last = tree->root;
    Top *top_next = tree->root;
    while (top_next != NULL)
    {
        top_last = top_next;
        if (comparison(value, top_last->element) >= 0)
            top_next = top_last->right;
        else
            top_next = top_last->left;
    }

    top_next = (Top*)calloc(sizeof(Top), 1);

    top_next->element = calloc(1, tree->size_element);

    if (top_next == NULL || top_next->element == NULL)
        return BYNAR_ERROR_ALLOCATION;

    if (comparison(value, top_last->element) >= 0)
        top_last->right = top_next;
    else
        top_last->left = top_next;

    memcpy(top_next->element, value, tree->size_element);
    return BYNAR_OK;
}

void *find_elem(const BynarTree *tree, const void *value, int (*comparison)(const void*, const void*))
{
    Top *top_last = tree->root;
    Top *top_next = tree->root;
    bool is_find = false;

    while (top_next != NULL && !is_find)
    {
        top_last = top_next;
        if (comparison(value, top_last->element) > 0)
            top_next = top_last->right;

        else
        {
            if (comparison(value, top_last->element) < 0)
                top_next = top_last->left;

            else
                is_find = true;
        }
    }
    if (is_find)
        return top_last->element;

    return NULL;
}

bool delete_elem(BynarTree *tree, const void *value, int (*comparison)(const void*, const void*))
{
// simplify
    if (tree->root == NULL)
        return false;

    Top *top_last_last = tree->root;
    Top *top_last = tree->root;
    Top *top_next = tree->root;
    bool is_find = false;

    while (top_next != NULL &&  !is_find)
    {
        top_last_last = top_last;
        top_last = top_next;
        if (comparison(value, top_last->element) > 0)
            top_next = top_last->right;

        else
        {
            if (comparison(value, top_last->element) < 0)
                top_next = top_last->left;

            else
                is_find = true;
        }
    }
    if (!is_find)
        return is_find;

    Top *delete_top = top_last;

    if (delete_top->right == NULL)
    {
        if (comparison(top_last_last->element, delete_top->element) >= 0)
            top_last_last->left = delete_top->left;
        else
            top_last_last->right = delete_top->left;
    }
    else
    {
        top_next = delete_top->right;
        top_last = delete_top->right;
        while (top_next != NULL)
        {
            top_last = top_next;
            top_next = top_last->left;
        }
        top_last->left = delete_top->left;
        if (comparison(top_last_last->element, delete_top->element) >= 0)
        {
            if (comparison(top_last_last->element, delete_top->element) == 0)
                tree->root = delete_top->right;
            else
                top_last_last->left = delete_top->right;
        }
        else
            top_last_last->right = delete_top->right;
    }
    free(delete_top->element);
    free(delete_top);
    return true;
}

void destroy_bynar_tree(BynarTree *tree)
{
    dtor_tree(tree->root);
    free(tree->root);
    free(tree->root->element);
}

static void dtor_tree(Top *top)
{
    if (top->left != NULL)
    {
        free(top->left);
        free(top->left->element);
        dtor_tree(top->left);
    }
    if (top->right != NULL)
    {
        free(top->right);
        free(top->left->element);
        dtor_tree(top->right);
    }
}











