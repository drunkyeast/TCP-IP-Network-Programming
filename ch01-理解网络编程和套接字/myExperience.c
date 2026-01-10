#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h> // 需要包含这个头文件
int main() {
    umask(0); // 将当前进程的 umask 设置为 0，不屏蔽任何权限

    int fd;
    
    // --- 实验 1: 强行让权限变成 0777 (rwxrwxrwx) ---
    // 我们用汇编指令把 0777 放到 rdx 寄存器
    asm("mov $0777, %%rdx" ::: "rdx"); 
    fd = open("hack_777.txt", O_CREAT | O_WRONLY | O_TRUNC);
    close(fd);

    // --- 实验 2: 强行让权限变成 0444 (r--r--r--) ---
    asm("mov $0444, %%rdx" ::: "rdx");
    fd = open("hack_444.txt", O_CREAT | O_WRONLY | O_TRUNC);
    close(fd);

    printf("实验完成，请执行 ls -l hack_*.txt 查看权限！\n");
    return 0;
}