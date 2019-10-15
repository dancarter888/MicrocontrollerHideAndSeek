/** @file   identical_matrices.c
    @author DANIEL SIEMERS ( ), ANN NGO (39979230)
    @date   18 Oct 2019
    @brief  Program check if two matrices are identical
    @Status Incompleted
*/

#include <stdio.h>
#define N 2

// This function returns 1 if A[][] and B[][] are identical
// otherwise returns 0

int areSame(int A[][N], int B[][N])
{
    int i, j;
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            if (A[i][j] != B[i][j])
                return 0;
    return 1;
}


int overlapPattern(int A[][N], int B[][N])
{   int sum = 0;
    int i, j;
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            if (A[i][j] == B[i][j])
                sum = sum + 1;
    resultPrint();
}

 int resultPrint(sum)
 {
    if (sum == 1)
    return 1;
    else if (sum == 2)
    return 2;
    else
    return 0;
}


int main()
{
    int A[N][N] = { {1, 1},
                    {4, 4}};

    int B[N][N] = { {1, 1},
                    {4, 4}};

    if (areSame(A, B))
        printf("Matrices are identical => Overlap");
    else if (overlapPattern(A, B))
        printf("Matrices have 1 overlap pos");
    else if (overlapPattern(A, B))
        printf("Matrices have 2 overlap pos");
    else
        printf("Matrices are not identical => Not overlap");
    return 0;
}
