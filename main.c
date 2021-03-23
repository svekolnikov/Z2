#include <stdio.h>
#include <stdlib.h>

#define R_MAX 2147483647
static long long unsigned int SEED = 0x1;

enum Cell{
    EMPTY,
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
    if(field[pos] == EMPTY)
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
    while(i<m1)
    {
        int rnd = 0;
        do
        {
            rnd = GetRandom(1, n-1);
        }
        while(!IsFree(field, rnd));

        SetCell(field, rnd, BLOCK);

        i++;
    }

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
    while(i<m2)
    {
        int rnd = 0;
        do
        {
            rnd = GetRandom(1, n-1);
        }
        while(!IsFree(field, rnd));

        SetCell(field, rnd, BOOST);

        i++;
    }

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
void ClearCell(int *field, int pos)
{
    field[pos] = EMPTY;
}
int IsSecondPlayerFaster(int *field, int n, int player)
{
    int curPlayer = GetPlayerPos(field,n,player);
    int nextPlayer = GetPlayerPos(field,n,3 -player);

    if(nextPlayer > curPlayer)
        return 1;

    return 0;
}

int GetWinner(int *posAfter)
{
    if(posAfter[PLAYER2-1] > posAfter[PLAYER1-1])
        return PLAYER2;
    else if(posAfter[PLAYER2-1] < posAfter[PLAYER1-1])
        return PLAYER1;

    return -1;
}
int GetHotspots(int *hotspots, int n)
{
    int maxHot = 0;
    for(int i=0; i<n; i++)
    {
        if(hotspots[i] > maxHot)
            maxHot = hotspots[i];
    }
    return maxHot;
}

int GetMax(int num1, int num2)
{
    if(num1>num2)
        return num1;
    else
        return num2;
}
void ExchangePlayers(int *field,int n,int *hotspots)
{
    int pos1 = GetPlayerPos(field, n, PLAYER1);
    int pos2 = GetPlayerPos(field, n, PLAYER2);
    SetPlayerToPos(field,pos2,PLAYER1,hotspots);
    SetPlayerToPos(field,pos1,PLAYER2,hotspots);
}
int main()
{
    int r1 = 0;
    int r2 = 0;

    int posBefore[2] = {0};
    int boostBefore[2] = {0};

    int posAfter[2] = {0};
    int boostAfter[2] = {0};

    int seed = 111;
    int n = 30;
    int m1 = 15; // block
    int m2 = 0;  // boost

    scanf("%d %d %d %d", &seed, &n, &m1, &m2);

    //validation
    if( seed <= 0 || n < 10 || n > 100 || (m1+m2) > n/2)
        return 1;

    srnd(seed);

    int field[n];
    for(int i = 0;i<n;i++)
        field[i]=EMPTY;
    int hotspots[n];
    for(int i = 0;i<n;i++)
        hotspots[i]=EMPTY;

    SetBlocks(field, n, m1);
    printf("\n");
    SetBoosts(field, n, m2);
    printf("\n");

    int round = 1;
    int curP = PLAYER1;
    while(posAfter[curP-1] < n)
    {
        posBefore[curP-1] = GetPlayerPos(field,n,curP);
        boostBefore[curP-1] = boostAfter[curP-1];

        SetTwoRandoms(&r1,&r2);

        int curPos = GetPlayerPos(field, n, curP);
        int d = 0;
        if(curPos == -1)
        {
            if(r1+r2 > 7)
            {
                d = r1+r2 -7 + boostBefore[curP-1];

                int nextCell = field[d];
                if(nextCell == EMPTY)
                {
                    SetPlayerToPos(field,d,curP,hotspots);
                    posAfter[curP-1] = d;
                }
                else if(nextCell == BLOCK)
                {
                    SetCell(field, d, EMPTY);
                    posAfter[curP-1] = posBefore[curP-1];
                    if(boostBefore[curP-1] > 0)
                        boostAfter[curP-1] = boostBefore[curP-1] -1;

                    hotspots[d]++;

                }
                else if(nextCell == BOOST)
                {
                    posAfter[curP-1] = d;
                    boostAfter[curP-1]++;
                    SetPlayerToPos(field,d,curP,hotspots);
                }
                else if(nextCell == 3-curP) // opposite player
                {
                    SetPlayerToPos(field,d,curP,hotspots);
                    posAfter[curP-1] = d;
                }
            }
            else
                posAfter[curP-1] = GetPlayerPos(field,n,curP);
        }
        else // Player on the field
        {
            if(((((r1 == r2) && r1 == 6)&&IsSecondPlayerFaster(field,n,curP) == 1) ||
               (((r1 == r2) && r1 == 1)&&IsSecondPlayerFaster(field,n,curP) == 0))&& GetPlayerPos(field,n,3-curP) != -1 )
            {
                ExchangePlayers(field,n,hotspots);
                posAfter[curP-1] = GetPlayerPos(field,n,curP);
            }
            else
            {
                d = GetPlayerPos(field,n,curP) + GetMax(r1,r2) + boostBefore[curP-1];

                if(d>n-1) //win
                {
                    posAfter[curP-1] = d;
                    PrintStep(round,curP,posBefore[curP-1],boostBefore[curP-1],r1,r2,posAfter[curP-1],boostAfter[curP-1]);
                    break;
                }


                SetCell(field, GetPlayerPos(field,n,curP), EMPTY);

                int nextCell = field[d];
                if(nextCell == EMPTY)
                {
                    SetPlayerToPos(field,d,curP,hotspots);
                    posAfter[curP-1] = d;
                }
                else if(nextCell == BLOCK)
                {

                    if(boostBefore[curP-1] > 0)
                    {
                        SetPlayerToPos(field,d,curP,hotspots);
                        boostAfter[curP-1] = 0;
                    }
                    else
                    {
                        SetCell(field, d, EMPTY);
                        hotspots[d]++;
                    }

                    posAfter[curP-1] = GetPlayerPos(field,n,curP);
                }
                else if(nextCell == BOOST)
                {
                    boostAfter[curP-1]++;
                    SetPlayerToPos(field,d,curP,hotspots);
                    posAfter[curP-1] = d;
                }
                else if(nextCell == 3-curP) // opposite player
                {
                    SetPlayerToPos(field,d,curP,hotspots);
                    posAfter[curP-1] = d;
                }
            }
        }

        PrintStep(round,curP,posBefore[curP-1],boostBefore[curP-1],r1,r2,posAfter[curP-1],boostAfter[curP-1]);
        curP = 3 - curP;
        round++;
    }

    //Statistics
    printf("WINNER:%d", GetWinner(posAfter));
    printf("\n");
    printf("HOTSPOT:%d", GetHotspots(hotspots,n)); // 3 6 9 10 17 23 35 // 1

    return 0;
}
