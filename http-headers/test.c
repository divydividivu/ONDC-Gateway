#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void func(char** foo)
{
    char* bar = "bar";
    *foo = bar;
}

int main()
{
    char buffer[1000000];
    char* buf = malloc(100);
    func(&buf);
    printf("%s", buf);
    printf("%d", sizeof(buf));
    FILE *fp = fopen("req", "r");
    char *filename = "req";
    fread(buffer, 1000000, 1, fp);
    char* header = strstr(buffer, "\r\n\r\n");
    
    printf("%s", header);
    fclose(fp);
}