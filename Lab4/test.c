#include <string.h> 
#include <stdio.h> 
#include <unistd.h>
 
int main() {
    FILE *fp = NULL;
    char buffer[1024]; 
    char input[1024]; 
 
    memset(input, 0, sizeof(input)); 
    memset(buffer, 0, sizeof(buffer)); 
    printf("please enter a string you want input to mydriver:\n"); 
    // get(input); 
    fp = fopen("/dev/mydev", "r+");
 
    // fd = open("/dev/mydev", O_RDWR, S_IRUSR|S_IWUSR);//打开设备 
 
    if (fp != NULL) {
        fread(buffer, sizeof(char), 1024, fp);
        printf("The message in mydriver now is: %s\n", buffer); 
 
        //将输入写入设备 
        // write(fd, &input, sizeof(input)); 
        //读出设备的信息并打印 
        // read(fd, &buf, sizeof(buf)); 
        // printf("The message changed to: %s\n", buf); 
        // sleep(1); 
    }
    fclose(fp);
 
    // close(fd);//释放设备 
 
    return 0; 
} 