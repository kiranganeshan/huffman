#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int main(){
    //for compression ratio
    int numchars=0;
    int numbytes=0;
    int freq[1000];
    for(int x=0;x<1000;x++){
        freq[x] = 0;
    }

    FILE* fin = fopen( "decodeME.txt" , "r" );
    char ch = 0;
    //int length = 28;
    
    /* FINDING LENGTH */

    int length = 0;
    int digits[100];
    for(int x=0;x<100;x++){digits[x]=0;}
    fread(&ch,sizeof(char),1,fin);
    int place = 0;

    //reading digits
    while(ch != 10){
        digits[place] = (ch-'0');
        //printf("%d\n",digits[place]);
        fread(&ch,sizeof(char),1,fin);
        place++;
    }

    //reconstructing length
    place = 0;
    while(digits[place] != 0){
        length = 10*length + digits[place];
        place++;
    }


    /*printf("%d",length);
    printf("\n");*/

    /* READING OVERHEAD */

    //instantiating heap
    char tree[10000];
    for(int x=0;x<10000;x++){tree[x] = 0;}
    ch = 0;
    int currindex;
    char lastbyte;

    //filling heap
    for(int x=0;x<length;x++){
        fread(&ch,sizeof(char),1,fin);
        currindex = 1;
        fread(&lastbyte,sizeof(char),1,fin);
        while(lastbyte!=10){
            if(lastbyte==49)
                currindex = 2*currindex + 1;
            if(lastbyte==48)
                currindex = 2*currindex;
            fread(&lastbyte,sizeof(char),1,fin);
        }
        //printf("%c has index %d\n",ch,currindex);
        tree[currindex] = ch;
    }

    /* READING MESSAGE */
    int nextbyteexists = fread(&ch,sizeof(char),1,fin);
    currindex = 1;
    while(nextbyteexists == 1){
        numbytes++;
        if(ch == 49){currindex = 2*currindex + 1;}
        if(ch == 48){currindex = 2*currindex;}
        if(tree[currindex] != 0){
            printf("%c",tree[currindex]);
            freq[(int)tree[currindex]]++;
            currindex = 1;
            numchars++;
        }

        nextbyteexists = fread(&ch,sizeof(char),1,fin);
    }
    printf("\n");
    fclose(fin);

    //calculating compression sizes
    printf("Precompressed Size: %d\n",8*numchars);
    printf("Postcompressed Size: %d\n",numbytes);

    //calculating compression ratio to 2 decimals
    double ratio = 100.0*(8.0*((double)numchars)-((double)numbytes))/(8.0*((double)numchars));
    int roundedRatio = (int)round(100.0*ratio);
    if(roundedRatio-100*(roundedRatio/100)<10)
        printf("Compression Ratio: %d.0%d%%\n",roundedRatio/100,roundedRatio-100*(roundedRatio/100));
    else
        printf("Compression Ratio: %d.%d%%\n",roundedRatio/100,roundedRatio-100*(roundedRatio/100));
    //calculating shannon size limit, rounding up to nearest int
    double shannonArr[1000];
    for(int x=0;x<1000;x++){
        if(freq[x]==0){shannonArr[x]=0.0;}
        else{shannonArr[x] = -((double)freq[x])*log(((double)freq[x])/((double)numchars))/log(2);}
        //printf("%f\n",shannonArr[x]);
    }
    double shannon = 0;
    for(int x=0;x<1000;x++){shannon = shannon + shannonArr[x];}
    printf("Shannon Size Limit: %d\n",(int)(shannon+1));

    //calculating shannon compression limit to 2 decimals
    ratio = 100.0*(8.0*((double)numchars)-shannon)/(8.0*((double)numchars));
    roundedRatio = (int)round(100.0*ratio);
    if(roundedRatio-100*(roundedRatio/100)<10)
        printf("Shannon Compression Limit: %d.0%d%%\n",roundedRatio/100,roundedRatio-100*(roundedRatio/100));
    else
        printf("Shannon Compression Limit: %d.%d%%\n",roundedRatio/100,roundedRatio-100*(roundedRatio/100));
}
