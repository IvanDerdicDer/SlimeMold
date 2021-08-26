#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include "intQueue.h"
#include "intList.h"

#define LIVING_SPACE_DIMENSIONS 40

struct cell{
    int32_t id;
    float pressure;
    float lightLevel;
    float slimeIntensity;
    bool isFoodSource;
    bool slimeCanExpand;
    bool wasVisited;
};

// TODO[Ivan]> Write function documentation

void printLivingSpaceSlime(struct cell** livingSpace){
    /*
     * Prints out the living space
    */
    for (int32_t i = 0; i < LIVING_SPACE_DIMENSIONS; ++i) {
        for (int32_t j = 0; j < LIVING_SPACE_DIMENSIONS; ++j) {
            if(livingSpace[i][j].isFoodSource){
                fprintf(stdout, "@");
            }else if(livingSpace[i][j].slimeIntensity == 0.0f){
                fprintf(stdout, "~");
            }else if(0.0f < livingSpace[i][j].slimeIntensity && livingSpace[i][j].slimeIntensity <= 0.2f){
                fprintf(stdout, ".");
            }else if(0.2f < livingSpace[i][j].slimeIntensity && livingSpace[i][j].slimeIntensity <= 0.4f){
                fprintf(stdout, ":");
            }else if(0.4f < livingSpace[i][j].slimeIntensity && livingSpace[i][j].slimeIntensity <= 0.6f){
                fprintf(stdout, "|");
            }else if(0.6f < livingSpace[i][j].slimeIntensity && livingSpace[i][j].slimeIntensity <= 0.8f){
                fprintf(stdout, "#");
            }else if(0.8f < livingSpace[i][j].slimeIntensity && livingSpace[i][j].slimeIntensity <= 1.0f){
                fprintf(stdout, "$");
            }
        }
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
}

void printLivingSpacePressure(struct cell** livingSpace){
    /*
     * Prints out the living space pressure
    */
    for (int32_t i = 0; i < LIVING_SPACE_DIMENSIONS; ++i) {
        for (int32_t j = 0; j < LIVING_SPACE_DIMENSIONS; ++j) {
            if(livingSpace[i][j].pressure == 0.0f){
                fprintf(stdout, "~");
            }else if(0.0f < livingSpace[i][j].pressure && livingSpace[i][j].pressure <= 0.2f){
                fprintf(stdout, ".");
            }else if(0.2f < livingSpace[i][j].pressure && livingSpace[i][j].pressure <= 0.4f){
                fprintf(stdout, ":");
            }else if(0.4f < livingSpace[i][j].pressure && livingSpace[i][j].pressure <= 0.6f){
                fprintf(stdout, "|");
            }else if(0.6f < livingSpace[i][j].pressure && livingSpace[i][j].pressure <= 0.8f){
                fprintf(stdout, "#");
            }else if(0.8f < livingSpace[i][j].pressure && livingSpace[i][j].pressure <= 1.0f){
                fprintf(stdout, "$");
            }
        }
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
}

struct cell** initLivingSpace(){
    /*
     * Creates and initialises the living space
    */
    struct cell** livingSpace = malloc(sizeof(struct cell*) * LIVING_SPACE_DIMENSIONS);
    for(int32_t i = 0; i < LIVING_SPACE_DIMENSIONS; i++){
        livingSpace[i] = malloc(sizeof(struct cell) * LIVING_SPACE_DIMENSIONS);
        for(int32_t j = 0; j < LIVING_SPACE_DIMENSIONS; j++){
            livingSpace[i][j].id = i * LIVING_SPACE_DIMENSIONS + j;
            livingSpace[i][j].isFoodSource = false;
            livingSpace[i][j].pressure = 0.2f;
            livingSpace[i][j].lightLevel = 0.0f;
            livingSpace[i][j].slimeIntensity = 0.0f;
            livingSpace[i][j].slimeCanExpand = true;
            livingSpace[i][j].wasVisited = false;
        }
    }
    return livingSpace;
}

void destroyLivingSpace(struct cell** livingSpace){
    for (int32_t i = 0; i < LIVING_SPACE_DIMENSIONS; ++i) {
        free((void*)livingSpace[i]);
    }
    free((void*)livingSpace);
}

uint8_t checkCellExistence(int32_t i, int32_t j){
    /* cellExists
     * Unsigned int containing information if a cell exists in relation to (i, j) coordinate
     *   7 <--- 0
     * 0b00000000
     * position - exists(true[1]/false[0])
     *        0 - (i - 1)
     *        1 - (i + 1)
     *        2 - (j - 1)
     *        3 - (j + 1)
    */
    uint8_t cellExists = 0;

    if(i - 1 >= 0){
        cellExists |= 0b1<<0;
    }
    if(j - 1 >= 0){
        cellExists |= 0b1<<2;
    }
    if(i + 1 < LIVING_SPACE_DIMENSIONS){
        cellExists |= 0b1<<1;
    }
    if(j + 1 < LIVING_SPACE_DIMENSIONS){
        cellExists |= 0b1<<3;
    }
    return cellExists;
}

void foodSourceSlimeExpand(struct cell** livingSpace, int32_t i, int32_t j){
    /*
     * Expands slime around the food source
    */
    uint8_t cellExists = checkCellExistence(i, j);

    if(cellExists >> 0 & 0b1){
        livingSpace[i - 1][j].slimeIntensity = 1.0f;
    }
    if(cellExists >> 1 & 0b1){
        livingSpace[i + 1][j].slimeIntensity = 1.0f;
    }
    if(cellExists >> 2 & 0b1){
        livingSpace[i][j - 1].slimeIntensity = 1.0f;
    }
    if(cellExists >> 3 & 0b1){
        livingSpace[i][j + 1].slimeIntensity = 1.0f;
    }
}

struct intList* findHighestPressureSpots(struct cell** livingSpace){
    struct intList* intList = createIntList();
    float maxPressure = livingSpace[0][0].pressure;
    for (int32_t i = 0, index = 0; i < LIVING_SPACE_DIMENSIONS; ++i) {
        for (int32_t j = 0; j < LIVING_SPACE_DIMENSIONS; ++j) {
            if(livingSpace[i][j].pressure > maxPressure){
                maxPressure = livingSpace[i][j].pressure;
                index = 0;
                destroyIntList(intList);
                intList = createIntList();
                insertElementInt(intList, livingSpace[i][j].id, index);
                index++;
            }

            if(livingSpace[i][j].pressure == maxPressure){
                insertElementInt(intList, livingSpace[i][j].id, index);
                index++;
            }
        }
    }

    return intList;
}

struct intList* findHighestSlimeSpots(struct cell** livingSpace){
    struct intList* intList = createIntList();
    float maxPressure = livingSpace[0][0].slimeIntensity;
    for (int32_t i = 0, index = 0; i < LIVING_SPACE_DIMENSIONS; ++i) {
        for (int32_t j = 0; j < LIVING_SPACE_DIMENSIONS; ++j) {
            if(livingSpace[i][j].slimeIntensity > maxPressure){
                maxPressure = livingSpace[i][j].slimeIntensity;
                index = 0;
                destroyIntList(intList);
                intList = createIntList();
                insertElementInt(intList, livingSpace[i][j].id, index);
                index++;
            }

            if(livingSpace[i][j].slimeIntensity == maxPressure){
                insertElementInt(intList, livingSpace[i][j].id, index);
                index++;
            }
        }
    }

    return intList;
}

void equalisePressure(struct cell** livingSpace, int32_t i, int32_t j){
    uint8_t cellExists = checkCellExistence(i, j);
    uint8_t count = 1;
    float pressureSum = livingSpace[i][j].pressure;

    if(cellExists >> 0 & 0b1 && !livingSpace[i - 1][j].wasVisited){
        count++;
        pressureSum += livingSpace[i - 1][j].pressure;
    }
    if(cellExists >> 1 & 0b1 && !livingSpace[i + 1][j].wasVisited){
        count++;
        pressureSum += livingSpace[i + 1][j].pressure;
    }
    if(cellExists >> 2 & 0b1 && !livingSpace[i][j - 1].wasVisited){
        count++;
        pressureSum += livingSpace[i][j - 1].pressure;
    }
    if(cellExists >> 3 & 0b1 && !livingSpace[i][j + 1].wasVisited){
        count++;
        pressureSum += livingSpace[i][j + 1].pressure;
    }

    float newPressure = pressureSum / (float)count;
    livingSpace[i][j].pressure = newPressure;

    if(cellExists >> 0 & 0b1 && !livingSpace[i - 1][j].wasVisited){
        livingSpace[i - 1][j].pressure = newPressure;
    }
    if(cellExists >> 1 & 0b1 && !livingSpace[i + 1][j].wasVisited){
        livingSpace[i + 1][j].pressure = newPressure;
    }
    if(cellExists >> 2 & 0b1 && !livingSpace[i][j - 1].wasVisited){
        livingSpace[i][j - 1].pressure = newPressure;
    }
    if(cellExists >> 3 & 0b1 && !livingSpace[i][j + 1].wasVisited){
        livingSpace[i][j + 1].pressure = newPressure;
    }
}

void feedSlime(struct cell** livingSpace, int32_t i, int32_t j){
    uint8_t cellExists = checkCellExistence(i, j);
    uint8_t count = 1;
    float slimeSum = livingSpace[i][j].slimeIntensity;

    if(cellExists >> 0 & 0b1 && livingSpace[i - 1][j].wasVisited){
        count++;
        slimeSum += livingSpace[i - 1][j].slimeIntensity;
    }
    if(cellExists >> 1 & 0b1 && livingSpace[i + 1][j].wasVisited){
        count++;
        slimeSum += livingSpace[i + 1][j].slimeIntensity;
    }
    if(cellExists >> 2 & 0b1 && livingSpace[i][j - 1].wasVisited){
        count++;
        slimeSum += livingSpace[i][j - 1].slimeIntensity;
    }
    if(cellExists >> 3 & 0b1 && livingSpace[i][j + 1].wasVisited){
        count++;
        slimeSum += livingSpace[i][j + 1].slimeIntensity;
    }

    float newSlime = slimeSum / (float)count;
    livingSpace[i][j].slimeIntensity = newSlime;

    if(cellExists >> 0 & 0b1  && livingSpace[i - 1][j].wasVisited){
        livingSpace[i - 1][j].slimeIntensity = newSlime;
    }
    if(cellExists >> 1 & 0b1  && livingSpace[i + 1][j].wasVisited){
        livingSpace[i + 1][j].slimeIntensity = newSlime;
    }
    if(cellExists >> 2 & 0b1  && livingSpace[i][j - 1].wasVisited){
        livingSpace[i][j - 1].slimeIntensity = newSlime;
    }
    if(cellExists >> 3 & 0b1  && livingSpace[i][j + 1].wasVisited){
        livingSpace[i][j + 1].slimeIntensity = newSlime;
    }
}

void resetWasVisited(struct cell** livingSpace){
    for (int32_t i = 0; i < LIVING_SPACE_DIMENSIONS; ++i) {
        for (int32_t j = 0; j < LIVING_SPACE_DIMENSIONS; ++j) {
            livingSpace[i][j].wasVisited = false;
        }
    }
}

void pressureEqualisationCycle(struct cell** livingSpace){
    struct intList* pressureSpots = findHighestPressureSpots(livingSpace);

    struct intQueue** bfsQueues = malloc(sizeof(struct intQueue*) * pressureSpots->numberOfElements);
    for (int32_t i = 0; i < pressureSpots->numberOfElements; ++i) {
        bfsQueues[i] = createIntQueue();
    }

    int32_t row, column, id;
    uint8_t cellExists;
    // Starting up BFS
    for (int32_t i = 0; i < pressureSpots->numberOfElements; ++i) {
        id = getElementInt(pressureSpots, i);
        row = id / LIVING_SPACE_DIMENSIONS;
        column = id % LIVING_SPACE_DIMENSIONS;

        cellExists = checkCellExistence(row, column);

        livingSpace[row][column].wasVisited = true;

        if(cellExists >> 0 & 0b1){
            enqueueInt(bfsQueues[i], livingSpace[row - 1][column].id);
        }
        if(cellExists >> 1 & 0b1){
            enqueueInt(bfsQueues[i], livingSpace[row + 1][column].id);
        }
        if(cellExists >> 2 & 0b1){
            enqueueInt(bfsQueues[i], livingSpace[row][column - 1].id);
        }
        if(cellExists >> 3 & 0b1){
            enqueueInt(bfsQueues[i], livingSpace[row][column + 1].id);
        }

        equalisePressure(livingSpace, row, column);
    }

    // Executing BFS
    bool loopCondition;
    struct data data;
    do {
        loopCondition = false;
        for (int32_t i = 0; i < pressureSpots->numberOfElements; ++i) {
            loopCondition |= !isIntQueueEmpty(bfsQueues[i]);

            if(!isIntQueueEmpty(bfsQueues[i])){
                data = dequeueInt(bfsQueues[i]);
                if(!data.success){
                    continue;
                }
                id = data.data;
                row = id / LIVING_SPACE_DIMENSIONS;
                column = id % LIVING_SPACE_DIMENSIONS;

                if(livingSpace[row][column].wasVisited){
                    continue;
                }

                cellExists = checkCellExistence(row, column);

                livingSpace[row][column].wasVisited = true;

                if ((cellExists >> 0 & 0b1) && !livingSpace[row - 1][column].wasVisited) {
                    enqueueInt(bfsQueues[i], livingSpace[row - 1][column].id);
                }
                if ((cellExists >> 1 & 0b1) && !livingSpace[row + 1][column].wasVisited) {
                    enqueueInt(bfsQueues[i], livingSpace[row + 1][column].id);
                }
                if ((cellExists >> 2 & 0b1) && !livingSpace[row][column - 1].wasVisited) {
                    enqueueInt(bfsQueues[i], livingSpace[row][column - 1].id);
                }
                if ((cellExists >> 3 & 0b1) && !livingSpace[row][column + 1].wasVisited) {
                    enqueueInt(bfsQueues[i], livingSpace[row][column + 1].id);
                }

                equalisePressure(livingSpace, row, column);
            }
        }
    } while (loopCondition);

    resetWasVisited(livingSpace);

    // Memory cleanup after use
    for (int32_t i = 0; i < pressureSpots->numberOfElements; ++i) {
        destroyIntQueue(bfsQueues[i]);
    }
    free((void*)bfsQueues);
    destroyIntList(pressureSpots);
}

void slimeFeedingCycle(struct cell** livingSpace){
    struct intList* slimeSpots = findHighestPressureSpots(livingSpace);

    struct intQueue** bfsQueues = malloc(sizeof(struct intQueue*) * slimeSpots->numberOfElements);
    for (int32_t i = 0; i < slimeSpots->numberOfElements; ++i) {
        bfsQueues[i] = createIntQueue();
    }

    int32_t row, column, id;
    uint8_t cellExists;
    // Starting up BFS
    for (int32_t i = 0; i < slimeSpots->numberOfElements; ++i) {
        id = getElementInt(slimeSpots, i);
        row = id / LIVING_SPACE_DIMENSIONS;
        column = id % LIVING_SPACE_DIMENSIONS;

        cellExists = checkCellExistence(row, column);

        livingSpace[row][column].wasVisited = true;

        if(cellExists >> 0 & 0b1 && livingSpace[row - 1][column].slimeIntensity > 0.0f){
            enqueueInt(bfsQueues[i], livingSpace[row - 1][column].id);
        }
        if(cellExists >> 1 & 0b1 && livingSpace[row + 1][column].slimeIntensity > 0.0f){
            enqueueInt(bfsQueues[i], livingSpace[row + 1][column].id);
        }
        if(cellExists >> 2 & 0b1 && livingSpace[row][column - 1].slimeIntensity > 0.0f){
            enqueueInt(bfsQueues[i], livingSpace[row][column - 1].id);
        }
        if(cellExists >> 3 & 0b1 && livingSpace[row][column + 1].slimeIntensity > 0.0f){
            enqueueInt(bfsQueues[i], livingSpace[row][column + 1].id);
        }

        feedSlime(livingSpace, row, column);
    }

    // Executing BFS
    bool loopCondition;
    struct data data;
    do {
        loopCondition = false;
        for (int32_t i = 0; i < slimeSpots->numberOfElements; ++i) {
            loopCondition |= !isIntQueueEmpty(bfsQueues[i]);

            if(!isIntQueueEmpty(bfsQueues[i])){
                data = dequeueInt(bfsQueues[i]);
                if(!data.success){
                    continue;
                }
                id = data.data;
                row = id / LIVING_SPACE_DIMENSIONS;
                column = id % LIVING_SPACE_DIMENSIONS;

                if(livingSpace[row][column].wasVisited){
                    continue;
                }

                cellExists = checkCellExistence(row, column);

                livingSpace[row][column].wasVisited = true;

                if ((cellExists >> 0 & 0b1) && !livingSpace[row - 1][column].wasVisited && livingSpace[row - 1][column].slimeIntensity > 0.0f) {
                    enqueueInt(bfsQueues[i], livingSpace[row - 1][column].id);
                }
                if ((cellExists >> 1 & 0b1) && !livingSpace[row + 1][column].wasVisited && livingSpace[row + 1][column].slimeIntensity > 0.0f) {
                    enqueueInt(bfsQueues[i], livingSpace[row + 1][column].id);
                }
                if ((cellExists >> 2 & 0b1) && !livingSpace[row][column - 1].wasVisited && livingSpace[row][column - 1].slimeIntensity > 0.0f) {
                    enqueueInt(bfsQueues[i], livingSpace[row][column - 1].id);
                }
                if ((cellExists >> 3 & 0b1) && !livingSpace[row][column + 1].wasVisited && livingSpace[row][column + 1].slimeIntensity > 0.0f) {
                    enqueueInt(bfsQueues[i], livingSpace[row][column + 1].id);
                }

                feedSlime(livingSpace, row, column);
            }
        }
    } while (loopCondition);

    resetWasVisited(livingSpace);

    // Memory cleanup after use
    for (int32_t i = 0; i < slimeSpots->numberOfElements; ++i) {
        destroyIntQueue(bfsQueues[i]);
    }
    free((void*)bfsQueues);
    destroyIntList(slimeSpots);
}

int main() {
    struct cell** livingSpace = initLivingSpace();

    livingSpace[0][0].pressure = 1.0f;
    livingSpace[0][39].pressure = 1.0f;
    livingSpace[39][0].pressure = 1.0f;
    livingSpace[39][39].pressure = 1.0f;

    printLivingSpacePressure(livingSpace);

    pressureEqualisationCycle(livingSpace);

    printLivingSpacePressure(livingSpace);

    destroyLivingSpace(livingSpace);
    return 0;
}
