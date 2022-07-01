#include <stdio.h>
#include <stdlib.h>

typedef struct _Node
{
    int key;
    int value;
    int hash;
    struct _Node *next;
} Node;

typedef struct
{
    Node *buckets;
    int capacity;
    int nodeCnt;
} MyHashMap;

// Copy Jenkins hash function
int HashCode(int key)
{
    int hash = key;
    hash += (int)(hash << 10);
    hash ^= hash >> 6;
    hash += (int)(hash << 3);
    hash ^= hash >> 11;
    hash += (int)(hash << 15);
    return hash;
}

int Hash(int key)
{
    if (key == 0)
    {
        return key;
    }
    key = HashCode(key);
    return key ^ (key >> 16);
}

MyHashMap *myHashMapCreate()
{
    MyHashMap *hashMap = (MyHashMap *)malloc(sizeof(MyHashMap));
    hashMap->nodeCnt = 0;
    hashMap->capacity = 16;
    hashMap->buckets = (Node *)calloc(hashMap->capacity, sizeof(Node));
    return hashMap;
}

void Resize(MyHashMap *obj)
{
    Node *oldBuckets = obj->buckets;
    int oldCapacity = obj->capacity;

    int newCapacity = oldCapacity << 1;
    Node *newBuckets = (Node *)calloc(newCapacity, sizeof(Node));

    for (int i = 0; i < oldCapacity; ++i)
    {
        if (oldBuckets[i].next != NULL)
        {
            Node *bucket = oldBuckets[i].next;
            while (bucket != NULL)
            {
                int pos = bucket->hash & (newCapacity - 1);
                if (newBuckets[pos].next == NULL)
                {
                    newBuckets[pos].next = bucket;
                }
                else
                {
                    Node *tail = newBuckets[pos].next;
                    while (tail->next != NULL)
                    {
                        tail = tail->next;
                    }
                    tail->next = bucket;
                    tail->next->next == NULL;
                }
                bucket = bucket->next;
            }
        }
    }

    obj->capacity = newCapacity;
    obj->buckets = newBuckets;

    free(oldBuckets);
    oldBuckets == NULL;
}

void myHashMapPut(MyHashMap *obj, int key, int value)
{
    if (obj->nodeCnt >= obj->capacity * 3 / 4)
    {
        Resize(obj);
    }

    int pos = Hash(key) & (obj->capacity - 1);

    if (obj->buckets[pos].next == NULL)
    {
        Node *node = (Node *)malloc(sizeof(Node));
        node->key = key;
        node->value = value;
        node->hash = Hash(key);
        node->next = NULL;
        obj->buckets[pos].next = node;
    }
    else
    {
        Node *pre = &(obj->buckets[pos]);
        Node *tail = obj->buckets[pos].next;
        while (tail != NULL)
        {
            if (tail->key == key)
            {
                tail->value = value;
                return;
            }
            pre = tail;
            tail = tail->next;
        }
        Node *node = (Node *)malloc(sizeof(Node));
        node->key = key;
        node->value = value;
        node->hash = Hash(key);
        node->next = NULL;
        pre->next = node;
    }
    ++(obj->nodeCnt);
}

int myHashMapGet(MyHashMap *obj, int key)
{
    int pos = Hash(key) & (obj->capacity - 1);
    Node *node = obj->buckets[pos].next;
    while (node != NULL)
    {
        if (node->key == key)
        {
            return node->value;
        }
        node = node->next;
    }
    return -1;
}

void myHashMapRemove(MyHashMap *obj, int key)
{
    int pos = Hash(key) & (obj->capacity - 1);
    Node *node = obj->buckets[pos].next;
    Node *pre = &(obj->buckets[pos]);
    while (node != NULL)
    {
        if (node->key == key)
        {
            Node *delete = node;
            pre->next = node->next;
            free(delete);
            --(obj->nodeCnt);
            return;
        }
        pre = node;
        node = node->next;
    }
}

void myHashMapFree(MyHashMap *obj)
{
    for (int i = 0; i < obj->capacity; ++i)
    {
        Node *bucket = obj->buckets[i].next;
        while (bucket != NULL)
        {
            Node *delete = bucket;
            bucket = bucket->next;
            free(delete);
        }
    }
    free(obj->buckets);
    obj->buckets = NULL;
    free(obj);
    obj = NULL;
}

int main()
{
    MyHashMap *hashmap = myHashMapCreate();
    for (int i = 0; i < 10; ++i)
    {
        myHashMapPut(hashmap, i << 1, i);
    }
    for (int i = 0; i < 10; ++i)
    {
        printf("%d\n", myHashMapGet(hashmap, i << 1));
    }
    for (int i = 0; i < 10; ++i)
    {
        myHashMapPut(hashmap, i << 1, i + 1);
    }
    for (int i = 0; i < 10; ++i)
    {
        printf("%d\n", myHashMapGet(hashmap, i << 1));
    }
    myHashMapFree(hashmap);
    return 0;
}
