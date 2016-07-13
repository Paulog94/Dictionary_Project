/*dictstat.h*/
#define ALPHA_SIZE (26)
typedef struct TrieNode Node;
struct TrieNode
{
    int key;                  
    char W[100];              
    int occurance;            
    int prefix;               
    int superword;            
    Node *child[ALPHA_SIZE];
};
typedef struct Tries trie;
struct Tries
{
    Node *root;
    int count;
};
Node *newNode(void);
void initialize(trie *pTrie);
void addWord(trie *pTrie, char word[]);
void markPrefix(Node *tempNode);
void searchOccurance(trie *pTrie, char word[]);
