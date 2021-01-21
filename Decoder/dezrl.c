#include "dezrl.h"
#include <stdio.h>
/*
 * Function: dezrl
 * ----------------------------
 *   Decompress Zero Run Length
 *
 *	 Decompresses given block compressed with Zero Run Length algorithm.
 *
 *   returns: nothing 
 */

void dezrl(char *block){
	short int i=0,j=0,br_nula;
	char output_field[64];
	while(block[i]!=0x7f){
		if(block[i]==0){
			if(block[i+1]==0x7e){
				br_nula=16;
			}
			
			else br_nula=block[i+1];
			for(;br_nula!=0;br_nula--){
				output_field[j]=0;
				j++;
			}
			
		}
		else{
			if(block[i-1]!=0 || i==0){
				output_field[j]=block[i];
				j++;
			}

		}
		i++;
	}
	for(i=j;i<64;i++){
		output_field[i]=0;
	}
	for(i=0;i<64;i++){
		block[i]=output_field[i];
	}
}