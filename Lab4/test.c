#include <string.h> 
#include <stdio.h> 
#include <unistd.h>
 
int main() {
    FILE * fp = NULL;
    char buffer[1024]; 
    char input[1024]; 
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
            fread(buffer, sizeof(char), 1024, fp);
            printf("%s\n", buffer);
        }
        else {
            fwrite(input, sizeof(char), 1024, fp);
        }
    }
    fclose(fp);
    return 0; 
} 