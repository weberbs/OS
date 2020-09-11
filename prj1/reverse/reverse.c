#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

//create linked list to read in FILE
typedef struct Node {
  char line[500];
  struct Node *next;
}
Node;
//function prototypes
void printList(Node* n, FILE *out);
void freeList(Node *n);
int createList(FILE *input, FILE *output);

//main
int main(int argc, char *argv[]){
  //ifs to check how many args were entered
  if (argc == 1){ // read from shell print to shell
    if (createList(stdin, stdout) == 1){
      fprintf(stderr, "Memory error\n");
    }
  }
  else if (argc == 2){ // read from file print to shell
    //open input file and test for good open
    FILE *input;
    input = fopen(argv[1],"r");
    if (input == NULL) {
      fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
      return 1;
    }
    //creat and print list
    //public head of linked list
    Node *head;
    // initialize line to read in
    char *text = NULL;
    size_t len = 0;
    int nread;
    //read line at a time into linked list
    while ((nread = getline(&text, &len, input)) != -1){
      Node *n = malloc(sizeof(Node));
      //check for malloc error
      if (n==NULL) return 1;
      strcpy(n->line,text);
      n->next = head;
      head = n;
    }
    //free text which is malloced by getline
    free(text);
    printList(head,stdout);
    freeList(head);
    //close file stream
    fclose(input);
  }
  else if (argc == 3){// read from file print to file
    //open output file and test for good fopen
    FILE *output;
    output = fopen(argv[2],"w");
    if (output == NULL) {
      fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
      return 1;
    }
    //open input file and test for good open
    FILE *input;
    input = fopen(argv[1],"r");
    if (input == NULL) {
      fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
      return 1;
    }
    //system call to check for links
    struct stat inputstats;
    if(stat(argv[1], &inputstats) < 0){
      fprintf(stderr, "error accesing input stats\n");
      return 1;
    }
    struct stat outputstats;
    if(stat(argv[2], &outputstats) < 0){
      fprintf(stderr, "error accesing output stats\n");
      return 1;
    }
    //checks to be sure files are different and not linked
    if(inputstats.st_ino == outputstats.st_ino){
      fprintf(stderr, "reverse: input and output file must differ\n");
      return 1;
    }
    //create and print list
    //public head of linked list
    Node *head;
    // initialize line to read in
    char *text = NULL;
    size_t len = 0;
    int nread;
    //read line at a time into linked list
    while ((nread = getline(&text, &len, input)) != -1){
      Node *n = malloc(sizeof(Node));
      //check for malloc error
      if (n==NULL) return 1;
      strcpy(n->line,text);
      n->next = head;
      head = n;
    }
    //free text which is malloced by getline
    free(text);
    printList(head,output);
    freeList(head);
    //close input and output
    fclose(input);
    fclose(output);
  }
  else{ // invalid usage of reverse
    fprintf(stderr, "usage: reverse <input> <output>\n");
    return 1;
  }
  return 0;
}

//function to read lines into linked list
int createList(FILE *input, FILE *output){
  //public head of linked list
  Node *head;
  // initialize line to read in
  char *text = NULL;
  size_t len = 0;
  int nread;
  //read line at a time into linked list
  while ((nread = getline(&text, &len, input)) != -1){
    Node *n = malloc(sizeof(Node));
    //check for malloc error
    if (n==NULL) return 1;
    strcpy(n->line,text);
    n->next = head;
    head = n;
  }
  //free text which is malloced by getline
  free(text);
  printList(head,output);
  freeList(head);
  return 0;
}

//Function to free memory used in linked list from
//https://stackoverflow.com/questions/6417158/c-how-to-free-nodes-in-the-linked-list
void freeList(Node* head)
{
   Node* tmp;
   while (head != NULL)
    {
       tmp = head;
       head = head->next;
       free(tmp);
    }
}

// This function prints contents of linked list starting from
// the given node modled after function
// https://www.geeksforgeeks.org/linked-list-set-1-introduction/
void printList(struct Node* n,FILE *output)
{
    while (n != NULL) {
        fprintf(output, "%s", n->line);
        n = n->next;
    }
}
