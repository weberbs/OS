#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
  //loop through each arg open file, read file, print,
  for (int i =1 ; i<argc; i++){
    //attemp to open file and check foro good open
    FILE *fp;
    fp = fopen(argv[i], "r");
    if (fp == NULL){
      fprintf(stdout, "wcat: cannot open file\n");
      return 1;
    }
    //read file line at a time and print
    char *text = NULL;
    size_t size = 0;
    while(getline(&text, &size, fp) != -1){
      printf("%s",text);
    }
    free(text);
    fclose(fp);
  }
  return 0;
}
