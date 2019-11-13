#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//트리 구조체
typedef struct TreeNode* treeptr;
typedef struct TreeNode{
    int value;
    treeptr left;
    treeptr right;
}TreeNode;

void s_cmp(void); //input파일을 열어서 operation에 따라 각 함수를 호출하는 함수
void InorderPrint(treeptr T); //중위순회로 트리의 노드를 출력하는 함수
void PreorderPrint(treeptr T); //전위순회로 트리의 노드를 출력하는 함수
void PostorderPrint(treeptr T); //후위순회로 트리의 노드를 출력하는 함수
void DestroyTree(treeptr T);

treeptr Insert(int x, treeptr T); //왼쪽서브트리<루트<오른쪽서브트리의key값 규칙을 지켜서 x를 삽입하는 함수
treeptr Delete(int x, treeptr T); //트리안에 있는 key값에서 x를 찾아 삭제하는 함수
treeptr Find(int x, treeptr T); //트리안에 있는 key값에서 x를 찾는 함수 
treeptr FindMax(treeptr T); //왼쪽서브트리에서 가장 큰 key값을 찾아서 반환하는 함수

FILE *input;
FILE *output;
treeptr T;

int main(void){
    
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");
	    
    s_cmp();
	
    DestroyTree(T); //동적할당한 구조체 포인터 T를 free
    
	fclose(input);
    fclose(output);

    return 0;
}

void s_cmp(void){
    char ch[2]; //i,d,f,pi,pr,po Operation에 따라 함수를 호출
    int x;
    
    while(EOF!=fscanf(input, "%s", ch)){
        if(strcmp(ch, "i") == 0){
            fscanf(input, "%d", &x);
            T = Insert(x, T); //삽입함수
        }
        else if(strcmp(ch, "d") == 0){
            fscanf(input, "%d", &x);
            Delete(x, T); //삭제함수
        }
        else if(strcmp(ch, "f") == 0){
            fscanf(input, "%d", &x);
            Find(x, T); //탐색함수
        }
        else if(strcmp(ch, "pi") == 0){
            fprintf(output, "pi - ");
            InorderPrint(T); //중위순회 함수
            fprintf(output, "\n");
        }
        else if(strcmp(ch, "pr") == 0){
            fprintf(output, "pr - ");
            PreorderPrint(T); //전위순회 함수
            fprintf(output, "\n");
        }
        else if(strcmp(ch, "po") == 0){
            fprintf(output, "po - ");
            PostorderPrint(T); //후위순회 함수
            fprintf(output, "\n");
        }
    }
}

//재귀함수를 이용하여 중위순회로 출력
void InorderPrint(treeptr T){
    if(T){
        InorderPrint(T->left);
        fprintf(output, "%d ", T->value);
        InorderPrint(T->right);
    }
}
   
//재귀함수를 이용하여 전위순회로 출력
void PreorderPrint(treeptr T){
    if(T){
        fprintf(output, "%d ", T->value);
        PreorderPrint(T->left);
        PreorderPrint(T->right);
    }
}

//재귀함수를 이용하여 후위순회로 출력
void PostorderPrint(treeptr T){
    if(T){
        PostorderPrint(T->left);
        PostorderPrint(T->right);
        fprintf(output, "%d ", T->value);
    }
}

void DestroyTree(treeptr T){
	if(T){
		DestroyTree(T->left);
		DestroyTree(T->right);
		free(T);
	}
}

treeptr Insert(int x, treeptr T){
    if(T==NULL){
        T = malloc(sizeof(TreeNode));
        if(T==NULL)
            fprintf(output, "Out of space!!!");
        else{
            T->value = x;
            T->left = NULL;
            T->right = NULL;
        }
    }
    else if(x < T->value){ //삽입하려는 값이 key값보다 작으면
        T->left = Insert(x, T->left); //왼쪽 서브트리로 이동
    }
    else if(x > T->value){ //삽입하려는 값이 key값보다 크면
        T->right = Insert(x, T->right); //오른쪽 서브트리로 이동
    }
    else //널도 아니고, key값보다 크지도 작지도 않다는 것은
        fprintf(output, "%d already exists.\n", x); //같은 값이 있다는 안내문 출력

    return T; //구조체포인터로 반환
}

treeptr Delete(int x, treeptr T){
    treeptr tmp;
    if(T==NULL)
        fprintf(output, "Deletion failed. %d does not exist.\n", x);
    else if(x < T->value) ////삭제하려는 값이 key값보다 작으면
        T->left = Delete(x, T->left); //왼쪽 서브트리로 이동
    else if(x > T->value) ////삭제하려는 값이 key값보다 크면
        T->right = Delete(x, T->right); //오른쪽 서브트리로 이동
    else if(T->left && T->right){ //자식이 두개라면
        tmp = FindMax(T->left); //왼쪽서브트리에서 가장 큰 값을 찾아서
        T->value = tmp->value; //루트값에 대입하고
        T->left = Delete(T->value, T->left); /*왼쪽서브트리에서 복사된(삭제하려는) 값을
        삭제하고 그 서브트리의 루트와 연결*/
    }
    else{ //자식이 한개 또는 0개라면
        tmp = T; //free시키기 위해 잠시 저장
        if(T->left == NULL)
            T=T->right;
        else if(T->right == NULL)
            T=T->left;
        free(tmp); //free
    }
    return T;
}

treeptr Find(int x, treeptr T){
    if(T == NULL){
        fprintf(output, "%d is not in the tree.\n", x); //없으면 없다고 출력
        return NULL;
    }
    if(x < T->value) //찾는 값이 key 값보다 작으면
        return Find(x, T->left); //왼쪽 서브트리로 이동
    else if(x > T->value) //찾는 값이 key 값보다 크면
        return Find(x, T->right); //오른쪽 서브트리로 이동
    else{
        fprintf(output, "%d is in the tree.\n", x); //찾으면 트리안에 있다고 출력
        return T;
    }
}

treeptr FindMax(treeptr T){ //(왼쪽)서브트리에서의 최댓값을 찾는 함수
    if(T==NULL)
        return NULL;
    else
        while(T->right!=NULL) //(왼쪽)서브트리에서의 가장 오른쪽이 최댓값이므로
            T = T->right; //오른쪽서브트리로 이동
    return T;
}
