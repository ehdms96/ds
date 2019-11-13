#include <stdio.h>
#include <stdlib.h>
#define order 3

struct B_node {
    int n_keys; /* n_keys < order No. of key in B_node will always less than order of B tree */
    int key[order-1]; /*array of key*/
    struct B_node *child[order]; /* (n_keys+1 pointers will be in use) */
}*root=NULL;

enum KeyStatus {Duplicate,Success,InsertIt};

void insert(int key);
enum KeyStatus ins(struct B_node *r, int x, int* y, struct B_node** u);
int searchPos(int x, int *key_arr, int n);
void inorder(struct B_node *ptr);

FILE *input;
FILE *output;

int main()
{
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");
    
    char ip_c;
    int ip_n;
    
    while(EOF!=fscanf(input, "%c", &ip_c)){
        switch (ip_c) {
            case 'i':
                fscanf(input, "%d", &ip_n);
                insert(ip_n);
                break;
            case 'p':
                inorder(root);
                fprintf(output, "\n");
                break;
            default:
                break;
        }/*End of switch*/
    }/*End of while*/
    return 0;
}/*End of main()*/


void insert(int key)
{
    struct B_node *newnode;
    int upKey;
    enum KeyStatus value;
    value = ins(root, key, &upKey, &newnode);
    if (value == Duplicate)
        fprintf(output, "Key already available\n");
    if (value == InsertIt)
    {
        struct B_node *uproot = root;
        root=malloc(sizeof(struct B_node));
        root->n_keys = 1;
        root->key[0] = upKey;
        root->child[0] = uproot;
        root->child[1] = newnode;
    }/*End of if */
}/*End of insert()*/

enum KeyStatus ins(struct B_node *ptr, int key, int *upKey, struct B_node **newnode)
{
    struct B_node *newPtr, *lastPtr;
    int pos, i, n,splitPos;
    int newKey, lastKey;
    enum KeyStatus value;
    if (ptr == NULL)
    {
        *newnode = NULL;
        *upKey = key;
        return InsertIt;
    }
    n = ptr->n_keys;
    pos = searchPos(key, ptr->key, n);
    if (pos < n && key == ptr->key[pos])
        return Duplicate;
    value = ins(ptr->child[pos], key, &newKey, &newPtr);
    if (value != InsertIt)
        return value;
    /*If key in B_node is less than order-1 where order is order of B tree*/
    if (n < order - 1)
    {
        pos = searchPos(newKey, ptr->key, n);
        /*Shifting the key and pointer right for inserting the new key*/
        for (i=n; i>pos; i--)
        {
            ptr->key[i] = ptr->key[i-1];
            ptr->child[i+1] = ptr->child[i];
        }
        /*Key is inserted at exact location*/
        ptr->key[pos] = newKey;
        ptr->child[pos+1] = newPtr;
        ++ptr->n_keys; /*incrementing the number of key in B_node*/
        return Success;
    }/*End of if */
    /*If key in B_nodes are maximum and position of B_node to be inserted is last*/
    if (pos == order - 1)
    {
        lastKey = newKey;
        lastPtr = newPtr;
    }
    else /*If key in B_node are maximum and position of B_node to be inserted is not last*/
    {
        lastKey = ptr->key[order-2];
        lastPtr = ptr->child[order-1];
        for (i=order-2; i>pos; i--)
        {
            ptr->key[i] = ptr->key[i-1];
            ptr->child[i+1] = ptr->child[i];
        }
        ptr->key[pos] = newKey;
        ptr->child[pos+1] = newPtr;
    }
    splitPos = (order - 1)/2;
    (*upKey) = ptr->key[splitPos];
    
    (*newnode)=malloc(sizeof(struct B_node));/*Right node after split*/
    ptr->n_keys = splitPos; /*No. of key for left splitted node*/
    (*newnode)->n_keys = order-1-splitPos;/*No. of key for right splitted node*/
    for (i=0; i < (*newnode)->n_keys; i++)
    {
        (*newnode)->child[i] = ptr->child[i + splitPos + 1];
        if(i < (*newnode)->n_keys - 1)
            (*newnode)->key[i] = ptr->key[i + splitPos + 1];
        else
            (*newnode)->key[i] = lastKey;
    }
    (*newnode)->child[(*newnode)->n_keys] = lastPtr;
    return InsertIt;
}/*End of ins()*/

/*
 find the index isuch that ki<x<ki+1 (k0=–∞ and km=∞),
 and recursively search the subtree pointed by pi
*/
int searchPos(int key, int *key_arr, int n)
{
    int pos=0;
    while (pos < n && key > key_arr[pos])
        pos++;
    return pos;
}/*End of searchPos()*/


/*
 Function to display each key in the tree in sorted order (in-order traversal)
 struct B_node *ptr, the pointer to the node you are currently working with
*/
void inorder(struct B_node *ptr) {
    if (ptr) {
        if (ptr->n_keys >= 1) {
            inorder(ptr->child[0]);
            fprintf(output, "%d ", ptr->key[0]);
            inorder(ptr->child[1]);
            
            if (ptr->n_keys == 2) {
                fprintf(output, "%d ", ptr->key[1]);
                inorder(ptr->child[2]);
            }
        }
    }
}
