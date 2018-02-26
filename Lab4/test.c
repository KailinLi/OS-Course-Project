#include <sys/types.h> 
#include <sys/stat.h> 
#include <string.h> 
#include <stdio.h> 
#include <fcntl.h> 

 
int main(void) 
{ 
    int fd; 
    char buf[1024]; 
    char input[1024]; 
 
 
    fd = open("/dev/mydriver", O_RDWR, S_IRUSR|S_IWUSR);

    while (1) {
        printf("Input: ");
        scanf("%s", input);
        if (!strcmp(input, "q")) {
            break;
        }
        else if (!strcmp(input, "read")) {
            read(fd, &buf, sizeof(buf)); 
            printf("%s\n", buf);
        }
        else {
            printf("%s\n", input);
            write(fd, &input, sizeof(input));
        }
    }
 
    close(fd);//释放设备 
 
    return 0; 
} 