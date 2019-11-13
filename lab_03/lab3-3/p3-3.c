#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100 //배열 최대 크기
char infix[MAX]; //중위표기법으로 된 수식배열
char postfix[MAX]; //후위표기법으로 된 수식배열 

void change(char *infix, char *postfix); //중위표기를 후위표기로 바꾸는 함수
int eval(char *postfix); //후위표기를 계산하는 함수
int precedence(char op); //연산자 우선순위를 반환

//스택구현
typedef struct Stack stack;
struct Stack{
    int *stac;
    int top;
};
void init(stack *s);//스택 선언 & top -1로 초기화
int isEmpty_s(stack *s); //스택이 비어있는지 확인
int isFull_s(stack *s); //스택이 꽉찼는지 확인
void push(stack *s, int val); //스택에 원소삽입
void pop(stack *s); //스택에서 원소삭제

//큐구현
int front = -1; //앞부분 위치
int rear = -1; //뒷부분 위치
char *queue; //main에서 동적할당으로 배열 선언
int isEmpty_q(void); //큐가 비어있는지
int isFull_q(void); //큐가 꽉찼는지
void enQ(int val); //큐에 원소삽입
void deQ(void); //큐에서 원소삭제

stack op_s; //중위->후위표기로 바꿀때 필요한 연산자용 스택
stack num_s; //후위표기를 계산하는 함수에 필요한 피연산자용 스택

FILE *input;
FILE *output;

int main(void){
    
    input = fopen("input3-3.txt", "rw");
    output = fopen("output3-3.txt", "w");

    op_s.stac = malloc(sizeof(int)*MAX);
    num_s.stac = malloc(sizeof(int)*MAX);
    queue = malloc(sizeof(char)*MAX);

    fscanf(input, "%[^#]", infix);
    fprintf(output, "Infix Form : %s\n", infix);
  
    change(infix, postfix); 
    fprintf(output, "Postfix Form : %s\n", postfix);

    eval(postfix);
    fprintf(output, "Evaluation Result : %d\n", eval(postfix) );

    fclose(input);
    fclose(output);

    free(op_s.stac);
    free(num_s.stac);
    free(queue);

    return 0;
}

//중위표기에서 후위표기로 바꾸는 함수 
void change(char *infix, char *postfix){

    init(&op_s); //연산자용 스택 
    int i;
    int len = (int)strlen(infix); //infix길이만큼 반복

    for( i=0 ; i<len ; i++ ){
        //0~9
        if((int)infix[i]>47 && (int)infix[i]<58){
            enQ(infix[i]); //피연산자는 그냥 큐에 넣는다
        }
        else{ //연산자를 만나면 스택에서 그 연산자보다 
            //낮은 우선순위의 연산자를 만날때까지 팝하여 
            //큐에 넣고 자신을 푸시한다
            switch(infix[i]){
                case '(':{ //스택에 무조건 저장
                    push(&op_s, (int)infix[i]);
                    break;}
                case ')':{ //스택에서 '('가 나올때까지 
                    //팝하여 큐에 넣고 '('는 팝하여 버린다
                    while(op_s.stac[op_s.top] != '('){
                        enQ(op_s.stac[op_s.top]);
                        pop(&op_s);
                    }
                    pop(&op_s);
                    break;}
                case '+':
                case '-':
                case '*':
                case '/':
                case '%':
                    while(!isEmpty_s(&op_s) && precedence(op_s.stac[op_s.top]) >= precedence(infix[i])){
                        enQ(op_s.stac[op_s.top]);
                        pop(&op_s);
                    }
                    push(&op_s, (int)infix[i]);
                    break;
            }
        }
    }
    //모든 입력이 끝나면 스택에 있는 연산자들을 
    //모두 팝하여 인큐에 출력한다
    while(!isEmpty_s(&op_s)){
        enQ(op_s.stac[op_s.top]);
        pop(&op_s);
    }

    //후위표기가 담긴 큐를 복사
    for( i=0 ; i<len ; i++){
        postfix[i] = queue[i];
    }
}

//후위표기를 계산하는 함수 
int eval(char *postfix){
    
    init(&num_s); //피연산자용 스택
    int op1, op2, val, i;
    int len = (int)strlen(postfix); //postfix길이만큼 반복
    char ch;

    for( i=0; i<len; i++){
        ch = postfix[i];

        if((int)ch>47 && (int)ch<58){
            val = ch - '0'; //문자형 상수를 정수형으로 변환
            push(&num_s, val);
            continue;
        }

        op2 = num_s.stac[num_s.top];
        pop(&num_s);
        op1 = num_s.stac[num_s.top];
        pop(&num_s);
    
        switch(ch){
            case '*':
                val = op1 * op2;
                break;
            case '/':
                val = op1 / op2;
                break;
            case '%':
                val = op1 % op2;
                break;
            case '+':
                val = op1 + op2;
                break;
            case '-':
                val = op1 - op2;
                break;
        }

        push(&num_s, val);
    }
    return (num_s.stac[num_s.top]);
}

//연산자 우선순위를 반환
int precedence(char op){
    if(op == '(') return 0;
    if(op == '+' || op == '-') return 1;
    if(op == '*' || op == '/' || op == '%') return 2;
    else return 3;
}

//스택구현
void init(stack *s){
    s->top = -1;}

int isEmpty_s(stack *s){ //스택이 비어있는지
    return (s->top == -1);}

int isFull_s(stack *s){ //스택이 꽉찼는지
    return (s->top == (MAX-1));}

void push(stack *s, int val){ //스택에 원소삽입
    if(isFull_s(s)){
        fprintf(output, "Stack is Full\n");}
    else {
        s->stac[++(s->top)] = val;}}

void pop(stack *s){ //스택에서 원소삭제
    if(isEmpty_s(s)){
        fprintf(output, "Stack is Empty\n");}
    else {
        (s->top)--;}}

//큐구현
int isEmpty_q(void){ //큐가 비어있는지
    if(front == rear) return 1;
    else return 0;}

int isFull_q(void){ //큐가 꽉찼는지
    int tmp = (rear+1)%MAX;
    if(tmp == front) return 1;
    else return 0;}

void enQ(int val){ //큐에 원소삽입
    if(isFull_q())
        fprintf(output, "Queue is Full\n");
    else
        queue[++rear] = val;}

void deQ(void){ //큐에서 원소삭제
    if(isEmpty_q())
        fprintf(output, "Queue is Empty\n");
    else{
        front = (front+1)%MAX;}}
