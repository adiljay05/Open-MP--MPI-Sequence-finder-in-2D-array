/****************************************************************************************
*   Name:   Jawad Adil                                                                  *
*   GC N0.: 3049429                                                                     *
*                                                                                       *
*   If multiple 0's are shared with single 1, it is counted as a shared number          *
*   If multiple 1's are shared with single 0, it is not counter as a shared number.     *
*                                                                                       *
*   For Example:                                                                        *
*       1 0 2                                                                           *
*       1 5 3                                                                           *
*       2 0 2                                                                           *
*       *  There a single shared number that is [1,0] index's 1 with other 2 zeros      *
*****************************************************************************************/
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int n=1000; // size of rows and columns of 2D array
/* Below function search for 3x3 array at these locations
        -i,-j  -i,j   -i,+j
        i,-j           i,+j
        +i,-j  +i,j   +i,+j

        * where i,j is the values passed as row and col value
 */
int search_number(int arr[n][n],int row,int col,int digit_2){
    int count=0;
    for (int i=row-1;i<=row+1;i++){     //from -row to +row = 3 indexes for rows
        for(int j=col-1;j<=col+1;j++){  //from -col to +col = 3 indexes for cols
            // skip for [row,col] and -1 for both [i,j] as well as till maximum size
            if(i<0 || j<0 || i>=n || j>=n || i==row && j==col) continue;
            if(arr[i][j] == digit_2){ //if second number is found, increase the counter
                count++;
            }
        }
    }
    return count;
}
int main(int argc, char *argv[]){
    int number = 10;    // put any number here to search
    int digit_2 = number % 10;
    int digit_1 = number / 10;
    printf("Size: %d x %d\nDigit1:%d, Digit2: %d\n",n,n,digit_1,digit_2);

    float start = omp_get_wtime();

    // int a[10][10]=
    // {{2,7,4,5,0,6,1,0,6,2},{6,5,6,5,5,7,4,0,8,8},{4,1,9,0,8,2,6,6,4,9},{5,7,9,4,8,3,3,0,1,5},{4,7,6,5,9,3,7,4,2,4},{4,3,7,6,8,3,4,2,6,9},{3,7,3,5,6,2,9,1,2,0},{6,1,8,7,2,0,2,3,5,2},{6,3,0,6,6,9,7,9,2,8},{9,7,3,6,1,2,9,3,1,9}};    // array to store n*n numbers
    int a[1000][1000];
    // Assign Random numbers for 2D array
    #pragma omp parallel for
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            a[i][j] = rand()%10;
        }
    }

    int count=0;
    int shared_count = 0;
    #pragma omp parallel for
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if (a[i][j]==digit_1) { // if found 1, pass its index i,j to search_number() function
                int n2 = search_number(a,i,j,digit_2);  // funtion will return the count of 0's shared with 1
                count+=n2;
                /*  If the count returned is more than 1, it means that 1 was passed shared with more than 1 zeros   *
                *   So increment the shared_counter by 1                                                             */
                if (n2>1)shared_count++;
            }
        }
    }
    printf("Count: %d\nShared Count:%d\n",count,shared_count);
    float end = omp_get_wtime();
    printf("Work took %f seconds\n",end-start);


}
