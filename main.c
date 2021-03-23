#include <stdio.h>
#include <stdlib.h>

#define R_MAX 2147483647
static long long unsigned int SEED = 0x1;
void srnd(int seed) {
  SEED = seed;
}
int GetRandom(int from, int to)
{
    SEED = SEED * 16807 % R_MAX;
    return from + (int) SEED % (to - from + 1);
}

int IsFree(int *field, int pos)
{
    if(field[pos] == 0)
        return 1;

    return 0;
}

void SetCell(int *field, int pos, int value)
{
    field[pos] = value;
}

void SetBlocks(int *field, int n, int m1)
{
    int i = 0;
    do
    {
        int rnd = 0;
        do
        {
            rnd = GetRandom(1, n-1);
        }
        while(!IsFree(field, rnd));

        SetCell(field, rnd, 3);

        i++;
    }while(i<m1);

    printf("BLOCK:");
    for(int i=0; i<n;i++)
    {
        if(field[i] == 3)
            printf("%d ", i);
    }
}
void SetBoosts(int *field, int n, int m2)
{
    int i = 0;
    do
    {
        int rnd = 0;
        do
        {
            rnd = GetRandom(1, n-1);
        }
        while(!IsFree(field, rnd));

        SetCell(field, rnd, 4);

        i++;
    }while(i<m2);

    printf("BOOST:");
    for(int i=0; i<n;i++)
    {
        if(field[i] == 4)
            printf("%d ", i);
    }
}
int main()
{
    int seed = 11;
    int n = 50;
    int m1 = 15; //block
    int m2 = 5;  // boost

    //scanf("%d %d %d %d", seed,n, m1, m2);

    //validation
    if( seed < 0 || n < 10 || n > 100 || (m1+m2)/2 > n/2)
        return -1;

    srnd(seed);

    int field[n];
    for(int i;i<n;i++)
        field[i]=0;

    SetBlocks(field, n, m1);
    printf("\n");
    SetBoosts(field, n, m2);

    while(1){}
    return 0;
}
