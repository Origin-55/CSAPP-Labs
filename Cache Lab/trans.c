/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */

/* 实验使用(s,E,b)参数为5，1，5的模拟cache */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    //64 x 64
    if(M == 64) 
    {
        for(int i=0;i<8;i++)
        {
            for(int j=0;j<8;j++) //先分8x8
            {
                int raw = 8*i,col = 8*j;
                
                    for(int k=raw;k<raw+4;k++) 再分4个4x4
                    {
                        int r0 = A[k][col],  //直接读取保存一个缓存行8个int
                            r1 = A[k][col+1],
                            r2 = A[k][col+2],
                            r3 = A[k][col+3],
                            r4 = A[k][col+4],
                            r5 = A[k][col+5],
                            r6 = A[k][col+6],
                            r7 = A[k][col+7];

                            B[col][k] = r0;  // 转置写入b左上，同时使用b的右上4x4暂存b左下的转置
                            B[col+1][k] = r1;
                            B[col+2][k] = r2;
                            B[col+3][k] = r3;
                            B[col][k+4] = r4;
                            B[col+1][k+4] = r5;
                            B[col+2][k+4] = r6;
                            B[col+3][k+4] = r7;

                    }
                    for(int k=col;k<col+4;k++)
                    {
                        int r0 = A[raw+4][k],   读取a左下，把b右上加载到缓存，一次性完成读和写，然后把刚才暂存的内容写入b左下
                            r1 = A[raw+5][k],
                            r2 = A[raw+6][k],
                            r3 = A[raw+7][k], 
                            r4 = B[k][raw+4],  
                            r5 = B[k][raw+5],
                            r6 = B[k][raw+6],
                            r7 = B[k][raw+7];

                            B[k][raw+4] = r0;   
                            B[k][raw+5] = r1;
                            B[k][raw+6] = r2;
                            B[k][raw+7] = r3;
                            B[k+4][raw] = r4; 
                            B[k+4][raw+1] = r5;
                            B[k+4][raw+2] = r6;
                            B[k+4][raw+3] = r7;

                    }

                    
                    for(int k = 4;k<8;k++) //把a右下先全部读入b再转置，减少冲突miss
                    {
                        int r0 = A[k+raw][col+4],   
                            r1 = A[k+raw][col+5],
                            r2 = A[k+raw][col+6],
                            r3 = A[k+raw][col+7];

                            B[col+k][raw+4] = r0; 
                            B[col+k][raw+5] = r1;
                            B[col+k][raw+6] = r2;
                            B[col+k][raw+7] = r3;
                            
                            
                    }
                   
                    if(i==j){ //对角线
                        for(int k = raw+4; k < raw+8; k++) 
                        {
                            for(int r = k+1; r < col+8; r++)
                            {
                                int tmp = B[k][r];
                                B[k][r] = B[r][k];
                                B[r][k] = tmp;
                            }
                        }
                        continue;
                    }

                    col = col+4;
                    raw = raw+4;
                    for(int k = 0;k<4;k++) //非对角线
                    {
                        for(int r = k+1;r < 4;r++)
                        {
                            int tmp = B[k+col][r+raw];
                            B[k+col][r+raw] = B[col+r][k+raw];
                            B[col+r][k+raw] = tmp;
                        }
                    }
                 
                
               

            }
        }
        return;
    }

    if(M == 61){ //61x67，先对前56x64采用8x8分块，再分别处理剩余61x5和3x56
        for(int i=0;i<8;i++)
        {
            for(int j=0;j<7;j++)
            {
                int raw = 8*i,col = 8*j;
                for(int k=raw;k<raw+8;k++)
                {
                
                int r0 = A[k][col],
                        r1 = A[k][col+1],
                        r2 = A[k][col+2],
                        r3 = A[k][col+3],
                        r4 = A[k][col+4],
                        r5 = A[k][col+5],
                        r6 = A[k][col+6],
                        r7 = A[k][col+7];

                        B[col][k] = r0;
                        B[col+1][k] = r1;
                        B[col+2][k] = r2;
                        B[col+3][k] = r3;
                        B[col+4][k] = r4;
                        B[col+5][k] = r5;
                        B[col+6][k] = r6;
                        B[col+7][k] = r7;
                        }
            }
        }

        for(int i=0;i<67;i++)
        {
            
           
            {
                    int r0 = A[i][56],
                        r1 = A[i][57],
                        r2 = A[i][58],
                        r3 = A[i][59],
                        r4 = A[i][60];

                        B[56][i] = r0;
                        B[57][i] = r1;
                        B[58][i] = r2;
                        B[59][i] = r3;
                        B[60][i] = r4;
                        
            }
         }

        
            for(int j=0;j<7;j++)
            {
                int raw = 8*j;
                for(int i=64;i<67;i++)
                
                {
                    int r0 = A[i][raw],
                        r1 = A[i][raw+1],
                        r2 = A[i][raw+2],
                        r3 = A[i][raw+3],
                        r4 = A[i][raw+4],
                        r5 = A[i][raw+5],
                        r6 = A[i][raw+6],
                        r7 = A[i][raw+7];

                        B[raw][i] = r0;
                        B[raw+1][i] = r1;
                        B[raw+2][i] = r2;
                        B[raw+3][i] = r3;
                        B[raw+4][i] = r4;
                        B[raw+5][i] = r5;
                        B[raw+6][i] = r6;
                        B[raw+7][i] = r7;
                }
            
        }
        

        



        return;
    }
    //32 x 32 对角线使用8x8分块减少抖动，其余部分可全部读入缓存且不冲突，直接转置即可
    for(int n=0;n<4;n++)
    {
        for(int m=0;m<4;m++)
        {
            int col = m*8,raw = n*8;
            if(m == n){
                for(int i=col;i<col+8;i++)
                {
                    int r0 = A[i][raw],
                        r1 = A[i][raw+1],
                        r2 = A[i][raw+2],
                        r3 = A[i][raw+3],
                        r4 = A[i][raw+4],
                        r5 = A[i][raw+5],
                        r6 = A[i][raw+6],
                        r7 = A[i][raw+7];

                        B[raw][i] = r0;
                        B[raw+1][i] = r1;
                        B[raw+2][i] = r2;
                        B[raw+3][i] = r3;
                        B[raw+4][i] = r4;
                        B[raw+5][i] = r5;
                        B[raw+6][i] = r6;
                        B[raw+7][i] = r7;

                }
                

            }
            else{
                for(int i=col;i<col+8;i++)
                {
                    for(int j=raw;j<raw+8;j++)
                    {
                        int tmp = A[i][j];
                        B[j][i] = tmp;
                    }
                }  
            } 
        }
    }
    

   
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

