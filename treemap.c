#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if(tree->root == NULL){
        TreeNode *aux = createTreeNode(key, value);
        tree->root = aux;
    }

    else{
        if(searchTreeMap(tree, key) != NULL) return;
        
        TreeNode *new = createTreeNode(key, value);
        new->parent = tree->root;
        if(tree->lower_than(tree->root->pair->key, key) == 1){
            tree->root->right->right = new;
            new->parent = tree->root->right;
            tree->current = tree->root->right->right;
        }
        else{
            tree->root->left->left = new;
            new->parent = tree->root->left;
            tree->current = tree->root->left->left;
        }   
    }
}

TreeNode * minimum(TreeNode * x){
    while(x != NULL){
        if(x->left == NULL) return x;
        else
            x = x->left;
    }   
    return NULL;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    Pair* resultado = searchTreeMap(tree, node->pair->key);
    if(resultado != NULL){
        if(node->left == NULL && node->right == NULL){
            TreeNode* parent = node->parent;
            if(parent != NULL){
                if(parent->left == node)
                    parent->left = NULL;
                else
                    parent->right = NULL;
            }
            else{
                tree->root = NULL;
            }
            free(node);
            return;
        }
        else if(node->left == NULL || node->right == NULL){
            TreeNode* child =(node->left !=NULL) ? node->left : node->right;
            TreeNode* parent = node->parent;
            if(parent != NULL){
                if(parent->left == node)
                    parent->left = child;
                else
                    parent->right = child;
            }
            else{
                tree->root = child;
            }
            child->parent = parent;
            free(node);
            return;
        }
        else{
            TreeNode* successor = minimum(node->right);
            int* succKey = successor->pair->key;
            void* succData = successor->pair->value;
            removeNode(tree, successor);
            node->pair->key = succKey;
            node->pair->value = succData;
        }
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode* aux = tree->root;
    while(aux != NULL){
        if(is_equal(tree, aux->pair->key,key) == 1){
            tree->current = aux;
            return aux->pair;
        }
        if(tree->lower_than(aux->pair->key,key) ==1){
            tree->current = aux;
            aux = tree->current->right;
        }
        else{
            tree->current = aux;
            aux = tree->current->left;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    tree->current = tree->root;
    while(tree->current->left != NULL){
        tree->current = tree->current->left;
    }
    return tree->current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode* aux = tree->current;

    if(tree->current->right != NULL){
        aux = minimum(tree->current->right);
        return aux->pair;
    }
    else{
        while(tree->current->parent != NULL){
            if(tree->lower_than(tree->current->parent->pair, tree->current->pair->key) == 0){
                tree->current = tree->current->parent;
                return tree->current->pair;
            }     
            tree->current = tree->current->parent;
        }
    }
    return NULL;
}
