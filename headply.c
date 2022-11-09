/*

Print out the header of a PLY file.

Greg Turk

-----------------------------------------------------------------------

Copyright (c) 2020, Georgia Institute of Technology
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer. Redistributions in binary form
must reproduce the above copyright notice, this list of conditions and the
following disclaimer in the documentation and/or other materials provided with
the distribution.

Neither the name of the Georgia Institute of Technology nor the names of its
contributors may be used to endorse or promote products derived from this
software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <math.h>
#include <stdio.h>
#include <strings.h>

void read_header(FILE *fp);

/******************************************************************************
Main program.
******************************************************************************/

main(int argc, char *argv[]) {
  int i, j;
  char *s;
  char *progname;
  char filename[80];
  FILE *fp;

  progname = argv[0];

  while (--argc > 0 && (*++argv)[0] == '-') {
    for (s = argv[0] + 1; *s; s++)
      switch (*s) {
      /*
      case 'f':
        fract = atof (*++argv);
        argc -= 1;
        break;
      */
      default:
        usage(progname);
        exit(-1);
        break;
      }
  }

  /* read from named file or from stdin */

  if (argc >= 1) {

    strcpy(filename, *argv);
    if (strlen(filename) < 4 ||
        strcmp(filename + strlen(filename) - 4, ".ply") != 0)
      strcat(filename, ".ply");

    fp = fopen(filename, "r");

    if (fp == NULL) {
      fprintf(stderr, "Cannot open file '%s'\n", filename);
      exit(-1);
    }

    read_header(fp);
  } else
    read_header(stdin);
}

/******************************************************************************
Print out usage information.
******************************************************************************/

usage(char *progname) {
  fprintf(stderr, "usage: %s <in.ply\n", progname);
  fprintf(stderr, "  or\n");
  fprintf(stderr, "usage: %s in.[ply]\n", progname);
}

/******************************************************************************
Get a line from a text file.

Entry:
  fp - file to read from

Exit:
  returns string of characters from the file
******************************************************************************/

char *get_line(FILE *fp) {
  static char str[200];
  char *ptr;

  fgets(str, 200, fp);
  str[200] = '\0';

  /* replace new-line with null character */
  for (ptr = str; *ptr != '\0'; ptr++)
    if (*ptr == '\n') {
      *ptr = '\0';
      break;
    }

  return (str);
}

/******************************************************************************
Read the header from a PLY file and print it out.

Entry:
  fp - file pointer of the file to read from
******************************************************************************/

void read_header(FILE *fp) {
  char *str;
  int count = 0;

  str = get_line(fp);

  /* make sure this is a PLY file */
  if (strcmp(str, "ply") != 0) {
    fprintf(stderr, "This is not a PLY file.\n");
    exit(-1);
  }

  printf("%s\n", str);

  while (1) {

    str = get_line(fp);
    printf("%s\n", str);
    count++;

    if (count > 100) {
      fprintf(stderr, "Header seems to be too long, so we're quitting.\n");
      break;
    }

    if (strcmp(str, "end_header") == 0)
      break;
  }
}
