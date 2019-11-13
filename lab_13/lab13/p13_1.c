#include <stdio.h>
#include <stdlib.h>

void MSort(int A[], int tmp[], int left, int right); //sort하기 위해 반을기준으로 왼/오 부분으로 나누고 머지하는 함수
void Merge(int A[], int tmp[], int Lpos, int Rpos, int Rend); //실제로 병합이되면서 sorting이 되는 함수

FILE *input;
FILE *output;

int main(void){
	
	input = fopen("input13_1.txt", "r");
	output = fopen("output13_1.txt", "w");

	int input_num, key;

	fscanf(input, "%d", &input_num);

	int i = 0;
	int A[input_num];
	int tmp[input_num];

    //파일을 읽어와서 input을 한다
	fprintf(output, "input : \n");
	while(EOF != fscanf(input, "%d", &key)){
		if(i == input_num)
			break;

		A[i] = key;
		fprintf(output, "%d ", A[i]); //input 성공
		i++;
	}
	fprintf(output, "\n\n");

    //재귀적인방법으로 머지소트를하는 과정을 보인다
	fprintf(output, "recursive : \n");
	MSort(A, tmp, 0, input_num-1);

	fclose(input);
	fclose(output);
	
	return 0;
}

void MSort(int A[], int tmp[], int left, int right){
	int i, center;
	if(left < right){
		center = (left + right) / 2;
        
        //재귀(recursive)한 방법으로 sort
		MSort(A, tmp, left, center);
		MSort(A, tmp, center+1, right);
		Merge(A, tmp, left, center+1, right);
	}
}

void Merge(int A[], int tmp[], int Lpos, int Rpos, int Rend){
    int i, j, Lend, TmpPos, NumElements;
    int p1 = Lpos;  //printf("p1 : %d\t", p1);
                    //int p2 = Rpos; //printf("p2 : %d\t", p2);
                    //int p3 = Rend; //printf("p3 : %d\t", p3);
    
	Lend = Rpos-1;
	TmpPos = Lpos;
	NumElements = Rend - Lpos + 1;
                    //printf("nE : %d\n", NumElements);

	while(Lpos<=Lend && Rpos<=Rend){
		if(A[Lpos]<=A[Rpos])
			tmp[TmpPos++] = A[Lpos++];
		else
			tmp[TmpPos++] = A[Rpos++];
	}

	while(Lpos<=Lend)
		tmp[TmpPos++] = A[Lpos++];
	while(Rpos<=Rend)
		tmp[TmpPos++] = A[Rpos++];

	for(i=0; i<NumElements; i++, Rend--)
		A[Rend] = tmp[Rend];

    //Merge하는 과정을 보이기 위한 출력
    if(p1<NumElements){
        for(i=p1; i<NumElements; i++)
            fprintf(output, "%d ", A[i]);
    }
    //p1의 인덱스가 0이 아니라는 것은 Merge하기 전의 오른쪽부분의 인덱스 시작부분이라는 것
    else{
        for(i=p1; NumElements>0 ; i++, NumElements--)
            fprintf(output, "%d ", A[i]);
    }
    fprintf(output, "\n");
    
}




