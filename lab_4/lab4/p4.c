#include <stdio.h>
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS

typedef threaded_tree* threaded_ptr;
typedef struct threaded_tree{
    int left_thread;
    threaded_ptr left_child;
    char data;
    int right_thread;
    threaded_ptr right_child;
}threaded_tree;

threaded_ptr CreateNode(char data); //동적할당으로 노드생성
threaded_ptr Insucc(threaded_ptr T); //오른 서브트리에서의 left_child 반환
void InorderPrint(threaded_ptr T); //중위순회 출력
void DestroyTree(threaded_ptr T); //후위순회로 동적할당을 free

FILE *input;
FILE *output;

int main(void){

    int num; char data;

    //파일열기
    input = fopen("input.txt", "rw");
    output = fopen("output.txt", "w");

    fscanf(input, "%d\n", &num);

    //노드생성
    threaded_ptr root = CreateNode('\0');
    fscanf(input, "%c  ", &data);
    threaded_ptr a = CreateNode(data);
    fscanf(input, "%c  ", &data);    
    threaded_ptr b = CreateNode(data);
    fscanf(input, "%c  ", &data);
    threaded_ptr c = CreateNode(data);
    fscanf(input, "%c  ", &data);
    threaded_ptr d = CreateNode(data);
    fscanf(input, "%c  ", &data);
    threaded_ptr e = CreateNode(data);
    fscanf(input, "%c  ", &data);
    threaded_ptr f = CreateNode(data);
    fscanf(input, "%c  ", &data);
    threaded_ptr g = CreateNode(data);
    fscanf(input, "%c  ", &data);
    threaded_ptr h = CreateNode(data);
    fscanf(input, "%c", &data);
    threaded_ptr i = CreateNode(data);

    //트리에 노드추가
    root->left_child = a;
    root->right_child = root;
    a->left_child = b;
    a->right_child = c;
    b->left_child = d;
    b->right_child = e;
    c->left_child = f;
    c->right_child = g;
    d->left_child = h;
    d->right_child = i;

    //스레드 트리
    h->left_thread = 1;
    h->right_thread = 1;
    i->left_thread = 1;
    i->right_thread = 1;
    e->left_thread = 1;
    e->right_thread = 1;
    f->left_thread = 1;
    f->right_thread = 1;
    g->left_thread = 1;
    g->right_thread = 1;

    h->left_child = root;
    h->right_child = d;
    i->left_child = d;
    i->right_child = b;
    e->left_child = b;
    e->right_child = a;
    f->left_child = a;
    f->right_child = c;
    g->left_child = c;
    g->right_child = root;

    //중위순회로 트리 출력
    InorderPrint(root);

    //소멸
    DestroyTree(root);

    //파일닫기
    fclose(input);
    fclose(output);

    return 0;
}

threaded_ptr CreateNode(char newData){
    threaded_ptr node = (threaded_ptr)malloc(sizeof(threaded_tree));
    //struct threaded_tree N; node = &N; 여기서는 왜 필요없을까??
    node->left_thread = 0;
    node->left_child = NULL;
    node->right_thread = 0;
    node->right_child = NULL;
    node->data = newData;

    return node;
}

threaded_ptr Insucc(threaded_ptr T){
    threaded_ptr tmp;
    tmp = T->right_child;
    if(T->right_thread == 0){
        while(tmp->left_thread == 0)
            tmp = tmp->left_child;
    }
    return tmp;
}

void InorderPrint(threaded_ptr T){
    threaded_ptr tmp = T;
    for(;;){
        tmp = Insucc(tmp);
        if(tmp == T) break;
        fprintf(output, "%c ", tmp->data);
    }
    fprintf(output, "\n");
}

void DestroyTree(threaded_ptr T){
    //왼쪽자식의 왼쪽스레드가1, 오른쪽스레드가0일때 예외 => C를 free시키기 위해서
    if(T->data == '\0' && T->left_child->right_thread == 0 && T->left_child->left_thread == 1){
        free((T->left_child)->right_child);
        (T->left_child)->right_thread = 1;
    }
    //왼쪽자식의 스레드가 1이면 왼쪽자식을 free
    if((T->left_child)->left_thread == 1){
        free(T->left_child);
        T->left_thread = 1;
        //왼쪽자식이 free되었고, 오른쪽자식의 스레드가 1이면 오른쪽자식을 free
        if((T->right_child)->right_thread == 1){
            free(T->right_child);
            T->right_thread = 1;
            return;
        }
        return;
    }
    //후위순회로 트리소멸
    DestroyTree(T->left_child);
    DestroyTree(T->right_child);
    //마지막 남은 root를 free
    if(T->data == '\0' && T->left_thread == 1){
        free(T);
        return;
    }
}
