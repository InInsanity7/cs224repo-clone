#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define BAD_NUMBER_ARGS 1

const int ASCII_126 = 126;
const int ASCII_32 = 32;

/**
 * Parses the command line.
 *
 * argc: the number of items on the command line (and length of the
 *       argv array) including the executable
 * argv: the array of arguments as strings (char* array)
 * bits: the integer value is set to TRUE if bits output indicated
 *       outherwise FALSE for hex output
 *
 * returns the input file pointer (FILE*)
 **/
FILE *parseCommandLine(int argc, char **argv, int *bits) {
  if (argc > 2) {
    printf("Usage: %s [-b|-bits]\n", argv[0]);
    exit(BAD_NUMBER_ARGS);
  }

  if (argc == 2 &&
      (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "-bits") == 0)) {
    *bits = TRUE;
  } else {
    *bits = FALSE;
  }

  return stdin;
}

/**
 * Writes data to stdout in hexadecimal.
 *
 * See myxxd.md for details.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/

void printDataAsHex(unsigned char *data, size_t size) {
  int printCount = 0;
  int decimalVal;
  //print data as hex
  for (int i = 0; i < size; i++) {
    printf(" ");
    decimalVal = (int)(data[i]);
    if (decimalVal < ASCII_32 || decimalVal > ASCII_126) {
        printf("0a");
    }
    else {
        printf("%02x", data[i]);
    }
    printCount += 3;
    i++;
    if (i < size) {
        decimalVal = (int)(data[i]);
        if (decimalVal < ASCII_32 || decimalVal > ASCII_126) {
            printf("0a");
        }
        else {
            printf("%02x", data[i]);
        }
        printCount += 2;
    }
  }
  //print remaining for width 20
  while (printCount < 40) {
	  printf(" ");
	  ++printCount;
  }
}

/**
 * Writes data to stdout as characters.
 *
 * See myxxd.md for details.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/
void printDataAsChars(unsigned char *data, size_t size) {
    int decimalVal;
	for(int i = 0; i < size; i++) {
        decimalVal = (int)(data[i]);
        if (decimalVal < ASCII_32 || decimalVal > ASCII_126) {
            printf(".");
        }
        else {
		printf("%c", data[i]);
        }
    }
}

//Hex read, send to print
void readAndPrintInputAsHex(FILE *input) {
  unsigned char data[16];
  int numBytesRead = fread(data, 1, 16, input);
  unsigned int offset = 0;
  while (numBytesRead != 0) {
    printf("%08x:", offset);
    offset += numBytesRead;
    printDataAsHex(data, numBytesRead);
    printf("  ");
    printDataAsChars(data, numBytesRead);
    printf("\n");
    numBytesRead = fread(data, 1, 16, input);
  }
}
//Bit print
void printDataAsBits(unsigned char *data, size_t size) {
  int printCount = 0;
  int binaryTmp;
  int decimalVal;
  for (int i = 0; i < size; i++) {
	printf(" ");
	++printCount;
	decimalVal = (int)(data[i]);
	for (int j = 7; j >= 0; j--) {
		binaryTmp = decimalVal / (1 << j);
		printf("%d", binaryTmp);
		++printCount;
		decimalVal = decimalVal % (1 << j);
	}
  }
  while (printCount < 54) {
	  printf(" ");
	  ++printCount;
  }
}
		//
	
//Bit read, send to print		     
void readAndPrintInputAsBits(FILE *input) {
  unsigned char data[6];
  int numBytesRead = fread(data, 1, 6, input);
  unsigned int offset = 0;
  while (numBytesRead != 0) {
    printf("%08x:", offset);
    offset += numBytesRead;
    printDataAsBits(data, numBytesRead);
    printf("  ");
    printDataAsChars(data, numBytesRead);
    printf("\n");
    numBytesRead = fread(data, 1, 6, input);
  }
}

int main(int argc, char **argv) {
  int bits = FALSE;
  FILE *input = parseCommandLine(argc, argv, &bits);

  if (bits == FALSE) {
    readAndPrintInputAsHex(input);
  } 
  else {
    readAndPrintInputAsBits(input);
  }
  return 0;
}
