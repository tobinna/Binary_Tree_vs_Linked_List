#include "bstdb.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool int 
#define MAX 999999

struct BT {
            // unique identifier for the document
  char *name;             // file name of the document    
  int   word_count; 
  int height;     // number of words in the document      
  int doc_id;
  struct BT * left, *right; 
};

int num_searches;
int nodenum;
struct BT * binarytree;
int num_books;
int id_number;
int num_comps;
int arraysize;
int ids[MAX];

int
bstdb_init ( void ) {
  
  binarytree = NULL;
  id_number = 0;
  nodenum=0;
  num_searches = 0;
  num_books = 0;
  num_comps =0;
  arraysize = 0;
  
	return 1;
}

static void
free_list_node ( struct BT *node ) {
    struct BT *p;
    p = node;
    while (p) {
        p = node->left;
        if (node->name) free(node->name);
        free(node);
        
        p = node->right;
        if (node->name) free(node->name);
        free(node);
        node = p;

    }
}

struct BT* newNode_char(char * item, int count, int id) 
{ 
    size_t len;
    struct BT* temp = (struct BT*)malloc(sizeof(struct BT)); 
    //temp->name = item; 
    temp->word_count = count; 
    temp->doc_id = id;
    temp->height = 1;
    temp->left = temp->right = NULL; 


    len = strlen(item)+1;
        temp->name = calloc(len, sizeof(char));
        if (temp->name) {
            // if calloc was successful, copy the filename into the node
            strcpy( temp->name, item );
        } else {
            // if calloc failed, release any memory that was allocated and 
            // report an error by returning NULL
            temp->left = temp->right = NULL; 
            free_list_node(temp);
            temp = NULL;
        }





    return temp; 
} 

 
  
// A utility function to get the height of the tree 
int height(struct BT *N) 
{ 
    if (N == NULL) 
        return 0; 
    return N->height; 
} 
  
// A utility function to get maximum of two integers 
int max(int a, int b) 
{ 
    return (a > b)? a : b; 
} 
// A utility function to right rotate subtree rooted with y 
// See the diagram given above. 
struct BT *rightRotate(struct BT *y) 
{ 
    struct BT *x = y->left; 
    struct BT *T2 = x->right; 
  
    // Perform rotation 
    x->right = y; 
    y->left = T2; 
  
    // Update heights 
    y->height = max(height(y->left), height(y->right))+1; 
    x->height = max(height(x->left), height(x->right))+1; 
  
    // Return new root 
    return x; 
} 
  
// A utility function to left rotate subtree rooted with x 
// See the diagram given above. 
struct BT *leftRotate(struct BT *x) 
{ 
    struct BT *y = x->right; 
    struct BT *T2 = y->left; 
  
    // Perform rotation 
    y->left = x; 
    x->right = T2; 
  
    //  Update heights 
    x->height = max(height(x->left), height(x->right))+1; 
    y->height = max(height(y->left), height(y->right))+1; 
  
    // Return new root 
    return y; 
} 
  
// Get Balance factor of node N 
int getBalance(struct BT *N) 
{ 
    if (N == NULL) 
        return 0; 
    return height(N->left) - height(N->right); 
} 
  
// Recursive function to insert a key in the subtree rooted 
// with node and returns the new root of the subtree. 
struct BT* insert(struct BT * node, char *name, int count, int id) 
{ 
    /* 1.  Perform the normal BST insertion */
    if (node == NULL) 
        return(newNode_char(name, count, id)); 
  
    if (id < node->doc_id) 
        node->left  = insert(node->left, name, count, id); 
    else if (id > node->doc_id) 
        node->right = insert(node->right, name, count, id); 
    else // Equal keys are not allowed in BST 
        return node; 
  
    /* 2. Update height of this ancestor node */
    node->height = 1 + max(height(node->left), 
                           height(node->right)); 
  
    /* 3. Get the balance factor of this ancestor 
          node to check whether this node became 
          unbalanced */
    int balance = getBalance(node); 
  
    // If this node becomes unbalanced, then 
    // there are 4 cases 
  
    // Left Left Case 
    if (balance > 1 && id < node->left->doc_id) 
        return rightRotate(node); 
  
    // Right Right Case 
    if (balance < -1 && id > node->right->doc_id) 
        return leftRotate(node); 
  
    // Left Right Case 
    if (balance > 1 && id > node->left->doc_id) 
    { 
        node->left =  leftRotate(node->left); 
        return rightRotate(node); 
    } 
  
    // Right Left Case 
    if (balance < -1 && id < node->right->doc_id) 
    { 
        node->right = rightRotate(node->right); 
        return leftRotate(node); 
    } 
  
    /* return the (unchanged) node pointer */
    return node; 
} 




int hash(char* s, int count){

  int hashAddress = 5381;
  while (*s){
    hashAddress = ((hashAddress << 5) + hashAddress) + *s + count;
    s++;
}

return abs(hashAddress);
}


int bstdb_add ( char *name, int word_count ) {
  
  struct BT * temp;
 
  int id = abs(hash(name, word_count));

  ids[arraysize] = id;

  arraysize++;
  
  temp = insert(binarytree, name, word_count, id);
  
  if(!temp) { return -1; }
  
  binarytree = temp;

  

return id;
	
}

static struct BT *
find_document ( int doc_id ) {
    struct BT *p = binarytree;
 
    num_searches++;
    
    while (p) {
        // count the number of traversals
        num_comps++;
        // If we found the node, return it. Otherwise keep searching.
        if (doc_id > p->doc_id){
          p = p->right;
        }
        else if (doc_id < p->doc_id){
          p = p->left;
        }

        if (p->doc_id == doc_id) { break; }
        
    }

    return p;
}

int
bstdb_get_word_count ( int doc_id ) {
  
  
  struct BT* tmp = find_document(doc_id);
  
  //return abs(search_count);
	return (tmp)? tmp->word_count : -1;
	//return -1;
}

char* bstdb_get_name ( int doc_id ) {
  num_books ++;
  //search_char(binarytree, doc_id, 0);
  struct BT* tmp = find_document(doc_id); 
  return (tmp)? tmp->name : 0;
  //return (search_name)? search_name : 0;

	//return 0;
}
int total_l=0;
int total_r=0;

bool isBalanced(struct BT* root) 
{ 
    int lh; /* for height of left subtree */
    int rh; /* for height of right subtree */
  
    /* If tree is empty then return true */
    if (root == NULL) 
        return 1; 
  
    /* Get the height of left and right sub trees */
    lh = height(root->left); 
    rh = height(root->right); 
    total_l = total_l +lh;
    total_r = total_r +rh;
    if (abs(lh - rh) <= 1 && isBalanced(root->left) && isBalanced(root->right)) 
        return 1; 
  
    /* If we reach here then tree is not height-balanced */
    return 0; 
} 

void nodenumber(struct BT* root) 
{ 
  
    if (root != NULL) { 
        nodenum++;
        nodenumber(root->left); 
        
        nodenumber(root->right); 
    } 

}

void repeated(){

  for (int i = 0;  i < arraysize; i++)
{
    //create nested loop for compare current values with actual value of arr
    for (int j = i+1; j < arraysize; j++)
    {

        //and here we put our condition
        if (ids[i] == ids[j])
        {
            printf("REPEATED ID\n");
        }
    }
}
}

void
bstdb_stat ( void ) {
  printf("STATS\n");
  printf("-------------------------------------------\n\n");

  
    if (isBalanced(binarytree)) 
        printf("TREE IS BALANCED\n"); 
    else
        printf("TREE IS NOT BALANCED\n"); 

  printf("%i LEFT NODES\t\t\t%i RIGHT NODES\n\n", total_l, total_r);
  //num_searches = num_searches/2;
  //num_books ++;
  printf("TOTAL SEARCHES:\t\t\t\t%i\t\n", num_searches);
  printf("TOTAL COMPS:\t\t\t\t%i\t\n", num_comps);
  nodenumber(binarytree);
  printf("TOTAL NODES:\t\t\t\t%i\n", nodenum);
  printf("AVERAGE NODES SEARCHED:\t\t%.2f \n", (double)num_comps/num_searches);
  printf("REPEATED NODES:\t\t\t\t%i\n", arraysize-nodenum);

  //repeated();
  
  


	// Use this function to show off! It will be called once after the 
	// profiler ends. The profiler checks for execution time and simple errors,
	// but you should use this function to demonstrate your own innovation.
	//
	// Suggestions for things you might want to demonstrate are given below,
	// but in general what you choose to show here is up to you. This function
	// counts for marks so make sure it does something interesting or useful.
	//
	//  + Check if your tree is balanced and print the result
	//
	//  + Does the number of nodes in the tree match the number you expect
	//    based on the number of insertions you performed?
	//
	//  + How many nodes on average did you need to traverse in order to find
	//    a search result? 
	//
	//  + Can you prove that there are no accidental duplicate document IDs
	//    in the tree?


}



void deleteTreeUtil(struct BT* node)
   {
       if (node == NULL) return;
       deleteTreeUtil(node->left);
       deleteTreeUtil(node->right);
       //printf("\n Deleting node: %c", node->letter);
       free(node);
    }

void deleteTree(struct BT** node_ref)
{
  deleteTreeUtil(*node_ref);
  *node_ref = NULL;
}

void inorder_print(struct BT* root) 
{ 
  
    if (root != NULL) { 
      
        inorder_print(root->left); 
        printf("%s \n", root->name); 
        printf("%i \n", root->doc_id); 
        inorder_print(root->right); 
    } 
} 

void
bstdb_quit ( void ) {
  //inorder_print(binarytree);
  
  deleteTree(&binarytree);
  printf("LIST IS DELETED\n");
	inorder_print(binarytree);
}

