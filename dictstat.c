#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "dictstat.h"
// Alphabet size (# of symbols)
#define ALPHA_SIZE (26)
// Converts word current character into index
// use only 'a' through 'z' and lower case
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')
 
char ALPHABET[26]="abcdefghijklmnopqrstuvwxyz";

trie Dictionary;        //creates Dictionary
 
Node *newNode(void)     // Returns new Tries node (initialized to NULLs)
{                         //Initializes its contents to 0 as well
    Node *pNode = NULL;
 
    pNode = (Node *)malloc(sizeof(Node));
 
    if( pNode )
    {
        int i;
 
        pNode->key = 0;
        pNode->occurance = 0;
        pNode->prefix = 0;
        pNode->superword = 0;
        for(i = 0; i < ALPHA_SIZE; i++)
        {
            pNode->child[i] = NULL;
        }
    }
 
    return pNode;
}
 
// Initializes Tries
void initialize(trie *pTrie)
{
    pTrie->root = newNode();
    pTrie->count = 0;
}

// If not present, addWords word into Tries
// If the word is prefix of Tries node, just marks leaf node
void addWord(trie *pTrie, char word[]){
    int nWord[100];
    int level;
    int length = strlen(word);
    int index;
    Node *tempNode;
 
    pTrie->count++;
    tempNode = pTrie->root;
 
    for( level = 0; level < length; level++ )
    {
        index = CHAR_TO_INDEX(word[level]);//stores word through the letter's index
        if( !tempNode->child[index] )
        {
            tempNode->child[index] = newNode();
        }
        nWord[level]=ALPHABET[index];        
        tempNode = tempNode->child[index];
    }
 
    // mark last node as leaf
    // records word in marked leaf
    tempNode->key = pTrie->count;
    int i;
    for(i=0; i<length; i++){
      tempNode->W[i]=nWord[i]; //Stores entire word in the key Node
      }
    tempNode->W[length]='\0';
}

void markPrefix(Node *tempNode){
   
   int i;
   for(i=0;i<ALPHA_SIZE;i++){
   if(tempNode->child[i]){
      if(tempNode->child[i]->key!=0){
      tempNode->child[i]->prefix++;
      markPrefix(tempNode->child[i]);
      }
      else{
      markPrefix(tempNode->child[i]);
      }
   }
   }
      
}

void searchOccurance(trie *pTrie, char word[]){    //Searches for occurances and adds to the nodes occurance data

    int level;
    int length = strlen(word);
    int index;
    Node *tempNode;
 
    tempNode = pTrie->root;
 
    for( level = 0; level < length; level++ )
    {
        index = CHAR_TO_INDEX(word[level]);
        
        if(tempNode->child[index]&&level==length-1){
        
        markPrefix(tempNode->child[index]);
        }
        else
        if( !tempNode->child[index] )
        {
            return;
        }
 
        tempNode = tempNode->child[index];
        if(tempNode->key!=0&&level!=length-1)   //if the word passes a marked leaf node, stores its superword+1
        tempNode->superword++;
    }
    
    tempNode->occurance++;          //Tallies the occurance
    
}

void readDict(FILE *dict_file){
   char file; 
   char temp[100];
   int i=0,N=0;                   
   
   while((!feof(dict_file))){// Preps file to be inserted
      file=fgetc(dict_file);            // into the Trie Data Structure
      if(isalpha(file)){
      temp[i]=tolower(file);   
         i++;
         }
      else if(i!=0&&isalpha(temp[i-1])&&!isalpha((file))){
         addWord(&Dictionary,temp);    //stores the word in the dictionary
         i=0;
         memset(temp,0,sizeof(temp));
         N++;
         }
      else 
      continue;
      
      }
   if(N==0)                      //checks number of words in dictionary
   printf("empty dictionary");   //if empty prints "empty dictionary
   
   }
   
void scanData(FILE *data_file){
   char temp[100],file;
   int i=0;
   while((!feof(data_file))){// Preps file to be inserted
      file=fgetc(data_file);            // into the Trie Data Structure
      if(isalpha(file)){
      temp[i]=tolower(file);   
         i++;
         }
      else if(i!=0&&isalpha(temp[i-1])&&!isalpha((file))){
         searchOccurance(&Dictionary,temp);  //Scans the word temp through the dictionary and records the requirements
         i=0;
         memset(temp,0,sizeof(temp));
         }
      else 
      continue;
      
      }  
   }    

void printTrie(Node *tempNode){ //set up to print entire trie
   
   int i;
   for(i=0;i<ALPHA_SIZE;i++){
   if(tempNode->child[i]){
      if(tempNode->child[i]->key!=0){
         printf("%s %d %d %d\n",tempNode->child[i]->W,tempNode->child[i]->occurance,
         tempNode->child[i]->prefix,tempNode->child[i]->superword);     //Prints nodes with marked key
                                                                        //along with the number of occurances,
      printTrie(tempNode->child[i]);                                    //prefixes, and superwords
      }
      else{
      printTrie(tempNode->child[i]);
      }
   }
   }
}

void printDictionary(trie dictionary){    //prints entire Dictionary trie
   
   Node *tempNode = dictionary.root;
   printTrie(tempNode);
}


void Free_Node(Node *tempNode){     // Frees memory from each node

   int i;
   
   for(i=0;i<ALPHA_SIZE;i++){
   if(tempNode->child[i]){    
                                                                         
      Free_Node(tempNode->child[i]);                                    
      free(tempNode->child[i]);
      }
   }
}

void free_Dictionary(trie dictionary){ //frees memory inside entire trie
   Node *tempNode=dictionary.root;
   Free_Node(tempNode);
   free(tempNode);
}


int main(int argc, char *argv[]){
   
   initialize(&Dictionary);      
   FILE *dict_file, *data_file;             
   
   dict_file= fopen(argv[1],"r");
   data_file= fopen(argv[2],"r");
   if(dict_file==NULL||data_file==NULL){
      printf("invalid input");
      return 0;
      }
      
   readDict(dict_file);
   scanData(data_file);
   printDictionary(Dictionary);
   
   free_Dictionary(Dictionary);

   if(fclose(dict_file)!=0 ||fclose(data_file)!=0 )
      printf("Error: couldnt close file");
   
   fclose(dict_file);
   fclose(data_file);
   return 0;
}
