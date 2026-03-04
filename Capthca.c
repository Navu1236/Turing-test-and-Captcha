#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void generate_captcha_string(char *str, int length) {
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < length; i++) {
        int key = rand() % (int)(sizeof(charset) - 1);
        str[i] = charset[key];
    }
    str[length] = '\0';
}

int main() {
    srand(time(NULL)); 
    
    char captcha[6];
    char user_input[10];
    
    generate_captcha_string(captcha, 5);
    
    printf("--- CAPTCHA VERIFICATION ---\n");
    printf("Please type the following characters to prove you are human: %s\n", captcha);
    printf("Input: ");

    if (fgets(user_input, sizeof(user_input), stdin) != NULL) {
        user_input[strcspn(user_input, "\n")] = 0; 
    }

    if (strcmp(captcha, user_input) == 0) {
        printf("[System] CAPTCHA Passed. Access Granted.\n");
        return 0; 
    } else {
        printf("[System] CAPTCHA Failed. Access Denied.\n");
        return 1; 
    }
}