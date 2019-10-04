//Kiran Ganeshan, 9/28/17, Period 1

#include <stdio.h>
#include <stdlib.h>
typedef struct TreeNode {
	char symbol;
	int frequency;
	struct TreeNode* left;
	struct TreeNode* right;
}TreeNode;
void printOverhead(TreeNode* head, char* a, int depth, char m[256][10]);
int main(){
	
	/*
	Data:
		int message[]: 		holds message chars
		int rawFreq[]: 		holds frequencies including 0s
		int freq[]: 		holds raw frequences
		int messageLength: 	holds length of message
		int numCharFreqs: 	holds number of distinct chars
		Node* treeArray[]:	holds Nodes to be assembled into tree
	*/

	//Read freqs and message from file

	FILE * fin = fopen("encodeME.txt","r");
	char message[100000] = {0};
	int rawFreq[256] = {0};
	int messageLength = 0;
	char ch;
	int numbytes = fread(&ch,sizeof(char),1,fin);
	while(numbytes == 1){
		message[messageLength] = ch;
		rawFreq[ch]++;
		messageLength++;
		numbytes = fread(&ch,sizeof(char),1,fin);
	}
	//Print Raw Frequencies
	/*
	printf("\nPrint Raw Frequencies:\n");
	for(int x=0;x<256;x++)
		printf("%c: %d\n",(char)x,rawFreq[x]);
	*/

	//Find Number of Different Chars
	int numCharFreqs = 0;
	for(int x=0;x<256;x++){
		if(rawFreq[x]>0){
			numCharFreqs = numCharFreqs + 1;
		}
	}

	//Print Number of Different Chars
	printf("%d\n",numCharFreqs);

	//Fill treeArray[]
	struct TreeNode* treeArray[numCharFreqs];
	int y=0;
	for(int x=0;x<256;x++){
		if(rawFreq[x]>0){
			treeArray[y] = (TreeNode*)malloc(sizeof(TreeNode));
			treeArray[y] -> symbol = (char)(x);
			treeArray[y] -> frequency = rawFreq[x];
			y++;
		}
	}

	//Print Pre-sort Node Array
	/*
	printf("\nPre-sort:\n");
	for(int x=0;x<numCharFreqs;x++){
		printf("%c, %d\n",treeArray[x] -> symbol, treeArray[x] -> frequency);
	}
	*/
	TreeNode* temp = (TreeNode*)malloc(sizeof(TreeNode));
	TreeNode* parent;
	//Sort treeArray[]
	for(int x=0;x<numCharFreqs-1;x++){
		//printf("\nCreated TreeNode\n");
    	for (int i = 1; i < numCharFreqs-x; i++) {
    		int j=i;
        	while (treeArray[j] -> frequency > treeArray[j - 1] -> frequency) {
            	temp = treeArray[j];
            	treeArray[j] = treeArray[j - 1];
            	treeArray[j - 1] = temp;
            	j--;
            	if(j==0)
            		break;
        	}
    	}
    	//printf("\nSorted Array\n");
    	parent = (TreeNode*)malloc(sizeof(TreeNode)); 
    	parent -> left = treeArray[numCharFreqs-x-2];
    	parent -> right = treeArray[numCharFreqs-x-1];
    	parent -> symbol = 0;
    	parent -> frequency = ((parent -> left) -> frequency)+((parent -> right) -> frequency);
    	treeArray[numCharFreqs-x-1] = NULL;
    	treeArray[numCharFreqs-x-2] = parent;
    	/*
    	printf("\nIteration %d:\n",x+1);
		for(int k=0;k<numCharFreqs-x-1;k++){
			printf("%c, %d\n",treeArray[k] -> symbol, treeArray[k] -> frequency);
		}
		printf("\n");
		*/
	}
	char bytes[9];
	for(int x=0;x<9;x++)
		bytes[x] = '\0';
	TreeNode* head = treeArray[0];
	char key[256][10];
	for(int i=0;i<256;i++)
		for(int j=0;j<10;j++)
			key[i][j] = '\0';
	//printf("%c, %d\n",head -> symbol, head -> frequency);
	//printf("%c, %d\n",head -> left -> symbol, head -> left -> frequency);
	//printf("%c, %d\n",head -> right -> symbol, head -> right -> frequency);
	int index;
	printOverhead(head,bytes,0,key);
	for(int x=0;x<messageLength;x++){
		index = (int)(message[x]);
		for(int y=1;y<(int)(key[index][0]);y++)
			printf("%c",key[index][y]);
	}
	printf("\n");
	return 0;
}
void printOverhead(TreeNode* head, char* a, int depth, char m[256][10]){
	//printf("At depth %d\n",depth);
	//printf("Passed 1\n");
	if(head -> symbol != 0){
		//printf("Passed 1.5\n");
		printf("%c",head -> symbol);
		int index = (int)(head -> symbol);
		m[index][0] = depth+1;
		for(int x=1;x<m[index][0];x++){
			m[index][x] = a[x-1];
			if(a[x-1]!=10)
				printf("%c",a[x-1]);
			else
				printf("\\n");
		}
		printf("\n");
		return;
	}
	//printf("Passed 2\n");
	a[depth] = '0';
	//printf("Passed 3\n");
	printOverhead(head -> left,a,depth+1,m);
	//printf("Passed 4\n");
	a[depth] = '1';
	//printf("Passed 5\n");
	printOverhead(head -> right,a,depth+1,m);
/*
Hello world from Kiran Ganeshan
17
l0000
n0001
 001
K01000
d01001
G01010
H01011
i01100
m01101
f01110
h01111
e1000
s10010
w10011
a101
o110
r111
0101110000000000011000110011110111000001001001011101111100110100101000011001111010001001010101010001100010010011111010001
*/
}






