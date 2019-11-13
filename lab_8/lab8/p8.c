#include <stdio.h>
#include <stdlib.h>

//typedef struct HeapStruct PriorityQueue;
typedef struct HeapStruct{
    int Capacity;
    int Size;
    int *Elements;
} PriorityQueue;

void Init(PriorityQueue *H, int capacity);
int IsFull(PriorityQueue *H) {
    if(H->Size == (H->Capacity-1))
        return 1;
    else return 0;
}
void Swap(int *a, int *b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void Insert(PriorityQueue *H, int X);
int DeleteMin(PriorityQueue *H);
void HeapSort(int A[], int N);
//A[left] ~ A[right]를 힙으로 만드는 함
void PercDown(int A[], int left, int right);
int FindQ(PriorityQueue *H, int X);
void Print(PriorityQueue *H);

FILE *input;
FILE *output;
PriorityQueue A;

int main(void){
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");

    int i, nx;
    char ifp;
    
    fscanf(input, "%d", &nx);
    Init(&A, nx+1); //요소개수 5개
    while(EOF!=fscanf(input, "%c", &ifp)){
        switch(ifp){
            int num;
            int f_n;
            case 'i' : {
                        fscanf(input, "%d", &num);
                        Insert(&A, num);
                        break;
                       }
            case 'f' : {
                        fscanf(input, "%d", &num);
                        f_n = FindQ(&A, num);
                        if(f_n < 0){
                            fprintf(output, "%d is not in the heap.\n", num);
                        }
                        else
                            fprintf(output, "%d is in the heap.\n", num);
                        break;
                       }
            case 'p' : {
                        //HeapSort(A.Elements, nx+1);
                        Print(&A);
                        return;
                       }
        }
    }
    free(A.Elements);

    fclose(input);
    fclose(output);

    return 0;
}

void Init(PriorityQueue *H, int capacity){
    H->Capacity = capacity;
    H->Size = 0;
    H->Elements = malloc(sizeof(int)*H->Capacity);
    H->Elements[0] = 9999;
}

void Insert(PriorityQueue *H, int X){
    int i;
    int f_n;
    f_n = FindQ(H, X);
    if(IsFull(H)==1){
        fprintf(output, "Priority Queue is Full\n");
        return;
    }
    //find한 값이 이미 존재하면 Insert함수 종료한다 
    if(f_n >= 0){
        fprintf(output, "%d is already in the heap.\n", X);
        return;
    }
    //트리를 거슬러 올라가면서 부모노드와 비교
    //i가 루트노드가 아니면서 삽입할 X값이 i의 부모노드보다 작으면
    for(i=++H->Size; H->Elements[i/2] < X; i/=2)
        //i번째 노드와 부모 노드를 교환한다
        H->Elements[i] = H->Elements[i/2];
    //새로운 노드를 삽입한다
    H->Elements[i] = X;
    fprintf(output, "insert %d\n", X);
}

int DeleteMin(PriorityQueue *H){
    int i, Child;
    int MinElement, LastElement;

    MinElement = H->Elements[1];
    LastElement = H->Elements[H->Size--];

    //PercDown
    for(i=1; i*2<=H->Size; i=Child){
        Child = i*2;
        if(Child != H->Size && H->Elements[Child + 1] > H->Elements[Child])
            Child++;
        if(LastElement < H->Elements[Child])
            H->Elements[i] = H->Elements[Child];
        else
            break;
    }
    H->Elements[i] = LastElement;
    return MinElement;
}

void HeapSort(int A[], int N){
    int i;
    for(i = N/2; i>0; i--)
        PercDown(A, i, N);
    for(i = N; i>0; i--){
        Swap(&A[1], &A[i]);
        PercDown(A, 1, i-1);
    }
}
//A[left] ~ A[right]를 힙으로 만드는 함수 
void PercDown(int A[], int left, int right){
    int temp = A[left]; //루트 노드
    int Parent, Child;
    
    for(Parent = left; Parent*2 <= right; Parent = Child){
        int cl = Parent*2; //왼쪽 자식
        int cr = cl+1; //오른쪽 자식
        Child = (cr <= right && A[cr] < A[cl]) ? cr : cl; //작은값을 선택
        if(temp < A[Child])
            break; //A[Parent] = A[Child];
        else
            A[Parent] = A[Child]; //break;
    }
    A[Parent] = temp;
}

int FindQ(PriorityQueue *H, int X){
    int i;
    for(i=1; i<=H->Size; i++){
        //printf("H[%d] : %d\t", i, H->Elements[i]);
        if(H->Elements[i] == X){
            //printf("\n");
            return X;
        }
    }
    //printf("\n");
    return -1;
}

void Print(PriorityQueue *H){
    int i;
    for(i=1; i<=H->Size; i++){
        fprintf(output, "%d ", H->Elements[i]);
    }
    fprintf(output, "\n");
}

