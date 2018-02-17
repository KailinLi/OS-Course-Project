#include <sys/types.h> 
#include <sys/stat.h> 
#include <string.h> 
#include <stdio.h> 
#include <fcntl.h> 
#include <unistd.h>
#include <sys/wait.h>
int main(void) { 
    FILE  *fp; 
    char buf[1024]; 
    char get[1024]; 
    memset(get, 0, sizeof(get)); 
    memset(buf, 0, sizeof(buf)); 
    printf("please enter a string you want input to mydriver:\n"); 
    scanf("%s", get);
    fp = fopen("/dev/mydev", "r+");
    if (fp != NULL) { 
        fread(buf, sizeof(char), sizeof(buf), fp); 
        printf("The message in mydriver now is: %s\n", buf); 
        fwrite(get, sizeof(char), sizeof(get), fp); 
        fread(buf, sizeof(char), sizeof(buf), fp); 
        printf("The message changed to: %s\n", buf); 
        sleep(1); 
    }  
    else { 
        printf("OMG..."); 
        return -1;
    }
    fclose(fp);
    return 0; 
} 