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

int isNumberInStr(char str[]);
int isCSV(char *str);

int main(int argc, char **argv){

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
        argCount++;// -l argument check not use area

        if (i == 0){ //omit exicute file name
            continue;
        }

        findTotal++;//count how many argument are checking

        check_ = strncmp(argv[i], "-", 1);
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

                            i++;//// to omit number take as argument

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

                if (totalArgment > 1){//if allowed (-p ,-m ,-t) mulltiple arguments present(any two or up) 
                    if ((count_t >=1 && count_m >=1) || (count_m >=1 && count_p >=1) || (count_p >=1 && count_t >=1)){
                        printf("Cannot plot multiple parameters in same graph.\n");
                        printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n", argv[0]);
                        return 0;
                    }
                }

            }else{//if not allowed argument fined
                printf("Invalid options for [%s]\n", argv[i]);
                printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n", argv[0]);
                return 0;
            }

        }
        
        int checkCsv = isCSV(argv[i]);//check csv file are presnt 
        if (checkCsv==1){

            //csvPtr[findCsv] = argv[i];
            findCsv++;
            //take file data
            //takeFileData(argv[i]);
            
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


    return 0;
} 

int isNumberInStr(char str[]){//given string intergers or not
    for (int i = 0; str[i]!= '\0'; i++){
        if (isdigit(str[i]) == 0)
              return 0;
    }
    return 1;
}

int isCSV(char *str){//given file ending with .csv or not

    if (!strcmp(strrchr(str, '\0') - 4, ".csv")){//The String ends with ".avi"
        return 1;
    }
    return 0;

}
