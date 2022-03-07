#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define Horizontal  80
#define Participant  1
#define Meeting      2
#define Duration     3
#define SCALED       4
#define NOTSCALED    5

typedef struct tableForMeeting{
    char hostName[80];
    int numberOfMeeting;
    int participantsOfMeeting;
    int duration;
    struct tableForMeeting* next;
} table;//creating a structure for storing csv file data

typedef union maxData{
    float participant;
    float meeting;
    float duration;
} max;//storing total count of data type


void insertSort(table **rowHead, int sortType);//given a linklist, change it to be in sorted order (using 'sortedInsert()').
void sortedInsert(table **head, table *newNode, int sortType);//function to insert a given node at its correct sorted position into a given list sorted in decreasing order
table* takeFileData(char fileName[]);

void printBarWithName(char hostName[], int maxChaNamNum, int typeData, int printArea);//print the host name part in the graph 
void printBarWithOutName(int maxChaNamNum, int printArea);//print the without host name part in the graph 
void printHorizontalLine(int maxChaNamNum);//print the graph last horizontal line 
void printSpace(int maxChaNamNum);//print the graph space part
void drawBarChart(table *rowHead, int length, int type, int scaledOrNot);//printing the graph

int NumOfDig(int number);//how many digit has the given number 
int maxChaInName(table *rowHead, int length);//in given lenth data, finding the maximum host name charaters 

int isNumberInStr(char str[]);//given string is interger or not
int isCSV(char *str);//given file ending with .csv or not


max total;//creating max type global varible

int main(int argc, char **argv){
    table *rowHead;//creating a table type variable for linklist head

    int find_ = 0, findCsv = 0, findTotal = 0;
    int find_l = 0, find_p = 0, find_m = 0, find_t = 0, find__scaled = 0;
    int count_l = 0, count_p = 0, count_m = 0, count_t = 0, count__scaled = 0;
    int check_;
    int totalArgment=0;

    int argCount =0;

    int length;
    int scaledOrNot;
    int drawingType;

    if (argc == 1){//if file not given print error massage
        printf("No input files were given\n");
        printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n", argv[0]);

        return 0;
    }


    for (int i = 0; i < argc; i++){//for checking all aruguments
        argCount++;// -l argument check for it is in last

        check_ = strncmp(argv[i], "-", 1);//checking argument start with '-'
        if (check_ == 0){

            find_ ++;//count starting with - argument

            //checking given argument with allowed arguments
            find_l = strcmp(argv[i], "-l");
            find_p = strcmp(argv[i], "-p");
            find_m = strcmp(argv[i], "-m");
            find_t = strcmp(argv[i], "-t");
            find__scaled = strcmp(argv[i], "--scaled");

            if (find_l == 0 || find_p == 0 ||find_m == 0 || find_t == 0 || find__scaled == 0){//if allowed argmnts fined

                if (find__scaled == 0){//checking and adding

                    count__scaled++;

                    scaledOrNot = SCALED;//and fined bar chart parameters

                } else if(find_p == 0){

                    count_p++;
                    totalArgment++;//counting total argment which has (-p,-m,-l) for any two argument are given 

                    drawingType = Participant;

                } else if (find_m == 0){

                    count_m++;
                    totalArgment++;

                    drawingType = Meeting;

                } else if (find_t == 0){

                    count_t++;
                    totalArgment++;

                    drawingType= Duration;

                } else if (find_l == 0){

                    count_l++;

                    if (argc > argCount){// -l argument check not use area

                        if (isNumberInStr(argv[i+1]) == 1){//after -l argments is number or not 
                    
                            length = atoi(argv[i+1]);//convert str to int

                            if (length == 0){//if length 0 close the programme
                                return 0;
                            }

                            //i++;//// to omit number take as argument
                            continue;

                        }else {//after length argument str fined
                            printf("Invalid options for [%s]\n", argv[i]);
                            printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n", argv[0]);
                            return 0;
                        }

                    }else {//-l arugment is last
                        printf("Not enough options for [%s]\n", argv[i]);
                        printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n", argv[0]);
                        return 0;
                    }
                
                }

            }else{//if not allowed argument fined(starig from '-')
                printf("Invalid option [%s]\n", argv[i]);
                printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n", argv[0]);
                return 0;
            }

        }

    }
    if (totalArgment > 1){//if allowed (-p ,-m ,-t) mulltiple arguments present(any two or up) 
        if ((count_t >=1 && count_m >=1) || (count_m >=1 && count_p >=1) || (count_p >=1 && count_t >=1)){
            printf("Cannot plot multiple parameters in same graph.\n");
            printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n", argv[0]);
            return 0;
        }
    }


    find_ = 0;//re initialization to 0 for secound for loop
    for (int i = 0; i < argc; i++){//for checking all aruguments

        if (i == 0){ //omit exicute file name
            continue;
        }

        findTotal++;//count how many argument are checking

        check_ = strncmp(argv[i], "-", 1);
        if (check_ == 0){
            find_ ++;//count starting with - argument

            find_l = strcmp(argv[i], "-l");
            if (find_l == 0){
                i++;//// to omit number take as argument (to omit number take as csv file) 
            }

            continue;             
        }
        
        int checkCsv = isCSV(argv[i]);//check csv file are presnt 
        if (checkCsv==1){

            rowHead = takeFileData(argv[i]); //take file data and store in rowhead

            if (rowHead == NULL){//cannot open given file 
                return 0;
            }
            
            findCsv++;
        }

        if (findTotal > find_ + findCsv){//if we check any other file type are present
            printf("Only .csv files should be given as inputs.\n");
            return 0;

        }

    }

    if (findCsv == 0){//no csv type file found
        printf("No input files were given\n");
        printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n", argv[0]);
        return 0;

    }


    if (count_l == 0){//if -l arugment not found,setting default value
        length = 10;                    
    }

    if (count__scaled == 0 ){//if -l arugment not found, setting default value
        scaledOrNot = NOTSCALED;
    }

    if (totalArgment ==  0){//if -l arugment not found, setting default value
        drawingType = Meeting;
    }

    insertSort(&rowHead, drawingType);//sorting with given drawing type

    drawBarChart(rowHead, length, drawingType, scaledOrNot);//drawing the bar chart

    return 0;
} 

//take the file data
table* takeFileData(char fileName[]){
    table *rowHead;//creating linklist head
    static table *previousRow = NULL;//for assening privious node address in to current node next

    typedef struct _{//creating temp struct for storing csv file row data
    char hostName[80];
    int participantsOfMeeting;
    int hrs;
    int min;
    int sec;
    } temp;
    temp fileMeetingRow;//creating temp variable

    FILE *file;//crating file pointer
    file = fopen(fileName, "r");//open file for reading
    if (file != NULL) {
        //printf("File created successfully!\n");
    }else {
        printf("Cannot open one or more given files\n");//if file was not there
        return NULL;   
    }
    
    int update = 0;//to cheack the getting data from file is stored or update 
    static int totalMeeting = 0;//count the total meeting for update data and static for mullitble csv file data 

    //assign csv file row data in temp struct variable
    while (fscanf(file, "%[^,],%d,%d:%d:%d\n", fileMeetingRow.hostName, &fileMeetingRow.participantsOfMeeting, &fileMeetingRow.hrs, &fileMeetingRow.min, &fileMeetingRow.sec) != EOF){
        
        //updating data
        if (totalMeeting > 0){//if taking data form csv file after 1 row it going to cheak if there any same entry are here
            for (table* currentRow=rowHead; currentRow != NULL; currentRow=currentRow -> next){
            
                if (strcmp(currentRow -> hostName,fileMeetingRow.hostName) == 0){//compare the data is already here

                    //updating
                    currentRow -> numberOfMeeting = currentRow -> numberOfMeeting +1;
                    currentRow -> participantsOfMeeting = currentRow -> participantsOfMeeting + fileMeetingRow.participantsOfMeeting;
                    currentRow -> duration = currentRow -> duration + fileMeetingRow.hrs*60.0 + fileMeetingRow.min;// + fileMeetingRow.sec/60.0;
                    
                    update = 1;//to indicate the data was update
                    break;//then come out the current loop 
                }
            }
         
        }

        //storing data
        if (update == 0){//if taking data is not update store the data
            rowHead = (table *)malloc(sizeof(table));//creating dynamic memery 

            //storing 
            strcpy(rowHead -> hostName, fileMeetingRow.hostName);
            rowHead -> participantsOfMeeting = fileMeetingRow.participantsOfMeeting;
            rowHead -> duration =  fileMeetingRow.hrs*60.0 + fileMeetingRow.min;// + fileMeetingRow.sec/60.0;
            rowHead -> numberOfMeeting++ ;

            rowHead -> next = previousRow;
            previousRow = rowHead;//store the previous structure address

        }else {
            update = 0;//reset the update value for next row data
        }

        totalMeeting++;//counting total data
    }
    
    fclose(file);//close the file

    return rowHead;//return the linklist head
}

//function to insert a given node at its correct sorted position into a given list sorted in decreasing order
void sortedInsert(table **rowHead, table *node, int sortType){
    table dummy;//creatr dummy node 
    table *current = &dummy;
    dummy.next = *rowHead;

    switch (sortType){//sorted in given sort type
        case Participant:
            total.participant = total.participant + node ->participantsOfMeeting;//find toatal participant

            while (current -> next != NULL && current -> next -> participantsOfMeeting > node->participantsOfMeeting) {//go to the correct position
                current = current->next;
            }
            break;

        case Meeting:
            total.meeting = total.meeting + node ->numberOfMeeting;//find total Meeting

            while (current -> next != NULL && current -> next -> numberOfMeeting > node->numberOfMeeting) {//go to the correct position
                current = current->next;
            }
            break;

        case Duration:
            total.duration = total.duration + node ->duration;//find total Duration

            while (current -> next != NULL && current -> next -> duration > node->duration) {//go to the correct position
                current = current->next;
            }
            break;

    }
    //insert the given node
    node->next = current->next;
    current->next = node;
    *rowHead = dummy.next;

    
}

//given a linklist, change it to be in sorted order (using 'sortedInsert()').
void insertSort(table **rowHead, int sortType){
    table *result = NULL;//create temporary head of linklist variable
    table *current = *rowHead;//iterate over the original list
    table *next;
 
    while (current != NULL)
    {
        next = current->next;//note the next pointer before we change it
        
        sortedInsert(&result, current, sortType);//for sorted
        current = next;
    }
 
    *rowHead = result;//give the linklist head to orignal head of linklist
    
}



//printing the graph
void drawBarChart(table *rowHead, int length, int drawingType, int scaledOrNot){
    int count = 0;
    int maxNamChaNum = maxChaInName(rowHead, length);//in given length data, finding the maximum host name charaters 

    float maxNum;//store maximum given type(-p,-t,-m) data

    int graphBarArea;// total available printing area
    int printArea;//printing for "░"

    printf("\n");
    switch (scaledOrNot)//printing graph is scaled or not scaled
    {
        case NOTSCALED://printing graph is not scaled

            switch (drawingType)//the graph drawing type is -m or -p or -t
            {

                case Participant://if -p
                    maxNum = rowHead -> participantsOfMeeting;//assign total participant into maxNum (float) variable 
                    
                    //finding graph area for printing "░"
                    graphBarArea = Horizontal - (maxNamChaNum + 2) - 1 - NumOfDig(maxNum);//1 for vertical '|'

                    for (table* currentRow = rowHead; currentRow != NULL; currentRow = currentRow -> next){

                        if(count == length){//printing graph in given length
                            break;
                        }

                        //finding print "░" area 
                        printArea = (graphBarArea/total.participant) * (currentRow -> participantsOfMeeting);//finding the ratio of print "░"

                        printBarWithOutName(maxNamChaNum, printArea);
                        printBarWithName(currentRow -> hostName, maxNamChaNum, currentRow -> participantsOfMeeting, printArea);
                        printBarWithOutName(maxNamChaNum, printArea);
                        printSpace(maxNamChaNum);

                        count++;
                        
                    } 
                    break;

                case Meeting://if -m
                    maxNum = (float)rowHead -> numberOfMeeting;//assign total numberOfMeeting into maxNum (float) variable 
                    
                    //finding graph area for printing "░"
                    graphBarArea = Horizontal - (maxNamChaNum + 2) - 1 - NumOfDig(maxNum);//1 for vertical '|'

                    for (table* currentRow = rowHead; currentRow != NULL; currentRow = currentRow -> next){

                        if(count == length){//printing graph in given length
                            break;
                        }

                        //finding print "░" area 
                        printArea = (graphBarArea/total.meeting) * (currentRow -> numberOfMeeting);//finding the ratio of print "░"

                        printBarWithOutName(maxNamChaNum, printArea);
                        printBarWithName(currentRow -> hostName, maxNamChaNum, currentRow -> numberOfMeeting, printArea);
                        printBarWithOutName(maxNamChaNum, printArea);
                        printSpace(maxNamChaNum);
                        
                        count++;

                    } break;

                case Duration://if -t
                    maxNum = (float)rowHead -> duration;//assign total duration into maxNum (float) variable 

                    //finding graph area for printing "░"
                    graphBarArea = Horizontal - (maxNamChaNum + 2) - 1 - NumOfDig(maxNum);//1 for vertical '|'

                    for (table* currentRow = rowHead; currentRow != NULL; currentRow = currentRow -> next){

                        if(count == length){//printing graph in given length
                            break;
                        }

                        //finding print "░" area 
                        printArea = (graphBarArea/total.duration) * (currentRow -> duration);//finding the ratio of print "░"
                        //printf("%d\n",printArea);

                        printBarWithOutName(maxNamChaNum, printArea);
                        printBarWithName(currentRow -> hostName, maxNamChaNum, currentRow -> duration, printArea);
                        printBarWithOutName(maxNamChaNum, printArea);
                        printSpace(maxNamChaNum);

                        count++;
                            
                    } break;

            }
            break;//break the NOTSCALED case

        case SCALED://printing graph is scaled

            switch (drawingType)//the graph drawing type is -m or -p or -t
            {

                case Participant://if -p
                    maxNum = (float)rowHead -> participantsOfMeeting;//assign total participant into maxNum (float) variable 
                    
                    //finding graph area for printing "░"
                    graphBarArea = Horizontal - (maxNamChaNum + 2) - 1 - NumOfDig(maxNum);//1 for vertical '|'

                    for (table* currentRow = rowHead; currentRow != NULL; currentRow = currentRow -> next){

                        if(count == length){//printing graph in given length
                            break;
                        }

                        //finding print "░" area                         
                        printArea = (graphBarArea/maxNum) * (currentRow -> participantsOfMeeting);//finding the ratio of print "░"

                        printBarWithOutName(maxNamChaNum, printArea);
                        printBarWithName(currentRow -> hostName, maxNamChaNum, currentRow -> participantsOfMeeting, printArea);
                        printBarWithOutName(maxNamChaNum, printArea);
                        printSpace(maxNamChaNum);

                        count++;
                        
                    }
                    break;

                case Meeting://if -m
                    maxNum = (float)rowHead -> numberOfMeeting;//assign total participant into maxNum (float) variable 

                    //finding graph area for printing "░"
                    graphBarArea = Horizontal - (maxNamChaNum + 2) - 1 - NumOfDig(maxNum);//1 for vertical '|'

                    for (table* currentRow = rowHead; currentRow != NULL; currentRow = currentRow -> next){

                        if(count == length){//printing graph in given length
                            break;
                        }

                        //finding print "░" area
                        printArea = (graphBarArea/maxNum) * (currentRow -> numberOfMeeting);//finding the ratio of print "░"

                        printBarWithOutName(maxNamChaNum, printArea);
                        printBarWithName(currentRow -> hostName, maxNamChaNum, currentRow -> numberOfMeeting, printArea);
                        printBarWithOutName(maxNamChaNum, printArea);
                        printSpace(maxNamChaNum);
                        
                        count++;

                    }
                    break;

                case Duration://if -t
                    maxNum = (float)rowHead -> duration;//assign total duration into maxNum (float) variable

                    //finding graph area for printing "░"
                    graphBarArea = Horizontal - (maxNamChaNum + 2) - 1 - NumOfDig(maxNum);//1 for vertical '|'

                    for (table* currentRow = rowHead; currentRow != NULL; currentRow = currentRow -> next){

                        if(count == length){//printing graph in given length
                            break;
                        }

                        //finding print "░" area
                        printArea = (graphBarArea/maxNum) * (currentRow -> duration);//finding the ratio of print "░"

                        printBarWithOutName(maxNamChaNum, printArea);
                        printBarWithName(currentRow -> hostName, maxNamChaNum, currentRow -> duration, printArea);
                        printBarWithOutName(maxNamChaNum, printArea);
                        printSpace(maxNamChaNum);

                        count++;
                            
                    } 
                    break;

            }
            break;//break the NOTSCALED case

    }
    printHorizontalLine(maxNamChaNum);//print the graph last horizontal line

}



//in given length data, finding the maximum host name charaters  
int maxChaInName(table *rowHead, int length){

    int temp;
    int maxNamChaNum = 0;//initalize maximum charaters of host

    int count = 0;
    for (table* currentRow = rowHead; currentRow != NULL; currentRow = currentRow -> next){

        if(count == length){//printing graph in given length
            break;
        }

        temp = strlen(currentRow -> hostName);
        if (temp > maxNamChaNum){
            maxNamChaNum = temp;//store the maximum host name charaters
        }

        count++;
    }

    return maxNamChaNum;//return the maximum host name charaters
}

//how many digit has the given number 
int NumOfDig(int number){
    int count = 0;

    do {
        number /= 10;
        ++count;
    } while (number != 0);

    return count;
}

//given string is interger or not
int isNumberInStr(char str[]){
    for (int i = 0; str[i]!= '\0'; i++){
        if (isdigit(str[i]) == 0)//checking each charater is interger or not
              return 0;
    }
    return 1;//given string is digit return 1
}

//given file ending with .csv or not
int isCSV(char *str){

    if (!strcmp(strrchr(str, '\0') - 4, ".csv")){//The String ends with ".csv"
        return 1;
    }
    return 0;
}



//printing the graph functions
//print the graph space part
void printSpace(int maxChaNamNum){

    for (int i =0; i < (maxChaNamNum + 2); i++){printf(" "); }//print space
    printf("\u2502");// print the "|"
    //for (int i =0; i < Horizontal-(maxChaNamNum + 2) -1; i++){printf(" "); }
    printf("\n");

}

//print the graph last horizontal line 
void printHorizontalLine(int maxChaNamNum){

    for (int i =0; i < (maxChaNamNum + 2); i++){printf(" "); }//print space
    printf("\u2514");// print the "└"
    for (int i =0; i < Horizontal-(maxChaNamNum + 2) -1; i++){printf("\u2500"); }// print the "-"
    printf("\n");

}

//print the without host name part in the graph 
void printBarWithOutName(int maxChaNamNum, int printArea){

    for (int i =0; i < (maxChaNamNum + 2); i++){printf(" "); }// print space
    printf("\u2502");// print the "|"
    for (int i =0; i < printArea; i++){printf("\u2591"); }// print the "░"
    printf("\n");

}

//print the host name part in the graph 
void printBarWithName(char hostName[], int maxChaNamNum, int typeDataNum, int printArea){

    printf(" %s ",hostName);//print the host name
    for (int i =0; i < maxChaNamNum - strlen(hostName); i++){printf(" "); }// print space

    printf("\u2502");// print the "|"

    for (int i =0; i < printArea; i++){printf("\u2591"); }// print the "░"

    printf("%d", typeDataNum);//print the digit

    printf("\n");

}


