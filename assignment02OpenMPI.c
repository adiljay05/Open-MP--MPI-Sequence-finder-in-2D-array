/****************************************************************************************
*   Name:   Jawad Adil                                                                  *
*   GC N0.: 3049429                                                                     *
*                                                                                       *
*   Root process is 0th process.                                                        *
*   1. Send the whole array from root process to process 1.                             *
*   2. Count the occurences and store it in local count.                                *
*   3. Call the reduce function and store the results to total_count.                   *
*   4. Finally print the results.                                                       *
*                                                                                       *
*****************************************************************************************/
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int N = 1000;   // size of array
MPI_Request request;
MPI_Status status;
/* Below function search for 3x3 array at these locations
        -i,-j  -i,j   -i,+j
        i,-j           i,+j
        +i,-j  +i,j   +i,+j

        * where i,j is the values passed as row and col value
 */
#define number 10    // put any number here to search
#define digit_2 number % 10
#define digit_1 number / 10
int search_second_number(int arr[N][N],int row,int col){
    int count=0;
    for (int i=row-1;i<=row+1;i++){     //from -row to +row = 3 indexes for rows
        for(int j=col-1;j<=col+1;j++){  //from -col to +col = 3 indexes for cols
            // skip for [row,col] and -1 for both [i,j] as well as till maximum size
            if(i<0 || j<0 || i>=N || j>=N || i==row && j==col) continue;
            if(arr[i][j] == digit_2){ //if second number is found, increase the counter
                count++;
            }
        }
    }
    return count;
}
int main(int argc,int *argv[]){
    int a[N][N];    // N * N sized 2D array
    int procID;
    int start_sub_index = 0,end_sub_index=0;
    int n_proc = 2;
    double starttime, endtime;
    int count=0;    // this will be local count for slave processes
    int total_count=0;  // overall count for master process
    if(procID==0){  //if root, initiallize the array and pass it to the slaves
        srand(time(NULL));
        starttime = MPI_Wtime();
        int count_1 = 0;
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                a[i][j]=rand()%10;
                // printf("%d ",a[i][j]);
                // if (a[i][j]==1)count_1++;
            }
            // printf("\n");
        }
    }
    MPI_Init(NULL,NULL);    // initiallizing MPI
    MPI_Comm_size(MPI_COMM_WORLD,&n_proc);  // getting the number of processes used
    MPI_Comm_rank(MPI_COMM_WORLD,&procID);  // getting process rank
    start_sub_index = procID*(N/n_proc);
    if(procID == (n_proc-1)){
        end_sub_index = N;
    }else{
        end_sub_index = start_sub_index + (N/n_proc);
    }
    for(int i= start_sub_index ;i< end_sub_index ;i++){   // searching the required sequence
    // if(procID!=0)count=0;
        for(int j=0;j<N;j++){
            // int num=0;
            if (a[i][j]==digit_1) {
                // printf("%d,%d = %d\nProcID=%d\n",i,j,a[i][j],procID);
                count+= search_second_number(a,i,j);
                // printf("Count: %d\n",count);
            }
        }
    }
    // reducing local count to total count
    // printf("Count local: %d\n",count);
    MPI_Reduce(&count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (procID==0)  // if root/master, print the total count
    {
        endtime = MPI_Wtime();
        // printf("\033[1m\033[31m");
        printf("Size: %dx%d\n",N,N);
        printf("Digit 1: %d, Digit 2: %d\n",1,0);
        printf("Count: %d\n",total_count);
        printf("Work took %f seconds\n",endtime-starttime);
    }
    MPI_Finalize();
}