#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

int main() {

    srand(time(NULL)); 
    
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

    return 0;
}