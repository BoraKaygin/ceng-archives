#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char *Formula=NULL,*Current_Position=NULL, Letters[26]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int Size = 1;

int evaluate_string()
{
    char left,right,operator;
    if (*Current_Position == '(')
    {
        Current_Position++;
        left = evaluate_string();
        Current_Position++;
        operator = *Current_Position;
        Current_Position++;
        right = evaluate_string();
        Current_Position++;
        switch (operator)
        {
            case '&':
            {
                return (left && right);
            }
            case '|':
            {
                return (left || right);
            }
            case '>':
            {
                if (left == 1 && right == 0) return 0;
                else return 1;
            }
            case '=':
            {
                return (left == right);
            }
            default :
            {
                return 0;
            }
        }
    }
    else
    {
        if (*Current_Position == '-')
        {
            Current_Position++;
            return !(evaluate_string());
        }
        else return Letters[*Current_Position-'a'];
    }
}

void print_truth_table()
{
    int i;
    for ( i = 0; i < 26; i++)
    {
        if (Letters[i] == 1) printf("T ");
        else
        {
            if (Letters[i] == 0) printf("F ");
        }
    }
}

void update_truth_table()
{
    int i;
    for(i = 25; i >= 0; i--)
    {
        if(Letters[i] == 1)
        {
            Letters[i] = 0;
            i++;
            for(; i < 26; i++)
            {
                if (Letters[i] == 0) Letters[i] = 1;
            }
            break;
        }
    }
}

void run_parser(char *formula)
{
    print_truth_table();
    Current_Position = formula;
    if(evaluate_string()) printf("T\n");
    else printf("F\n");
}

int main()
{
    int i;
    char new_char;
    while (1)
    {
        new_char = getchar();
        if (new_char < 0)
        {
            Formula[Size-1] = 0;
            break;
        }
        if(isspace(new_char)) continue;
        if(islower(new_char)) Letters[new_char-'a'] = 1;
        Size++;
        Formula = realloc(Formula,Size);
        Formula[Size-2] = new_char;
    }
    for (i = 0; i < 26; i++) if (Letters[i] > 0) printf("%c ",'a'+i);
    printf("R\n");
    while (1)
    {
        run_parser(Formula);
        for (i = 0; i < 26; i++)
        {
            if (Letters[i] == 1)
            {
                goto UPDATE;
            }
        }
        return 0;
        UPDATE: update_truth_table();
    }
}