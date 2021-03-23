#include <stdio.h>
#include <stdlib.h>

#define R_MAX 2147483647
static long long unsigned int SEED = 0x1;

enum Cell{
    EMPTY = 0,
    PLAYER1,
    PLAYER2,
    BLOCK,
    BOOST
};

void srnd(int seed) {
  SEED = seed;
}
int GetRandom(int from, int to)
{
    SEED = SEED * 16807 % R_MAX;
    return from + (int) SEED % (to - from + 1);
}
void SetTwoRandoms(int *r1, int *r2)
{
    *r1 = GetRandom(1, 6);
    *r2 = GetRandom(1, 6);
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

        SetCell(field, rnd, BLOCK);

        i++;
    }while(i<m1);

    printf("BLOCK:");
    for(int i=0; i<n;i++)
    {
        if(field[i] == BLOCK)
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

        SetCell(field, rnd, BOOST);

        i++;
    }while(i<m2);

    printf("BOOST:");
    for(int i=0; i<n;i++)
    {
        if(field[i] == BOOST)
            printf("%d ", i);
    }
}

PrintStep(int round, int player, int posBefore,int boostBefore,int r1,int r2,int posAfter,int boostAfter)
{
    printf("[%d,%d] [%d,%d] [%d,%d] [%d,%d]\n",round, player,posBefore,boostBefore,r1,r2,posAfter,boostAfter);
}

int GetPlayerPos(int *field,int n, int player)
{
    for(int i=0; i<n; i++)
    {
        if(field[i] == player)
            return i;
    }

    return -1;
}

void SetPlayerToPos(int *field, int pos, int player, int *hotspots)
{
    field[pos] = player;
    hotspots[pos]++;
}
int main()
{
    int r1 = 0;
    int r2 = 0;

    int posBefore[2] = {0};
    int boostBefore[2] = {0};

    int posAfter[2] = {0};
    int boostAfter[2] = {0};

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
        field[i]=EMPTY;
    int hotspots[n];
    for(int i;i<n;i++)
        hotspots[i]=EMPTY;

    SetBlocks(field, n, m1);
    printf("\n");
    SetBoosts(field, n, m2);
    printf("\n");

    int round = 0;
    int curP = PLAYER1;
    while(1)
    {
        curP = 3 - curP;

        posBefore[curP-1] = posAfter[curP-1];
        boostBefore[curP-1] = boostAfter[curP-1];
        posBefore[curP-1] = posAfter[curP-1];
        boostBefore[curP-1] = boostAfter[curP-1];

        SetTwoRandoms(&r1,&r2);

        int curPos = GetPlayerPos(field, n, curP);
        if(curPos == -1)
        {
            int d = 0;
            if(r1+r2 > 7)
            {
                d = r1+r2 -7 + boostBefore[curP-1];
                SetPlayerToPos(field,d,curP,hotspots);
            }
        }
        else
        {

        }

        round++;
        PrintStep(round,curP,posBefore[curP-1],boostBefore[curP-1],r1,r2,posAfter[curP-1],boostAfter[curP-1]);
    }

    //Statistics


    while(1){}
    return 0;
}
