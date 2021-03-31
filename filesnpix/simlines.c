#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "pnmrdr.h"
#include "table.h"
#include "readaline.h"
#include "atom.h"



typedef struct LineData {
  const char *atom;
  char *file_name;
  int line_number;
  int size;
  struct LineData *next;

} LineData;

char *fixLine(char *string, int size);
const char *lineToAtom(char *string);
LineData *newLineData(FILE *fp, char *file_name);


int main(int argc, char *argv[]){
  FILE *fp = fopen(argv[1], "r");
  LineData *test = newLineData(fp, argv[1]);

  printf("silence warning %s, %d\n", test->atom, argc);
  // printf("file name? %s\n", test->file_name);
  // printf("line num? %d\n", test->line_number);
  // printf("size? %d\n", test->size);

  // char *string;
  // char *clean_line;
  // const char *atom;
  // int size;
  // int x = 0;
  //
  // // need to make sure doesnt print beyond end of file
  // while(x < 2) {
  //   size = readaline(fp, &string);
  //   clean_line = fixLine(string, size);
  //   x++;
  //   atom = lineToAtom(clean_line);
  //   printf("%d, %d, atom: %s \n", size, argc, atom);
  //
  //   // free(clean_line);
  //   free(string);
  // }

  fclose(fp);
  return 0;
}

LineData *newLineData(FILE *fp, char *file_name) {
  char *string;
  char *clean_line;
  // const char *atom;
  int size;
  int x = 0;
  int numline = 0;

  LineData *test;
  test = malloc(sizeof(LineData));

  // x++;
  // atom = lineToAtom(clean_line);
  // free(string);
  while(!eof()) {
    size = readaline(fp, &string);
    clean_line = fixLine(string, size);
    numline++;
    test->atom = lineToAtom(clean_line);
    test->file_name = file_name;
    test->line_number = numline;
    test->size = size;

    printf("atom? %s\n", test->atom);
    printf("file name? %s\n", test->file_name);
    printf("line num? %d\n", test->line_number);
    printf("size? %d\n", test->size);

    x++;
  }

  printf("here?\n");
  return test;

}






const char *lineToAtom(char *string) {
  const char *atom_key = Atom_string(string);
  free(string);
  return atom_key;
}





char *fixLine(char *string, int size) {
  char *array;
  array = (char *)(malloc(size*sizeof(char)));
  int nonWordChar = 2;
  int newSize = 0;
  char c;
  c = *string;

  while(c != '\n') {
    if (isalnum(c) || c == '_') {
      *array = c;
      nonWordChar = 0;
      newSize++;
      array++;
    } else if (!isalnum(c) && (nonWordChar == 0)) {
      *array = '$';
      nonWordChar = 1;
      newSize++;
      array++;
    }
    else if (!isalnum(c) && (nonWordChar == 1)) {
    }
    string++;
    c = *string;
  }
  *(array - 1) = '\0';
  array = array - newSize;

  return array;
}