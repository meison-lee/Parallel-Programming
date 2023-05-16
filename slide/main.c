#include <stdio.h>
#include <stdlib.h>

void f1(int x[]) {
    printf("in function f1\n");
    int i;
    for (i = 0; i < 10; i++) {
        printf("x[%d] = ", i);
        //printf("%d\t", x[i]);
        //printf("%d\n", *(x+i));
        printf("%d\n", *(x++));
    }
}

void f2(int *x[]) {	// how to get the dimention
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            printf("%d\t", x[i][j]);
            printf("%d\n", *(*(x+i)+j));
        }
    }
}

void f3(int *x) {	// how to get the dimention
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
        	printf("%d\n", x[i * 4 + j]);	// how to get the number 4?
            printf("%d\n", *(x + i * 4 + j));
        }
    }
}




int main(int argc, char *argv[])
{
    int a[10];
    int b[3][4];
    int i, j;
    int *p, **q, **r;
    int *bp[3];
    
    int *da;
    da = malloc(10 * sizeof(int));
    
    // 1D
    for (i = 0; i < 10; i++) {
        a[i] = i + 10;
    } 
    
    p = (int *)a;
    //printf("a = %d\n", a);
    //printf("a = %d\n", p);
    //printf("a = %d\n", ++a);	// compile error
    //printf("a = %d\n", ++p);

    //for (i = 0; i < 10; i++) {
    //    printf("a[%d] = ", i);
    //    printf("%d\t", a[i]);
    //    printf("%d\t", *(a+i));
    //    printf("%d\t", p[i]);
    //    printf("%d\t", *(p+i));
    //}
    
    f1(a);
    f1(p);

    // 2D    
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            b[i][j] = i + j;
        }
    }
    
    r = (int **)b;
    
    p = (int *)b;
    bp[0] = b[0];
    bp[1] = b[1];
    bp[2] = b[2];
    
    q = (int **) bp;
    
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            printf("b[%d][%d] = ", i, j);
            printf("%d\t", b[i][j]);
            printf("%d\t", *(*(b+i)+j));
            //printf("%d\t", r[i][j]);	//runtime bus error, core dump
            //printf("%d\t", *(*(r+i)+j));	//runtime bus error, core dump
            printf("%d\t", bp[i][j]);
            printf("%d\t", *(*(bp+i)+j));
            printf("%d\t", q[i][j]);
            printf("%d\t", *(*(q+i)+j));
            
            printf("%d\n", *(p+i*4+j));
        }
    }
    f2(q);
    f2(bp);
    // f2(b);	//runtime bus error, core dump
    f3(p);
    f3((int *)b);
    
    system("PAUSE");	
    return 0;
}
