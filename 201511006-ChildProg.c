#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int main(int argc, char **argv)
{
   FILE *fp;
   char file_name[30];
   char ch;
   int frequency[27] = {0};
   char *key1 = argv[2]; // for start char
   char *key2 = argv[3]; //for finish char
   int x;                //char to int count

   fp = fopen(argv[1], "r");
   if (fp == NULL)
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }

   //reads char from file and finding frequency of it
   while ((ch = fgetc(fp)) != EOF)
   {
      //from A to Z
      if (ch >= *(key1)-32 && ch <= *(key2)-32)
      {
         x = ch - 'A';

         frequency[x]++;
      }
      //from a to z
      if (ch >= *(key1) && ch <= *(key2))
      {
         x = ch - 'a';
         frequency[x]++;
      }
   }

   fclose(fp);
   //printing it because we need it in stdout
   for (int i = 0; i < 26; ++i)
   {
      printf("%d,", frequency[i]);
   }
   printf("\n");
   return 0;
}
