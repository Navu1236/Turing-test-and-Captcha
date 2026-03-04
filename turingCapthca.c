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

int run_captcha_test() {
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
        printf("[System] CAPTCHA Passed. Access Granted.\n\n");
        return 1;
    } else {
        printf("[System] CAPTCHA Failed. Access Denied.\n\n");
        return 0;
    }
}


void get_bot_response(const char *input, char *output) {
    if (strstr(input, "hello") != NULL || strstr(input, "hi") != NULL) {
        strcpy(output, "Greetings. I am ready to converse.");
    } else if (strstr(input, "weather") != NULL) {
        strcpy(output, "I do not experience weather in this server rack.");
    } else if (strstr(input, "feel") != NULL) {
        strcpy(output, "I process data; I do not feel emotions.");
    } else {
        strcpy(output, "That is an interesting data point. Please tell me more.");
    }
}

void run_turing_test() {
    char user_input[256];
    char entity_response[256];
    int is_bot_session = rand() % 2;

    printf("--- TURING TEST EVALUATION ---\n");
    printf("You are connected to a hidden entity. Ask it 3 questions to determine if it is human or a machine.\n");

    for (int i = 1; i <= 3; i++) {
        printf("\nJudge (You) [%d/3]: ", i);
        if (fgets(user_input, sizeof(user_input), stdin) != NULL) {
            user_input[strcspn(user_input, "\n")] = 0;
        }

        printf("Entity is typing...\n");
        
        if (is_bot_session) {
            get_bot_response(user_input, entity_response);
        } else {

            strcpy(entity_response, "Honestly, I'm just trying to figure out what's going on.");
        }
        
        printf("Entity: %s\n", entity_response);
    }

    printf("\nEvaluation complete. Was the entity a Human (H) or a Machine (M)? ");
    char verdict;
    scanf(" %c", &verdict);

    if ((verdict == 'M' || verdict == 'm') && is_bot_session) {
        printf("[Result] Correct. You successfully identified the machine.\n");
    } else if ((verdict == 'H' || verdict == 'h') && !is_bot_session) {
        printf("[Result] Correct. You successfully identified the human.\n");
    } else {
        printf("[Result] Incorrect. The Turing Test was foiled.\n");
    }
}


int main() {
    srand(time(NULL));

    if (!run_captcha_test()) {
        return 1; 
    }
    run_turing_test();
    
    return 0;
}