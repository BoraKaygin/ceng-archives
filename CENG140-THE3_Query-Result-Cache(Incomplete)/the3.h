#ifndef __THE3_H__
#define __THE3_H__

typedef struct QueryNode
{
    char *query;
    struct QueryNode *prev;
    struct QueryNode *next;
} QueryNode;

typedef struct CountNode
{
    int count;
    struct CountNode *prev;
    struct CountNode *next;
    QueryNode **querylist;
} CountNode;

#endif
