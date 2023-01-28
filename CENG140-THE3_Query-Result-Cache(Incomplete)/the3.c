#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "the3.h"

#define MAX_SIZE 10

QueryNode *QueryTable[MAX_SIZE];
CountNode *CountList=NULL;
char String[6];

CountNode* Insert_Count_Node(CountNode* count_list_head, int query_count)
{
    CountNode *currentNode, *traversalNode = count_list_head;
    while(traversalNode)
    {
        if (traversalNode->count == query_count) return count_list_head;
        else traversalNode = traversalNode->next;
    }
    currentNode = malloc(sizeof(CountNode));
    if (currentNode)
    {
        currentNode->count = query_count;
        currentNode->querylist = NULL;
        if (count_list_head)
        {
            if (query_count < count_list_head->count)
            {
                currentNode->prev = NULL;
                currentNode->next = count_list_head;
                count_list_head->prev = currentNode;
                return currentNode;
            }
            else
            {
                traversalNode = count_list_head;
                while(traversalNode->next && (traversalNode->next)->count < query_count)
                {
                    traversalNode = traversalNode->next;
                }
                currentNode->next = traversalNode->next;
                currentNode->prev = traversalNode;
                traversalNode->next = currentNode;
                if (currentNode->next)
                {
                    (currentNode->next)->prev = currentNode;
                }
                return count_list_head;
            }
        }
        else
        {
            currentNode->prev = NULL;
            currentNode->next = NULL;
            return currentNode;
        }
    }
    else
    {
        printf("Error inserting count node.\n");
        return count_list_head;
    }
}

QueryNode* Insert_Query_Node(CountNode* count_list_head, int query_count, char* query_string)
{
    int i;
    CountNode *countTraversalNode = count_list_head;
    QueryNode *queryTraversalNode;
    while( query_count != countTraversalNode->count)
    {
        countTraversalNode = countTraversalNode->next;
    }
    if (countTraversalNode->querylist)
    {
        int later;
        QueryNode *currentNode = malloc(sizeof(QueryNode));
        currentNode->query = malloc(sizeof(char) * 6);
        for (i = 0; query_string[i]; i++)
        {
            (currentNode->query)[i] = query_string[i];
        }
        (currentNode->query)[i] = 0;
        queryTraversalNode = *(countTraversalNode->querylist);
        while (1)
        {
            later = 0;
            for (i = 0; query_string[i]; i++)
            {
                char curChar;
                curChar = (queryTraversalNode->query)[i];
                if (query_string[i] > curChar)
                {
                    later = 1;
                    break;
                }
                else if (query_string[i] < (queryTraversalNode->query)[i])
                {
                    break;
                }
            }
            if (later && !(queryTraversalNode->next))
            {
                currentNode->next = NULL;
                currentNode->prev = queryTraversalNode;                
                queryTraversalNode->next = currentNode;
                return currentNode;
            }
            else if (later)
            {
                queryTraversalNode = queryTraversalNode->next;
            }
            else
            {
                currentNode->next = queryTraversalNode;
                currentNode->prev = queryTraversalNode->prev;
                queryTraversalNode->prev = currentNode;
                if (currentNode->prev)
                {
                    (currentNode->prev)->next = currentNode;
                }
                if ((*(countTraversalNode->querylist))->prev)
                {
                    *(countTraversalNode->querylist) = currentNode;
                }
                return currentNode;
            }
        }
    }
    else
    {
        countTraversalNode->querylist = malloc(sizeof(QueryNode*));
        queryTraversalNode = malloc(sizeof(QueryNode));
        queryTraversalNode->next = NULL;
        queryTraversalNode->prev = NULL;
        queryTraversalNode->query = malloc(sizeof(char) * 6);
        for (i = 0; query_string[i]; i++)
        {            
            (queryTraversalNode->query)[i] = query_string[i];
        }
        (queryTraversalNode->query)[i] = 0;
        *(countTraversalNode->querylist) = queryTraversalNode;
        return queryTraversalNode;
    }
}

CountNode* Find_Count_Node(CountNode* traversal_count_node, char* check_string)
{
    while(1)
    {
        QueryNode *currentNode = *(traversal_count_node->querylist);
        while (currentNode)
        {
            if (!strcmp(currentNode->query,check_string))
            {
                return traversal_count_node;
            }
            currentNode = currentNode->next;
        }
        traversal_count_node = traversal_count_node->next;
    }
}

void Remove_Query_Node(CountNode* traversal_count_node, int query_count, char* query_string)
{
    QueryNode* queryTraversalNode;
    while (traversal_count_node->count != query_count)
    {
        traversal_count_node = traversal_count_node->next;
    }
    queryTraversalNode = *(traversal_count_node->querylist);
    while (strcmp(queryTraversalNode->query, query_string))
    {
        queryTraversalNode = queryTraversalNode->next;
    }
    if (queryTraversalNode->next && queryTraversalNode->prev)
    {
        (queryTraversalNode->prev)->next = queryTraversalNode->next;
        (queryTraversalNode->next)->prev = queryTraversalNode->prev;
        free(queryTraversalNode);
    }
    else if (!(queryTraversalNode->next) && queryTraversalNode->prev)
    {
        (queryTraversalNode->prev)->next = queryTraversalNode->next;
        free(queryTraversalNode);
    }
    else if (queryTraversalNode->next && !(queryTraversalNode->prev))
    {
        *(traversal_count_node->querylist) = queryTraversalNode->next;
        (queryTraversalNode->next)->prev = queryTraversalNode->prev;
        free(queryTraversalNode);
    }
    else
    {
        free(queryTraversalNode);
        free(traversal_count_node->querylist);
        (traversal_count_node->next)->prev = traversal_count_node->prev;
        if (traversal_count_node->prev)
        {
            (traversal_count_node->prev)->next = traversal_count_node->next;
        }
        else
        {
            CountList = traversal_count_node->next;
        }
        free(traversal_count_node);
    }
}

int main()
{
    int i;
    int inputCount,queryCount;
    scanf("%d", &inputCount);
    for (i = 0; i < inputCount; i++)
    {
        int index = 0,curCount;
        char curChar;
        scanf(" %c", &curChar);
        while (curChar != ' ')
        {
            String[index] = curChar;
            index++;
            curChar = getchar();
        }
        String[index] = 0;
        scanf("%d", &curCount);
        CountList = Insert_Count_Node(CountList, curCount);
        QueryTable[i] = Insert_Query_Node(CountList, curCount, String);
    }
    scanf("%d", &queryCount);
    for (i = 0; i < queryCount; i++)
    {
        int j,addedNode = 0,lowestNodeCount;
        QueryNode* lowestNode;
        scanf("%s", String);
        for (j = 0; j < MAX_SIZE; j++)
        {
            QueryNode *currentNode;
            currentNode = QueryTable[j];
            if (currentNode && !strcmp(currentNode->query, String))
            {
                int curCount = Find_Count_Node(CountList, String)->count;
                CountList = Insert_Count_Node(CountList, curCount + 1);
                QueryTable[j] = Insert_Query_Node(CountList, curCount + 1, String);
                Remove_Query_Node(CountList, curCount, String);
                addedNode = 1;
                break;
            }
        }
        if (addedNode) continue;
        for (j = 0; j < MAX_SIZE; j++)
        {
            if (!(QueryTable[j]))
            {
                Insert_Count_Node(CountList, 1);
                QueryTable[j] = Insert_Query_Node(CountList, 1, String);
                addedNode = 1;
                break;
            }
        }
        if (addedNode) continue;
        lowestNode = *(CountList->querylist);
        for (j = 0; j < MAX_SIZE; j++)
        {
            if (QueryTable[j] == lowestNode) break;
        }
        lowestNodeCount = Find_Count_Node(CountList, lowestNode->query)->count;
        Remove_Query_Node(CountList, lowestNodeCount, lowestNode->query);
        Insert_Count_Node(CountList, 1);
        QueryTable[j] = Insert_Query_Node(CountList, 1, String);
    }
    while (CountList)
    {
        QueryNode *currentNode = *(CountList->querylist);
        printf("%d ", CountList->count);
        while (currentNode)
        {
            printf("%s", currentNode->query);
            if (currentNode->next) printf(" ");
            currentNode = currentNode->next;
        }
        printf("\n");
        CountList = CountList->next;
    }
    return 0;
}