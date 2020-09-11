#include <stdio.h>

int main (int argc, char *argv[1]){
  //make sure files were passed
  if (argc < 2){
    printf("wzip: file1 [file2 ...]\n" );
    return 1;
  }
  //pointeer and size variables
  int *ip;
  size_t is = sizeof(int);
  int counter = 1;
  int cold;
  int cnew;
  ip = &counter;

  //open each file, loop though lines and print compressed
  for (int i = 1; i < argc; i++){
    FILE *fp;
    fp = fopen(argv[i], "r");
    if (fp==NULL){
      printf("wzip: error reading file\n");
      return 1;
    }
    //get first char and make sure file is not empty
    if(i == 1){
      cold = fgetc(fp);
      if(cold == EOF) return 0;
    }
    //get char increment counter print as new chars are found
    while((cnew=fgetc(fp)) != EOF){
      if (cold == cnew){
        counter++;
        cold = cnew;
      }
      else{
        fwrite(ip,is,1,stdout);
        printf("%c", cold);
        cold = cnew;
        counter = 1;
      }
    }
    fclose(fp);
  }
  //do once more for when EOF is reached after last file
  fwrite(ip,is,1,stdout);
  printf("%c", cold);
  cold = cnew;
}
