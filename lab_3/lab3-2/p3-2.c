#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS

#define MAX 100 //배열 최대 크기
int front = -1; //앞부분쪽
int rear = -1; //뒷부분쪽
int *queue; //main에서 동적할당으로 배열 선언

int isEmpty(void); //큐가 비어있는지 확인
int isFull(void); //큐가 꽉찼는지 확인 -> 원형큐
void enQ(int val); //큐에 원소삽입
void deQ(void); //큐에서 원소삭제

FILE *f1;
FILE *f2;

int main(void){

    int i=0;
    int tmp, num; //명령갯수, 인큐에 넣을 값
    int cmp; //문자열 같은지 비교
    char idq[50]; //인큐인지 디큐인지 판단

    f1 = fopen("input3-2.txt", "rw");
    f2 = fopen("output3-2.txt", "w");
    
    //동적할당으로 큐 선언
    queue = malloc(sizeof(int)*MAX);

    //파일 첫줄 값 만큼 반복문 실행
    fscanf(f1, "%d", &tmp);
    while(i<tmp){
        fscanf(f1, "%s", idq);

        if( strcmp(idq,"enQ") == 0 ){ //같으면 0을 반환하니까
            cmp = 0; //인큐일경우 0
        } else if( strcmp(idq,"deQ") == 0 ){ //같으면 0을 반환
            cmp = 1; //디큐일경우 1
        }

        switch(cmp){
            case 0:
                fscanf(f1, "%d", &num);
                enQ(num);
                break;
            case 1:
                deQ();
                break;
        }
        i++;
    }

    fclose(f1);
    fclose(f2);
    free(queue);

    return 0;
}

//큐가 꽉차있는지 확인
int isFull(void){
    int tmp = (rear+1)%MAX; //원형 큐에서 rear+1을 MAX로 나눈 값이
    if(tmp == front) //front와 같으면 큐는 가득 차있는 상태
        return 1; //true
    else
        return 0; //false
}

//큐가 비어있는지 확인
int isEmpty(void){
    if(front == rear) //값이 같으면 큐는 비어있는 상태
        return 1; //true
    else
        return 0; //false
}

//큐에 원소 삽입 
void enQ(int val){
    if(isFull())
        fprintf(f2,"Full\n");
    else
        queue[++rear] = val;
}

//큐에서 원소 삭제 
void deQ(void){
    if(isEmpty())
        fprintf(f2,"Empty\n");
    else{
        fprintf(f2,"%d\n", queue[front+1]);
        front = (front+1)%MAX;
    }
}
