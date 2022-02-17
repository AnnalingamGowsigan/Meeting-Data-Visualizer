#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define Horizontal  80
#define Participant  1
#define Meeting      2
#define Duration     3
#define SCALED       4
#define NOTSCALED    5



typedef struct tableForMeeting{
    char hostName[100];
    int numberOfMeeting;
    int participantsOfMeeting;
    int duration;
    struct tableForMeeting* next;
} table;

typedef union maxData{
    float participant;
    float meeting;
    float duration;
} max;


void print(table *rowHead);
void insertSort(table **rowHead, int sortType);
void sortedInsert(table **head, table *newNode, int sortType);
table* takeFileData(char fileName[]);

void printBarWithName(char hostName[], int maxChaNamNum, int typeData, int printArea);
void printBarWithOutName(int maxChaNamNum, int printArea);
void printHorizontalLine(int maxChaNamNum);
void printSpace(int maxChaNamNum);
void drawBarChart(table *rowHead, int length, int type, int scaledOrNot);

int NumOfDig(int number);
int maxChaInName(table *rowHead, int length);

max total;

int main() {
    table *rowHead;

    rowHead = takeFileData("1.csv");

    insertSort(&rowHead,Participant);
    //insertSort(&rowHead,Meeting);
    //insertSort(&rowHead,Duration);

    print(rowHead);//for printing

    //drawBarChart(rowHead, 4, Participant, NOTSCALED);

    return 0;
}
table* takeFileData(char fileName[]){
    table *rowHead;
    static table *previousRow = NULL;//for assening privious node address in to current node next

    typedef struct _{
    char hostName[100];
    int participantsOfMeeting;
    int hrs;
    int min;
    int sec;
    } temp;
    temp fileMeetingRow;

    FILE *file;
    file = fopen(fileName, "r");
    if (file != NULL) {
        //printf("File created successfully!\n");
    }else {
        printf("Cannot open one or more given files\n");
        return 0;
    }
    
    int update = 0;
    static int totalMeeting = 0;
    while (fscanf(file, "%[^,],%d,%d:%d:%d\n", fileMeetingRow.hostName, &fileMeetingRow.participantsOfMeeting, &fileMeetingRow.hrs, &fileMeetingRow.min, &fileMeetingRow.sec) != EOF){
        
        //updating data
        if (totalMeeting > 0){
            for (table* currentRow=rowHead; currentRow != NULL; currentRow=currentRow -> next){
            
                if (strcmp(currentRow -> hostName,fileMeetingRow.hostName) == 0){

                    currentRow -> numberOfMeeting = currentRow -> numberOfMeeting +1;
                    currentRow -> participantsOfMeeting = currentRow -> participantsOfMeeting + fileMeetingRow.participantsOfMeeting;
                    currentRow -> duration = currentRow -> duration + fileMeetingRow.hrs*60.0 + fileMeetingRow.min;// + fileMeetingRow.sec/60.0;
                    
                    update = 1;
                    break;
                }
            }
         
        }

        //storing data
        if (update == 0){
            rowHead = (table *)malloc(sizeof(table));

            strcpy(rowHead -> hostName, fileMeetingRow.hostName);
            rowHead -> participantsOfMeeting = fileMeetingRow.participantsOfMeeting;
            rowHead -> duration =  fileMeetingRow.hrs*60.0 + fileMeetingRow.min;// + fileMeetingRow.sec/60.0;
            rowHead -> numberOfMeeting++ ;

            rowHead -> next = previousRow;
            previousRow = rowHead;//store the previous structure address

        }else {
            update = 0;
        }

        totalMeeting++;
    }
    
    fclose(file);

    return rowHead;
}

void sortedInsert(table **rowHead, table *node, int sortType){
    table dummy;
    table *current = &dummy;
    dummy.next = *rowHead;

    switch (sortType){
        case Participant:
            total.participant = total.participant + node ->participantsOfMeeting;

            while (current -> next != NULL && current -> next -> participantsOfMeeting > node->participantsOfMeeting) {
                current = current->next;
            }
            break;

        case Meeting:
            total.meeting = total.meeting + node ->numberOfMeeting;

            while (current -> next != NULL && current -> next -> numberOfMeeting > node->numberOfMeeting) {
                current = current->next;
            }
            break;

        case Duration:
            total.duration = total.duration + node ->duration;

            while (current -> next != NULL && current -> next -> duration > node->duration) {
                current = current->next;
            }
            break;

    }

    node->next = current->next;
    current->next = node;
    *rowHead = dummy.next;

    
}
void insertSort(table **rowHead, int sortType){
    table *result = NULL;    
    table *current = *rowHead;   
    table *next;
 
    while (current != NULL)
    {
        next = current->next;
 
        sortedInsert(&result, current, sortType);
        current = next;
    }
 
    *rowHead = result;
    
}

void drawBarChart(table *rowHead, int length, int drawingType, int scaledOrNot){
    int count = 0;
    int maxNamChaNum = maxChaInName(rowHead, length);
    //printf("%d\n",maxNamChaNum);

    float maxNum;
    int graphBarArea;
    int printArea;

    printf("\n");
    switch (scaledOrNot)
    {
        case NOTSCALED:

            switch (drawingType)
            {

                case Participant:
                    maxNum = rowHead -> participantsOfMeeting;
                    //printf("%f\n",maxNum);
                    graphBarArea = Horizontal - (maxNamChaNum + 2) - 1 - NumOfDig(maxNum);//1 for vertical '|'
                    //printf("graphBarArea%d\n",graphBarArea);

                    for (table* currentRow = rowHead; currentRow != NULL; currentRow = currentRow -> next){

                        if(count == length){
                            break;
                        }

                        printArea = (graphBarArea/total.participant) * (currentRow -> participantsOfMeeting);

                        printBarWithOutName(maxNamChaNum, printArea);
                        printBarWithName(currentRow -> hostName, maxNamChaNum, currentRow -> participantsOfMeeting, printArea);
                        printBarWithOutName(maxNamChaNum, printArea);
                        printSpace(maxNamChaNum);

                        count++;
                        
                    } 
                    break;

                case Meeting:
                    maxNum = (float)rowHead -> numberOfMeeting;
                    //printf("%f\n",maxNum);
                    graphBarArea = Horizontal - (maxNamChaNum + 2) - 1 - NumOfDig(maxNum);//1 for vertical '|'
                    //printf("%d\n",graphBarArea);

                    for (table* currentRow = rowHead; currentRow != NULL; currentRow = currentRow -> next){

                        if(count == length){
                            break;
                        }

                        printArea = (graphBarArea/total.meeting) * (currentRow -> numberOfMeeting);
                        //printf("%d\n",printArea);

                        printBarWithOutName(maxNamChaNum, printArea);
                        printBarWithName(currentRow -> hostName, maxNamChaNum, currentRow -> numberOfMeeting, printArea);
                        printBarWithOutName(maxNamChaNum, printArea);
                        printSpace(maxNamChaNum);
                        
                        count++;

                    } break;

                case Duration:
                    maxNum = (float)rowHead -> duration;
                    //printf("%f\n",maxNum);
                    graphBarArea = Horizontal - (maxNamChaNum + 2) - 1 - NumOfDig(maxNum);//1 for vertical '|'
                    //printf("%d\n",graphBarArea);

                    for (table* currentRow = rowHead; currentRow != NULL; currentRow = currentRow -> next){

                        if(count == length){
                            break;
                        }

                        printArea = (graphBarArea/total.duration) * (currentRow -> duration);
                        //printf("%d\n",printArea);

                        printBarWithOutName(maxNamChaNum, printArea);
                        printBarWithName(currentRow -> hostName, maxNamChaNum, currentRow -> duration, printArea);
                        printBarWithOutName(maxNamChaNum, printArea);
                        printSpace(maxNamChaNum);

                        count++;
                            
                    } break;

            }
            break;

        case SCALED:

            switch (drawingType)
            {

                case Participant:
                    maxNum = (float)rowHead -> participantsOfMeeting;
                    //printf("%f\n",maxNum);
                    graphBarArea = Horizontal - (maxNamChaNum + 2) - 1 - NumOfDig(maxNum);//1 for vertical '|'
                    //printf("%d\n",graphBarArea);

                    for (table* currentRow = rowHead; currentRow != NULL; currentRow = currentRow -> next){

                        if(count == length){
                            break;
                        }

                        printArea = (graphBarArea/maxNum) * (currentRow -> participantsOfMeeting);
                        //printf("%d\n",printArea);

                        printBarWithOutName(maxNamChaNum, printArea);
                        printBarWithName(currentRow -> hostName, maxNamChaNum, currentRow -> participantsOfMeeting, printArea);
                        printBarWithOutName(maxNamChaNum, printArea);
                        printSpace(maxNamChaNum);

                        count++;
                        
                    }
                    break;

                case Meeting:
                    maxNum = (float)rowHead -> numberOfMeeting;
                    //printf("%f\n",maxNum);
                    graphBarArea = Horizontal - (maxNamChaNum + 2) - 1 - NumOfDig(maxNum);//1 for vertical '|'
                    //printf("%d\n",graphBarArea);

                    for (table* currentRow = rowHead; currentRow != NULL; currentRow = currentRow -> next){

                        if(count == length){
                            break;
                        }

                        printArea = (graphBarArea/maxNum) * (currentRow -> numberOfMeeting);
                        //printf("%d\n",printArea);

                        printBarWithOutName(maxNamChaNum, printArea);
                        printBarWithName(currentRow -> hostName, maxNamChaNum, currentRow -> numberOfMeeting, printArea);
                        printBarWithOutName(maxNamChaNum, printArea);
                        printSpace(maxNamChaNum);
                        
                        count++;

                    }
                    break;

                case Duration:
                    maxNum = (float)rowHead -> duration;
                    //printf("%f\n",maxNum);
                    graphBarArea = Horizontal - (maxNamChaNum + 2) - 1 - NumOfDig(maxNum);//1 for vertical '|'
                    //printf("%d\n",graphBarArea);

                    for (table* currentRow = rowHead; currentRow != NULL; currentRow = currentRow -> next){

                        if(count == length){
                            break;
                        }

                        printArea = (graphBarArea/maxNum) * (currentRow -> duration);
                        //printf("%d\n",printArea);

                        printBarWithOutName(maxNamChaNum, printArea);
                        printBarWithName(currentRow -> hostName, maxNamChaNum, currentRow -> duration, printArea);
                        printBarWithOutName(maxNamChaNum, printArea);
                        printSpace(maxNamChaNum);

                        count++;
                            
                    } 
                    break;

            }
            break;

    }
    printHorizontalLine(maxNamChaNum);
}

int maxChaInName(table *rowHead, int length){

    table* currentRow;
    int temp;
    int maxNamChaNum = 0;

    currentRow = rowHead;

    for(int i = 0; i < length; i++){
        temp = strlen(currentRow -> hostName);
        if (temp > maxNamChaNum){
            maxNamChaNum = temp;
        }
        currentRow = currentRow -> next;
    }

    return maxNamChaNum;
}

void printSpace(int maxChaNamNum){
    for (int i =0; i < (maxChaNamNum + 2); i++){printf(" "); }
    printf("\u2502");
    //for (int i =0; i < Horizontal-(maxChaNamNum + 2) -1; i++){printf(" "); }
    printf("\n");

}

void printHorizontalLine(int maxChaNamNum){
    for (int i =0; i < (maxChaNamNum + 2); i++){printf(" "); }
    printf("\u2514");
    for (int i =0; i < Horizontal-(maxChaNamNum + 2) -1; i++){printf("\u2500"); }
    printf("\n");
}

void printBarWithOutName(int maxChaNamNum, int printArea){
    for (int i =0; i < (maxChaNamNum + 2); i++){printf(" "); }
    printf("\u2502");
    for (int i =0; i < printArea; i++){printf("\u2591"); }
    printf("\n");

}

void printBarWithName(char hostName[], int maxChaNamNum, int typeDataNum, int printArea){

    printf(" %s ",hostName);
    for (int i =0; i < maxChaNamNum - strlen(hostName); i++){printf(" "); }

    printf("\u2502");

    for (int i =0; i < printArea; i++){printf("\u2591"); }

    printf("%d", typeDataNum);

    printf("\n");
}

int NumOfDig(int number){
    int count = 0;

    do {
        number /= 10;
        ++count;
    } while (number != 0);

    return count;
}

void print(table *rowHead){

    for (table* currentRow=rowHead; currentRow != NULL; currentRow=currentRow -> next){
            printf("name:- %s\n", currentRow -> hostName);
            printf("participantsOfMeeting:- %d\n", currentRow -> participantsOfMeeting);
            printf("durationOfMeeting: %d\n", currentRow -> duration);
            printf("numberOfMeeting:- %d\n\n", currentRow -> numberOfMeeting);

        }

}
