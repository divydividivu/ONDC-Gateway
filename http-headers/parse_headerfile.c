#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char* argv[])
{
    char const* const fileName = argv[1]; /* should check that argc > 1 */
    FILE* file = fopen(fileName, "r"); /* should check the result */
    char line[4096];
    char* strh[100];
    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        //printf("%s", line); 
        strh[i] = (char*)malloc(strlen(line) + 1);
        strcpy(strh[i], line);
        i++;
    }
    fclose(file);
    
    //printf("%d", i);
    char* headers[20]; char* values[20];
    for (int j = 0; j < i; j++)
    {   
        int k = 0;
        while(strh[j][k] != '\0' && strh[j][k] != '\n')
        {
            if (strh[j][k] == ':')
            {
                headers[j] = (char*)malloc(k+1);
                strncpy(headers[j], strh[j], k);
                headers[j][k] = '\0';
                values[j] = (char*)malloc(strlen(strh[j]) - k);
                strncpy(values[j], strh[j] + k + 2, strlen(strh[j]) - k);
                headers[j][k] = '\0';
                break;
            }
            k++;
        }
        printf("header = %s, ", headers[j]);

        printf("value = %s\n", values[j]);
    }
   
    return 0;
}