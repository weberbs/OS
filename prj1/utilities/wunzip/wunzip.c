#include <stdio.h>

int main (int argc, char *argv[1]){
  //make sure files were passed
  if (argc < 2){
    printf("wunzip: file1 [file2 ...]\n" );
    return 1;
  }
  //pointeer and size variables
  int *ip;
  size_t is = sizeof(int);
  char c[1];
  int count;
  ip = &count;

  //open each file, loop though lines and print compressed
  for (int i = 1; i < argc; i++){
    FILE *fp;
    fp = fopen(argv[i], "r");
    if (fp==NULL){
      printf("wunzip: error reading file\n");
      return 1;
    }
    //get the int and char
    while(fread(ip,is,1,fp) != 0){
      fread(c,1,1,fp);
      for (int j=0;j<count; j++){
        printf("%s",c);
      }
    }
    fclose(fp);
  }
  return 0;
}
