#include <stdio.h>
#include <stdlib.h>

#define Max(a, b) (((a) > (b)) ? (a) : (b))

struct AVLNode;
typedef struct AVLNode *Position;
typedef struct AVLNode *AVLTree;

struct AVLNode{
    int data;
    AVLTree Left;
    AVLTree Right;
    int Height;
}AVLNode;

int Height(Position P); //높이를 리턴(리프노드를 0으로 기준삼는다)
Position SingleRotateWithLeft(Position K2); //LL => Right rotation
Position SingleRotateWithRight(Position K2); //RR => Left rotation
Position DoubleRotateWithLR(Position K3); //Left-Right rotation
Position DoubleRotateWithRL(Position K3); //Right-Left rotation
AVLTree Insert(int num, AVLTree T); //AVLtree 규칙을 지키면서 삽입
void InorderPrint(Position P); //중위순회로 노드값들과 높이를 출력
void DestroyTree(Position P); //후위순회를 이용하여 동적할당 한 노드들을 free

FILE *input;
FILE *output;
AVLTree avl;

int main(void){

    int num;

    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");
    
    while(EOF!=fscanf(input, "%d", &num)){ //파일이 끝날때까지 fscanf
        avl = Insert(num, avl);
        InorderPrint(avl);
        fprintf(output, "\n");
    }

    DestroyTree(avl); // 동적할당 한 노드들을 free
    fclose(input);
    fclose(output);

    return 0;
}

int Height(Position P){
    if(P == NULL)
        return -1;
    else
        return P->Height;
}

Position SingleRotateWithLeft(Position K2){
    Position K1;

    K1 = K2->Left;
    K2->Left = K1->Right; //오른자식의 서브트리를 왼쪽자식의 오른서브트리로
    K1->Right = K2;

    K2->Height = Max(Height(K2->Left),Height(K2->Right)) + 1;
    K1->Height = Max(Height(K1->Left),Height(K1->Right)) + 1;

    return K1;
}

Position SingleRotateWithRight(Position K2){
    Position K1;

    K1 = K2->Right;
    K2->Right = K1->Left; //왼쪽자식의 서브트리를 오른자식의 왼쪽서브트리로
    K1->Left = K2;

    K2->Height = Max(Height(K2->Left),Height(K2->Right)) +1;
    K1->Height = Max(Height(K1->Left),Height(K1->Right)) +1;

    return K1;
}

Position DoubleRotateWithLR(Position K3){
    K3->Left = SingleRotateWithRight(K3->Left); //Left rotation
    return SingleRotateWithLeft(K3); //Right rotation
}

Position DoubleRotateWithRL(Position K3){
    K3->Right = SingleRotateWithLeft(K3->Right); //Right rotation
    return SingleRotateWithRight(K3); //Left rotation
}

AVLTree Insert(int num, AVLTree T){
    if(T==NULL){
        T=malloc(sizeof(AVLNode));
        if(T==NULL)
            fprintf(output, "Out of space!!!\n");
        else{
            T->data = num;
            T->Height = 0;
            T->Left = NULL;
            T->Right = NULL;
        }
    }else if(num<T->data){
        T->Left=Insert(num,T->Left);
        if(Height(T->Left)-Height(T->Right)==2)
            if(num<T->Left->data)
                T=SingleRotateWithLeft(T);
            else
                T=DoubleRotateWithLR(T);
    }else if(num>T->data){
        T->Right=Insert(num,T->Right);
        if(Height(T->Right)-Height(T->Left)==2)
            if(num>T->Right->data)
                T=SingleRotateWithRight(T);
            else
                T=DoubleRotateWithRL(T);
    }else{
        fprintf(output, "%d already in the tree!\n", num);
    }
    T->Height=Max(Height(T->Left),Height(T->Right))+1;
    return T; //함수타입이 AVLTree이므로
}

void InorderPrint(Position P){
    if(P){
        InorderPrint(P->Left);
        fprintf(output, "%d(%d) ", P->data, P->Height);
        InorderPrint(P->Right);
    }
}

void DestroyTree(Position P){
    if(P){
        DestroyTree(P->Left);
        DestroyTree(P->Right);
        free(P);
    }
}

