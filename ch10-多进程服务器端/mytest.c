#include <stdio.h>
int main() {
    char str[100];
    printf("Please input something: ");
    scanf("%s", str);  // 尝试读取键盘输入
    printf("You entered: %s\n", str);
    return 0;
}