#include<iostream>
#include<chrono>
#include "omp.h"

using namespace std;
using namespace std::chrono;

void genearte(int a[],int b[],int n){
    for(int i=0;i<n;i++){
        a[i]=b[i]= n-i;
    }
}
void merege(int a[],int s1,int e1,int s2,int e2){
	int *temp = new int[e2-s1+1];
	int i1 = s1;
	int i2 = s2;
	int k=0;
	while(i1<e1&&i2<e2){
		if(a[i1]<a[i2])
			temp[k++] = a[i1++];
		else
			temp[k++] = a[i2++];
	}
	while(i1<e1){
		temp[k++] = a[i1++];
	}
	while(i2<e1){
		temp[k++] = a[i2++];
	}
	for(i1=s1,i2=0;(i1<=e2);i1++,i2++){
		a[i1] = temp[i2];
	}
}
void mergeserial(int a[],int low,int high){
	if(low<high){
		int middle=(low+high)/2;
		mergeserial(a,low,middle);
		mergeserial(a,middle+1,high);
		merege(a,low,middle,middle+1,high);
	}
}
void mergeparallel(int a[],int low,int high){
	if(low<high){
		int middle=(low+high)/2;
		#pragma omp sections
		{
			#pragma omp section
		{mergeserial(a,low,middle);}
			#pragma omp section
		{mergeserial(a,middle+1,high);}
		}
		merege(a,low,middle,middle+1,high);
	}
}
void serial(int a[],int n){
    time_point<system_clock> start,end;
    start = system_clock::now();

    for(int i=0;i<n-1;i++){
        for(int j=0;j<n-1;j++){
            if(a[j]>a[j+1]){
                int temp = a[j];
                a[j] = a[j+1];
                a[j+1] = temp;
            }
        }
    }
    end = system_clock::now();
    duration<double> time = end - start;
    cout<<"The time taken is "<<time.count()*1000<<endl;
}

void parallel(int a[],int n){
    time_point<system_clock> start,end;
    start = system_clock::now();
    omp_set_num_threads(2);
    int first = 0;
    for(int i=0;i<n-1;i++){
        first = i%2;
        #pragma omp parallel for 
        for(int j=first;j<n-1;j=j+2){
            if(a[j]>a[j+1]){
                int temp = a[j];
                a[j] = a[j+1];
                a[j+1] = temp;
            }
        }
    }
    end = system_clock::now();
    duration<double> time = end - start;
    cout<<"The time taken is "<<time.count()*1000<<endl;
}
int main(){
    int n;
    cout<<"Enter the Size of the array";
    cin>>n;
    int *b = new int[n];
    int *a = new int[n];
    genearte(a,b,n);
    serial(a,n);
    parallel(b,n);
    genearte(a,b,n);
    time_point<system_clock> start,end;
    start = system_clock::now();
    mergeserial(a,0,n);
    end = system_clock::now();
    duration<double> time = end - start;
    cout<<"The time taken is "<<time.count()*1000<<endl;
    start = system_clock::now();
    mergeparallel(b,0,n);
    end = system_clock::now();
    time = end - start;
    cout<<"The time taken is "<<time.count()*1000<<endl;
    return 0;
}



