#include "allfuncs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    USER *head = (USER *)malloc(sizeof(USER)), *current_user = (USER *)malloc(sizeof(USER));
    current_user->username = NULL;
    head->next_user = NULL;
    int end_of_program = 1;
    char **input = (char **)malloc(sizeof(char *)), all_words[256][256];
    while (end_of_program)
    {
        printf("\nplease enter the input here: ");
        input_catcher(input);
        printf("\n\n");
        decoder(input[0], all_words);
        end_of_program = recognizer(all_words, current_user, head);
        printf("\n\n");
        free(input[0]);
    }
    return 0;
}
