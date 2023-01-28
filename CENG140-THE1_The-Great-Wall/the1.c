#include <stdio.h>
#include <math.h>
#define W_INDEX 0
#define H_INDEX 1
#define T_INDEX 2
#define P_INDEX 3
#define S_INDEX 4
#define A_INDEX 5
#define N_INDEX 6

int Wall[100000];

int main()
{
    int WallLength,NumTribe,Width,Height,Time,Position,Speed,Interval,AttackNum,i,j,CurAttack,l,CurAttPos;

    scanf("%d", &WallLength);
    scanf("%d", &NumTribe);
    for ( j = 0; j < NumTribe; j++)     /* Tribe iteration */
    {
        scanf("%d %d %d %d %d %d %d", &Width, &Height, &Time, &Position, &Speed, &Interval, &AttackNum);
        for ( CurAttack = 0; CurAttack < AttackNum; CurAttack++)    /* Attack iteration */
        {
            for ( l = 0; l < Width; l++)    /* Tribe width traversal */
            {
                CurAttPos=Position+CurAttack*Speed*Interval+l;
                if (CurAttPos < 0 || CurAttPos > WallLength) continue;
                if (Wall[CurAttPos] < Height) Wall[CurAttPos] = Height;
            }
        }
    }
    for (i = 0 ; i < WallLength; i++)  /* Wall Finalization */
    {
        if (Wall[i] < 1) Wall[i] = 1;
        printf("%d ", Wall[i]);
    }
    if (Wall[WallLength] < 1) Wall[WallLength]=1;
    printf("%d\n", Wall[WallLength]);
    return 0;
}