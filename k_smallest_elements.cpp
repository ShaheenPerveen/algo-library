
/*find the k smallest elements of an array in O(n) time. Using the Kth order statistic-random pivoting algorithm to find the kth smallest element and then looping through the array to find the elements smaller than kth smallest element.Assuming distinct elements*/

#include <stdio.h>
#include <math.h>
#include <time.h>
#define SIZE 10
#define swap(X,Y) {int temp=X; X=Y; Y=temp;}


int partition(int array[], int start, int end)
{
	if(start==end)
		return start;
	if(start>end)
		return -1;
	int pos=end+1,j;
	for(j=start+1;j<=end;j++)
	{		
		if(array[j]<=array[start] && pos!=end+1)
		{
			swap(array[j],array[pos]);
			pos++;
		}
		else if(pos==end+1 && array[j]>array[start])
			pos=j;
	}
	pos--;
	swap(array[start], array[pos]);
	return pos;
}

int order_statistic(int array[], int start, int end, int k)
{
	if(start>end || (end-start+1)<k)
		return -1;                   //return -1 
	int pivot=rand()%(end-start+1)+start, position, p;
	swap(array[pivot], array[start]);
	position=partition(array, start, end);
	p=position;
	position=position-start+1;                  //size of left partition
	if(k==position)
		return array[p];
	else if(k<position)
		return order_statistic(array, start,p-1,k);
	else
		return order_statistic(array,p+1,end,k-position);
}


void main()
{
	srand((unsigned int)time(NULL));
	int i, array[SIZE],k;
	printf("Printing the array...\n");
	for(i=0;i<SIZE;i++)
		array[i]=abs(rand()%100), printf("%d ",array[i]);
	printf("\n\nk=");
	scanf("%d",&k);
	int k_small=order_statistic(array,0,SIZE-1,k);
	printf("\n\n");
	if(k_small==-1)
	{
		printf("Not possible\n");
		return ;
	}
	printf("\nk smallest elements...\n");
	for(i=0;i<SIZE;i++)
	{
		if(array[i]<=k_small)
			printf("%d ",array[i]);
	}
}
