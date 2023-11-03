#define _CRT_SECURE_NO_WARNINGS
// Noam Klainer ID 316015411
// Roee ShaGmoon ID 206564759

#include<stdio.h>
#include<string.h>
#include <stdlib.h>


typedef struct serie {
    char serieName[20];
    int numSeasons;
    int rank;
    int ID;
    int* watchingDetails;
    char active;
} serie;

int showMenu(int* p) {
    int num = 0, flag = 0;
    char temp[20];
    printf("**********Main Menu **************\n1. Unify Database.\n2. Get Unify Database Details.\n3. Print The Series List Sorted By Code.\n4. Get Watching Details.\n5. Show All Watching Details.\n6. Print Series Sorted By Rank.\n7. Make Series File Sorted By Rank.\n8. Exit.\nEnter Your Selection:");
    do {
        fgets(temp, 20, stdin);
        sscanf(temp, "%d", &num);
        if (!(num >= 1 && num <= 8)) {
            printf("Wromg input please enter a number between 1 to 8!!!\n" );
        }
        if (num >= 1 && num <= 8) {
            flag = 1;
        }
    }
    while (!flag);
    *p = num;
    return 1;
}
int binary_search(serie** arr, int sizeOfArr, int key)
{
    int start = 0, middle, end = sizeOfArr - 1;
    for (int k = 0; k < sizeOfArr;k++)
        arr[k];
    while (start <= end)
    {
        middle = (start + end) / 2;
        if (arr[middle]->ID == key)
        {
            return middle;
        }
        else if (arr[middle]->ID < key)
        {
            start = middle + 1;
        }
        else
        {
            end = middle - 1;
        }
    }
    return -1;
}

int unifyDatabase(FILE* f, FILE* g,int* p) {
    char active, ch1, ch2;
    serie serie1, serie2;       //constructor
    FILE* u;
    u = fopen("series.txt", "w");

    do {
        fscanf(f, "%[^,]%*c%3d%*c%2d%*c%[^\n]", serie1.serieName, &serie1.ID, &serie1.numSeasons,
            &serie1.active);//scanigng first seirie from the first file
        fgetc(f);
    } while (serie1.active == 'N');//if  the serie not active take the next one until we take active serie
    if (!serie1.active) {
        serie1.ID = 1000;//if the file end ID = infinty(1000 is like infinty hgere) to continue only with the other file
    }

    do {
        fscanf(g, "%[^,]%*c%3d%*c%2d%*c%c", serie2.serieName, &serie2.ID, &serie2.numSeasons,
            &serie2.active);//scanigng first seirie from the second file
        fgetc(g);
    } while (serie2.active == 'N');//if  the serie not active take the next one until we take active serie
    if (!serie2.active) {
        serie2.ID = 1000;//if the file end ID = infinty(1000 is like infinty hgere) to continue only with the other file

    }

    while (serie1.active || serie2.active) {//while i have at least one active serie to check from some of the files


        if (serie1.ID > serie2.ID) {//checking wich ID is bigger for the sort

            fprintf(u, "%s%c%d%c%d\n", serie2.serieName, ',', serie2.ID, ',',
                serie2.numSeasons);//scaning into the new file
                (*p)++;
            do {//if i scaning from the first file i need to find the next active serie in this file
                fscanf(g, "%[^,]%*c%3d%*c%2d%*c%c", serie2.serieName, &serie2.ID, &serie2.numSeasons, &serie2.active);
                fgetc(g);
            } while (serie2.active != 'Y' && !(feof(g)));//checking if the serie active or i got to end of file
            if (feof(g)) {
                serie2.ID = 1000;
            }

        }
        else if (serie2.ID > serie1.ID) {//checking wich ID is bigger for the sort

            fprintf(u, "%s%c%d%c%d\n", serie1.serieName, ',', serie1.ID, ',', serie1.numSeasons);
            (*p)++;
            do {//if i scaning from the second file i need to find the next active serie in this file
                fscanf(f, "%[^,]%*c%3d%*c%2d%*c%c", serie1.serieName, &serie1.ID, &serie1.numSeasons, &serie1.active);
                fgetc(f);

            } while (serie1.active != 'Y' && (!feof(f)));//checking if the serie active or i got to end of file
            if (feof(f)) {
                serie1.ID = 1000;
            }
        }
        if (serie1.ID == serie2.ID) {//if i have the same seirie in both files
            if (serie1.ID == 1000) {//if both files end break from the loop because i finish
                break;
            }
            if (serie1.numSeasons > serie2.numSeasons) {//checking number of serie
                fprintf(u, "%s%c%d%c%d\n", serie1.serieName, ',', serie1.ID, ',', serie1.numSeasons);
                (*p)++;

            }

            if (serie1.numSeasons < serie2.numSeasons) {
                fprintf(u, "%s%c%d%c%d\n", serie2.serieName, ',', serie2.ID, ',', serie2.numSeasons);
                (*p)++;

            }

            if (serie1.numSeasons == serie2.numSeasons) {

                fprintf(u, "%s%c%d%c%d\n", serie2.serieName, ',', serie2.ID, ',', serie2.numSeasons);
                (*p)++;
            }
            do {//because the serie is the same in both files i need to find the next serie in both files
                fscanf(g, "%[^,]%*c%3d%*c%2d%*c%c", serie2.serieName, &serie2.ID, &serie2.numSeasons, &serie2.active);
                fgetc(g);
            } while ((serie2.active != 'Y') && (!feof(g)));
            if (feof(g)) {
                serie2.ID = 1000;
            }
            do {
                fscanf(f, "%[^,]%*c%3d%*c%2d%*c%c", serie1.serieName, &serie1.ID, &serie1.numSeasons, &serie1.active);
                fgetc(f);

            } while (serie1.active != 'Y' && (!feof(f)));
            if (feof(f)) {
                serie1.ID = 1000;
            }
        }

    }
    fclose(u);
    fclose(f);
    fclose(g);
    return 1;

}



serie** getUnifyDatabaseDetails(FILE* fp, FILE* watch, int number_series) {
    fclose(fopen("waching.txt", "w"));
    char ch;
    int i = 0;

    serie** S;  //pointer to series Array

    S = (serie**)malloc(number_series * sizeof(serie*));
       
    
    if (S == NULL) {
        printf("Memory allocation failed, quiting... ");
        exit(0);
    }

    while (!feof(fp)) {
        S[i] = (serie*)malloc(sizeof(serie)); // pointer for serie
        fscanf(fp, "%[^,]%*c %3d%*c %2d%*c", S[i]->serieName, &S[i]->ID, &S[i]->numSeasons);
        S[i]->rank = 0;

        if (i < number_series) {
            S[i]->watchingDetails = (int*)malloc(sizeof(int) * S[i]->numSeasons);
            if (S[i]->watchingDetails == NULL) {
                printf("Memory allocation failed, quiting... ");
                exit(0);
            }

            for (int k = 0;k < S[i]->numSeasons;k++) {
                S[i]->watchingDetails[k] = 0;
            }
        }
        i++;

    }

    return S;

}

void printSeriesList(serie** seriesData, int number_series) {
    for (int i = 0; i < number_series; i++) {
        printf("%03d-%s(%d)\n", seriesData[i]->ID, seriesData[i]->serieName, seriesData[i]->numSeasons);
    }
    printf("\n");
}


int getSerieCode(serie** seriesData, int* rangeSeason, int number_series) {
    char temp[20];
    int SerieCode = 0;             //after debug change for 0
    int flagserch = 0;
   
    int i;
    fgets(temp, 20, stdin);
    sscanf(temp, "%d", &SerieCode);
    int code;
    for (i = 0; i < number_series; i++) {
        code = seriesData[i]->ID;
        if (code == SerieCode) {
            *rangeSeason = seriesData[i]->numSeasons;
            flagserch = 1;
            break;
        }
    }
    if (!flagserch) {
        return 0;              // will print "worng Serie Code, try again!!!" next state
    }
    return SerieCode;
}

void getWatchingDetails(serie** seriesData, int num) {
    FILE* watch;
    char temp[20];
    int SeasonNumber ,flagseson = 0, flagSerieCode = 0,numseries,indx, rangeSeason;   //the flag get the Serie Code only if find him
    watch = fopen("waching.txt", "a");//making new file waching.txt

    printf("Insert Serie Code:");
    do{
    flagSerieCode = getSerieCode(seriesData, &rangeSeason, num);
    if (flagSerieCode == 0) {
        printf("wrong serie code, try again!!!\n");
        
    }
    } 
    while (!flagSerieCode);
    printf("Insert Season Number (1 - %d):", rangeSeason);
    do {
        fgets(temp, 20, stdin);
        sscanf(temp, "%d", &SeasonNumber);
        if (1 <= SeasonNumber && SeasonNumber <= rangeSeason) {
            printf("series: %d, season: %d accepted!!!\n", flagSerieCode, SeasonNumber);
            fprintf(watch, "%03d,%d\n", flagSerieCode, SeasonNumber);
            indx = binary_search(seriesData, num, flagSerieCode);
            seriesData[indx]->rank++;
            seriesData[indx]->watchingDetails[SeasonNumber-1]++;
            fclose(watch);
            flagseson = 1;
        }
        else {
            printf("wrong season number, try again!!!\n");
        }
    }
    while (!flagseson);
}

void printWatchingDetails() {
    FILE* watch;
    int SerieCode = 0;
    int SeasonNumber = 0;

    watch = fopen("waching.txt", "r");
    printf("Watching Details\n");
    while (fscanf(watch, "%3d%*c %d", &SerieCode, &SeasonNumber) != -1) {//fscanf return EOF = -1
        printf("Series Code:%03d, Season Number:%d\n", SerieCode, SeasonNumber);
    }
    fclose(watch);
}
void printSeriesByRank(serie** list, int num) {
    int len = num, var = -1;
    serie* temp;
   
    
    
    for (int i = 0; i < len-1; i++) {
        for (int j = 0; j < len-1-i; j++) {
            if (list[j]->rank > (list[j+1])->rank) {
                
               
                 temp = list[j];
                 list[j] = list[j + 1];
                 list[j + 1] = temp;
            
              
            }
            if (list[j]->rank == list[j + 1]->rank && list[j]->ID < list[j + 1]->ID) {
                temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
        }
    }
    for (int i = num-1; i >= 0; i--) {
        if (list[i]->rank < var || var == -1) {
            printf("RANK %d\n", list[i]->rank);
            var = list[i]->rank;
            }
        
       
       
        printf("%03d-%s\n", list[i]->ID, list[i]->serieName);
        for (int k = 0; k < (list[i]->numSeasons);++k) {
            printf("Season %d:%d\n", k+1, list[i]->watchingDetails[k]);
        }
        if (i > 0) {
            while (list[i - 1]->rank != var) {
                printf("RANK %d\n", --var);
            }
        }
    }
    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - 1 - i; j++) {
            if (list[j]->ID > (list[j + 1])->ID) {


                temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;


            }

        }
    }

}
void makeSeriesFileSortedByRank(serie** list, int len) {
    FILE* fp;
    serie* temp;
    fp = fopen("seriesRank.txt", "w");
    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - 1 - i; j++) {
            if (list[j]->rank >= list[j + 1]->rank) {
                temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
            if (list[j]->rank == list[j + 1]->rank && list[j]->ID < list[j + 1]->ID) {
                temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
        }
      
    }
    
        for (int i = len - 1; i >= 0; i--) {
            fprintf(fp, "%03d(Rank:%d)-%s\n", list[i]->ID, list[i]->rank, list[i]->serieName);
            
        }
            
   
    fclose(fp);
    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - 1 - i; j++) {
            if (list[j]->ID > (list[j + 1])->ID) {


                temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;


            }

        }
    }
}

int main() {
    char filename1[20], filename2[20], temp[21];
    serie** seriesData = 0;
    FILE* fp, * fg, * series, * watch;
    int* p;
    int flag1 = 0, flag2 = 0;
    int chose;
    int number_series = 0;
   
    do {
        showMenu(&chose);//printing menue return user chosen
        if (chose == 1 && flag1 == 1) {

            printf("Unify Database Has Been Done Already !!!\n");
        }
        if (chose == 1&& flag1==0) {//if input is 1
            
            flag1 = 1;//flag to know that option 1 happend for the other oprions
            flag2 = 0;
            do {
                printf("Enter First File Name:\n");
                fgets(temp, 20, stdin);//turn on when not debug (not in comment!!) //eazy to get strong from user with fgets puting this in temp var
                sscanf(temp, "%s", filename1);//turn on when not debug (not in comment!!) //cleaning the \n from fgets
                fp = fopen(filename1, "r");   //when not debug change s.txt to filename1

                if ((!fp)) {
                    printf("error in opening file %s !!!\n", filename1);//cheking if file exist
                }
            } while (!fp);//if name file is invaild do all this again

            do {
                printf("Enter Second File Name:\n");
         
               fgets(temp, 20, stdin);                                           
               sscanf(temp, "%s", filename2);//the user chose second file          
                fg = fopen(filename2, "r");                             //when not debug change n.txt to filename2
                if ((!fg)) {
                    printf("error in opening file %s !!!\n", filename2);//cheking if file existe
                }
            } 
            while (!fg);//if name file is invaild do all this again
            p = &number_series;
            unifyDatabase(fp, fg, p);
            


            printf("Unify Succeeded\n");
            
             
        }
       

        if (chose == 2) {//if user input 2
            if (flag1 == 1) {
                series = fopen("series.txt", "r");//file from option 1
                
               
                watch = fopen("waching.txt", "w");//making new file waching.txt
                seriesData = getUnifyDatabaseDetails(series, watch, number_series);
                printf("Get Unify Database Details Succeeded\n");
                flag2 = 1;//flag to know that option 2 happend
                fclose(series);
                fclose(watch);
               
            }
            else {
                printf("You must unify database before using this option!!!\n");//if option 1 didnt happend
              
            }
        }
        if (chose == 3) {//if user input 3
            if (flag1 && flag2) {
                
               
                printf("Series List\n");
                printSeriesList(seriesData, number_series);
               
            }


            if (!flag1) {//if option 1 didn't happend
                printf("You must unify database before using this option!!!\n");
              

            }
            if ((!flag2) && flag1) {//if option 1 happend but 2 didnt
                printf("You must Get Unify Database Details before using this option!!!\n");
               
            }
        }
        if (chose == 4) { //Get Watching Details
            if (flag1 && flag2) {//if option 1 and 2 happend already
               


                getWatchingDetails(seriesData, number_series);





               
            }
        

            if (!flag1) {//if option 1 didn't happend
                printf("You must unify database before using this option!!!\n");
             

            }
            if ((!flag2) && flag1) {//if option 1 happend but 2 didn't
                printf("You must Get Unify Database Details before using this option!!!\n");
               
            }
        }
        if (chose == 5) { //Show All Watching Details.
            if (flag1 && flag2) {//if option 1 and 2 happend already
                printWatchingDetails();

              
            }

            if (!flag1) {//if option 1 didn't happend
                printf("You must unify database before using this option!!!\n");
              

            }
            if ((!flag2) && flag1) {//if option 1 happend but 2 didn't
                printf("You must Get Unify Database Details before using this option!!!\n");
               
            }
        }
        if (chose == 6) {
            if (flag1 && flag2) {
               
                printSeriesByRank(seriesData, number_series);
              
            }
            if (!flag1) {//if option 1 didn't happend
                printf("You must unify database before using this option!!!\n");
               
            }
            if ((!flag2) && flag1) {//if option 1 happend but 2 didn't
                printf("You must Get Unify Database Details before using this option!!!\n");
                
            }
            

        }
        if (chose == 7) {
            if (flag1 && flag2) {
              
                makeSeriesFileSortedByRank(seriesData, number_series);
                
            }

            if (!flag1) {//if option 1 didn't happend
                printf("You must unify database before using this option!!!\n");
              
            }
            if ((!flag2) && flag1) {//if option 1 happend but 2 didn't
                printf("You must Get Unify Database Details before using this option!!!\n");
                
            }

        }

            


    } 
    while (chose != 8);//while the input 1�<=�chose�<=�7
    
    if (flag1 && flag2) {
        
        for (int i = 0; i < number_series; i++) {
          

            free(seriesData[i]->watchingDetails);
        }
        for (int i = 0; i < number_series; i++) {

            free(seriesData[i]);
            }
       
        
    }
   
    printf("End Of The Program...\n");
}
