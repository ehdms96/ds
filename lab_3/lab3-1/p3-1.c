#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define _CRT_SECURE_NO_WARNINGS

#define MAX 100 //배열 최대 크기
int top = -1; //탑의 처음 위치
int *stack; //main에서 동적할당으로 배열 선언

int isEmpty(void); //스택이 비어있는지 확인
int isFull(void); //스택이 꽉찼는지 확인
void push(int val); //스택에 원소삽입
void pop(void); //스택에서 원소삭제

FILE *f1;
FILE *f2;

int main(void){
    
    int i=0;
    int tmp, num; //명령 갯수, push에 넣을값
    int cmp; //문자열 같은지 비교
    char pp[50]; //푸쉬인지 팝인지 판단

    f1 = fopen("input3-1.txt", "rw");
    f2 = fopen("output3-1.txt", "w");

    //동적할당으로 배열 선언
    stack = malloc(sizeof(int)*MAX);

    //파일 첫줄 값 만큼 반복문 실행
    fscanf(f1, "%d", &tmp);
    while(i < tmp){
        fscanf(f1, "%s", pp);
        
        //push면 1, pop이면 0
        if( strcmp(pp, "pop") == 0 ){
            cmp = 0;
        }
        else if( strcmp(pp, "push") == 0 ){ 
            cmp = 1;
        }
   
        switch(cmp){
            case 1:
                fscanf(f1, "%d", &num);
                push(num);
                break;
            case 0:
                pop();
                break;
        }
        
        i++;
    }

    fclose(f1);
    fclose(f2);

    free(stack);

    return 0;
}

//스택이 비어있는지 확인
int isEmpty(void){
    return (top == -1);
}

//스택이 꽉찼는지 확인
int isFull(void){
    return (top == (MAX-1));
}

//스택에 원소삽입
void push(int val){
    if(isFull()){
        fprintf(f2,"Full\n");
    } else {
        stack[++top] = val;
    }

}

//스택에서 원소삭제
void pop(void){
    if(isEmpty()){
        fprintf(f2,"Empty\n");
    } else {
        fprintf(f2,"%d\n", stack[top]);
        top--;
    }
}

