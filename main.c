#include <stdio.h>
#include <stdlib.h>

int GetRandom(int from, int to)
{
    SEED = SEED * 16807 % R_MAX;
    return from + (int) SEED % (to - from + 1);
}

int main()
{
    seed = 11;
    n = 50;
    m1 = 15;
    m2 = 5;

    //scanf("%d %d %d %d", seed,n, m1, m2);

    //validation
    if( seed < 0 || n < 10 || n > 100 || (m1+m2)/2 <= n/2)
        return -1;




    while(1){}
    return 0;
}
