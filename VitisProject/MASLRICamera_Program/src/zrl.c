#include "zrl.h"
#include <stdio.h>
/*
 * Function: zrl
 * ----------------------------
 *   Zero Run Length
 *
 *	 Compresses given block with Zero Run Length algorithm.
 *
 *   returns: int length of compressed block
 */

int zrl(char *block){

    short int i,j=0;
    char output_field[64];
    char counter=0;
    short int flag=0;

    for(i=0;i<64;i++){

        if(block[i]==0){
            flag+=1;
            while(block[i]==0){
                    counter++;
					i++;
                    if(counter==16) {
                        counter=0x7e;
                        break;
                    }
            }

            output_field[j]=0;
            output_field[j+1]=counter;
            j+=2;
            counter=0;
			i--;
        }

        else{
            flag=0;
            output_field[j]=block[i];
            j++;
        }
		block[i]=0;
    }

    for(i=0;i<flag;i++){
        j-=2;
    }

    for(i=0;i<j;i++)
       block[i]=output_field[i];

	block[j]=0x7f;
	j++;
     return sizeof(char)*j;
}
