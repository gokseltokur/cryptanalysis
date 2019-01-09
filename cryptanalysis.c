/*


		Göksel Tokur 150116049


*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include<locale.h>

void encrypt(char fileName[], int a, int b);
void decrypt(char fileName[]);
int getLength(char filename[]);
char outputCounter = 1;

int main(){
	int posibilitiesOfA[12] = {1,3,5,7,9,11,15,17,19,21,23,25};
	srand(time(0));
	while(1){	
		int choice;
		printf("\nWhat do you want to do? -Enter 1 for encryption -Enter 2 for decryption\n");
		scanf("%d", &choice);
		if(choice == 1){
			//Random values for a and b
			int randa = rand() % 12; // default 12 / to make easy  1
			int randb = rand() % 26; // default 26 / to make easy  3
			char inputFile[30];
			printf("\nEnter name of input file //Ex: input1.txt\n");
			scanf("%s", inputFile);
			encrypt(inputFile, posibilitiesOfA[randa], randb);
			printf("\n a's random value: %d b's random value: %d \n", posibilitiesOfA[randa], randb);
		}
		else if(choice == 2){
			char inputFile[30];
			printf("\nEnter name of input file //Ex: encryptedoutput1.txt\n");
			scanf("%s", inputFile);
			decrypt(inputFile);
		}
	}
	//decrypt("encryptedoutput1.txt");
	
	return 0;
}

// Encrypt function to encrypt given text file
void encrypt(char fileName[], int a, int b){
	FILE *inputfile;
	FILE *outputfile;
	char c;
	
	if ((inputfile = fopen(fileName, "r")) == NULL) {
		printf("File could not be opened");
		return;
	}
	char outputName[15] = "encryptedoutput";
	char buffer[10];
	itoa(outputCounter, buffer, 10);
	strcat(outputName, buffer);
	strcat(outputName, ".txt");
	if ((outputfile = fopen(outputName, "w")) == NULL ){
    	printf("Error opening file!\n");
    	return;
	}
	outputCounter++;
	
	while(fscanf(inputfile, "%c", &c) != EOF){
		//Encryption for just letters
		if( 65<=(int)c && (int)c<=90 || 97<=(int)c && (int)c<=122 ){
			c = tolower(c);
			c = (char)(97+((( a*((int)c-97) ) + b ) % 26)); // encryption formula.I put 97 to change characters
			fprintf(outputfile, "%c", c);
			//printf("%c ", (char)c);
		}else{
			fprintf(outputfile, "%c", c);
		}
		
	}
	fclose(outputfile);
}

// Decrypt function to decrypted decrypted file
void decrypt(char fileName[]){
	FILE *inputfile;
	FILE *outputfile;
	int totalWord = getLength(fileName);
	//printf("\n***%d****\n", totalWord);
	if ((inputfile = fopen(fileName, "r")) == NULL  ) {
		printf("File error");
		return;
	}
	
	char chArray[26] = {'\0'};
	int countArray[26] = {0};
	char c;
	
	////////////////////////////////////////////////////////////////////////////////
	int posibilitiesOfA[12] = {1,3,5,7,9,11,15,17,19,21,23,25};
	
	int x, b;
	//This loops to try all of possible a and b values 
	for(x=0; x<=11 ; x++){
		int a = posibilitiesOfA[x];
		for(b=0; b<=25; b++){
			if ((outputfile = fopen("decryptedoutput.txt", "w")) == NULL){
				printf("File error2");
				return;
			}
			
			printf("\n                         *********************************************************\n");
			printf("\n                         *********************************************************\n");
			printf("\n                                 	  Values %d - %d is being tried.\n", posibilitiesOfA[x], b);
			printf("\n                         *********************************************************\n");
			printf("\n                         *********************************************************\n");
			
			
			
			while(fscanf(inputfile, "%c", &c) != EOF){
				if( 65<=(int)c && (int)c<=90 || 97<=(int)c && (int)c<=122 ){
					int lockedascii = (int)c;

					int p;
					for(p=0; p<26; p++){
						if( 97+(((a*(p))+b)%26) == lockedascii ){ // i put 97 to change characters
							break;
						}
					}
					p = p+97;
					char unlockedchar = (char)p;
					
					printf("%c", unlockedchar);
					fprintf(outputfile, "%c", unlockedchar);
				}else{
					printf("%c", c);
					fprintf(outputfile, "%c", c);
				}
								
			}
			
			
				/*			
					This block of code compares decrypted text and dictionary. 
					If similarity between decrypted text and dictionary is bigger than %50, function returns.
				*/
				fclose(outputfile);
				FILE *decryptedfile;
				FILE *dictionaryfile;
				if ((decryptedfile = fopen("decryptedoutput.txt", "r")) == NULL){
					printf("File error3");
					return;
				}
				if ((dictionaryfile = fopen("HW4_dictionary.dat", "r")) == NULL){
					printf("File error1");
					return;
				}
				char currentword[25];
				char dicword[25];
				int totalCoupling = 0;
				double percentageOfSimilarity =0;
				while(fscanf(decryptedfile, "%s", currentword) != EOF){
					//printf("Debug");
					while(fscanf(dictionaryfile, "%s", dicword) != EOF){
						//printf("\n %s - %s \n", currentword, dicword );
						if(strcmp(currentword, dicword) == 0){
							totalCoupling++;
							//printf("\n%d", totalCoupling);
						}
						
					}
					fclose(dictionaryfile);
					if ((dictionaryfile = fopen("HW4_dictionary.dat", "r")) == NULL){
						printf("File error4");
						return;
					}
				}
				// if percentage of similarity is bigger than 0.5 current a and b values is correct
				// so stop return there.
				percentageOfSimilarity = (double)totalCoupling / (double)totalWord;
				printf("\n\n************ Total Coupling:%d - Total Words:%d - Percentage Of Similarity:%lf ***************\n\n", totalCoupling, totalWord, percentageOfSimilarity*100);
				if(percentageOfSimilarity > 0.5){
					fclose(outputfile);
					printf("\nDecryption is successfull a: %d b: %d Output file is decryptedoutput.txt\n", posibilitiesOfA[x], b );
					return;
				}
				
				fclose(decryptedfile);
				totalCoupling = 0;
			
			
			if ((inputfile = fopen(fileName, "r")) == NULL){
				printf("File error");
				return;
			}
		}
	}
	
}

//getLength function to get number of words in given file.
int getLength(char filename[]){
	FILE *file;
	int filelength = 0;
	char currentword[25];
	
	if ((file = fopen(filename, "r")) == NULL){
		printf("\nFile error in getLength function");
		return;
	}
	
	while(fscanf(file, "%s", currentword) != EOF){
		filelength++;
	}
	fclose(file);
	
	return filelength;
}

/*


		Göksel Tokur 150116049


*/
