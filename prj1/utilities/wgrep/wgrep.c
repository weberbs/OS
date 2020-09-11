#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
  //make sure we have a search term
  if (argc < 2){
    printf("wgrep: searchterm [file ...]\n");
    return 1;
  }
  //save search word
  char *search = argv[1];
  //option when no  files passed
  if (argc == 2){
    char *text = NULL;
    size_t size = 0;
    while(getline(&text, &size, stdin) != -1){
      if (strstr(text, search) != NULL){
        printf("%s",text);
      }
    }
    free(text);
  }
  //loop through each arg open file, read file, print,
  for (int i = 2 ; i<argc; i++){
    //attemp to open file and check foro good open
    FILE *fp;
    fp = fopen(argv[i], "r");
    if (fp == NULL){
      fprintf(stdout, "wgrep: cannot open file\n");
      return 1;
    }
    //read file line at a time and print is search is in line
    char *text = NULL;
    size_t size = 0;
    while(getline(&text, &size, fp) != -1){
      if (strstr(text, search) != NULL){
        printf("%s",text);
      }
    }
    free(text);
    fclose(fp);
  }
  return 0;
}
