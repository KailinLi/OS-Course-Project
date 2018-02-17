#include <sys/types.h> 
#include <sys/stat.h> 
#include <string.h> 
#include <stdio.h> 
#include <fcntl.h> 
#include <unistd.h>
#include <sys/wait.h>
int main(void) { 
    int fd; 
    char buf[1024]; 
    char get[1024]; 
    memset(get, 0, sizeof(get)); 
    memset(buf, 0, sizeof(buf)); 
    printf("please enter a string you want input to mydriver:\n"); 
    scanf("%s", get);
    fd = open("/dev/mydriver", O_RDWR, S_IRUSR|S_IWUSR);
    if (fd > 0) { 
        read(fd, buf, sizeof(buf)); 
        printf("The message in mydriver now is: %s\n", buf); 
        write(fd, get, sizeof(get)); 
        read(fd, buf, sizeof(buf)); 
        printf("The message changed to: %s\n", buf); 
        sleep(1); 
    }  
    else { 
        printf("OMG..."); 
        return -1; 
    }
    close(fd);
    return 0; 
} 