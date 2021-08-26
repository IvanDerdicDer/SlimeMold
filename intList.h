//
// Created by ivand on 25.8.2021..
//

#ifndef SLIMEMOULDINC_COORDINATESLIST_H
#define SLIMEMOULDINC_COORDINATESLIST_H

#endif //SLIMEMOULDINC_COORDINATESLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

struct intList{
    int32_t size;
    int32_t numberOfElements;
    int32_t* list;
};

struct intList* createIntList(){
    struct intList* list = malloc(sizeof(struct intList));
    list->size = 2;
    list->numberOfElements = 0;
    list->list = malloc(sizeof(int32_t) * 2);
    return list;
}

void enlargeIntList(struct intList* oldList){
    int32_t* newList = malloc(sizeof(int32_t) * oldList->size * 2);
    for (int32_t i = 0; i < oldList->size; ++i) {
        newList[i] = oldList->list[i];
    }
    oldList->size *= 2;
    free((void*)oldList->list);
    oldList->list = newList;
}

void insertElementInt(struct intList* list, int32_t data, int32_t index){
    while(index > list->size - 1){
        enlargeIntList(list);
    }

    list->list[index] = data;
    list->numberOfElements++;
}

int32_t getElementInt(struct intList* list, int32_t index){
    int32_t data = -1;

    if(index > list->size){
        return data;
    }

    return list->list[index];
}

void destroyIntList(struct intList* list){
    free((void*)list->list);
    free((void*)list);
}