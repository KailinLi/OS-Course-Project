#include <string.h> 
#include <stdio.h> 
#include <unistd.h>
const int SIZE = 1024;
int main() {
    FILE * fp = NULL;
    char buffer[SIZE]; 
    char input[SIZE]; 
    memset(input, 0, sizeof(input)); 
    memset(buffer, 0, sizeof(buffer)); 
    fp = fopen("/dev/mydev", "r+");
    while (1) {
        printf("Input command: ");
        scanf("%s", input);
        if (!strcmp(input, "q")) {
            break;
        }
        else if (!strcmp(input, "read")) {
            fread(buffer, sizeof(char), SIZE, fp);
            printf("%s\n", buffer);
        }
        else {
            fwrite(input, sizeof(char), SIZE, fp);
        }
    }
    fclose(fp);
    return 0; 
} 