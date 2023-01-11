#include "allfuncs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int recognizer(char all_words[MAX_LEN][MAX_LEN], USER *current_user, USER *head)
{
    if (!strcmp(all_words[0], "signup"))
    {
        if (current_user->username) /*if the current user existed*/
        {
            printf("\nyou have to exit your account to sign up!\n");
            return PROGRAM_CONTINUES;
        }
        else
        {
            if (all_words[3][0] == '\0' && all_words[2][0] != '\0' && all_words[1][0] != '\0') /*resistance against the form*/
            {
                USER *cur = head->next_user;
                while (cur) /*checking if the user exists*/
                {
                    if (!strcmp(cur->username, all_words[1]))
                    {
                        printf("\nuser already exists!\n");
                        return PROGRAM_CONTINUES;
                    }
                    cur = cur->next_user;
                }

                if (all_words[1][0] == ' ' || all_words[2][0] == ' ')
                {
                    printf("\nyou have to enter a username or password!\n");
                    return PROGRAM_CONTINUES;
                }

                /*if the user did not exist then*/
                int word_length;
                USER *new_user = (USER *)malloc(sizeof(USER)); /*making new user*/
                if (!new_user)
                {
                    printf("malloc err rec 48");
                    return PROGRAM_CONTINUES;
                }
                node_adder(head, new_user); /*adding the new created user to our linked list*/

                word_length = word_length_finder(all_words[1]); /*finding the word length of the second word*/
                new_user->username = (char *)malloc(sizeof(char) * (word_length + 1));
                if (!new_user->username)
                {
                    printf("malloc err rec 56");
                    return PROGRAM_CONTINUES;
                }
                copier(new_user->username, all_words[1], word_length + 1);

                word_length = word_length_finder(all_words[2]); /*finding the word length of the third word*/
                new_user->password = (char *)malloc(sizeof(char) * (word_length + 1));
                if (!new_user->password)
                {
                    printf("malloc err rec 64");
                    return PROGRAM_CONTINUES;
                }
                copier(new_user->password, all_words[2], word_length + 1);

                new_user->posts_head = (POSTS *)malloc(sizeof(POSTS)); /*making the posts head node*/
                if (!new_user->posts_head)
                {
                    printf("malloc err rec 71");
                    return PROGRAM_CONTINUES;
                }
                new_user->posts_head->next = NULL; /*tale of the posts linked list*/
                new_user->posts_head->id = 0;      /*head node is dummy*/

                new_user->liked_posts_head = (LIKED_POSTS *)malloc(sizeof(LIKED_POSTS)); /*making the liked posts head node*/
                if (!new_user->liked_posts_head)
                {
                    printf("malloc err rec 106");
                    return PROGRAM_CONTINUES;
                }
                new_user->liked_posts_head->next = NULL; /*tale of the liked posts linked list*/

                accounts_file_writer(head);

                printf("\nyou succesfuly signed up!\n");
                return PROGRAM_CONTINUES;
            }
            else
            {
                printf("\nwrong input\n");
                return PROGRAM_CONTINUES;
            }
        }
    }
    else if (!strcmp(all_words[0], "login"))
    {
        if (current_user->username) /*if the current user existed*/
        {
            printf("\nyou have to exit your account before logging in!\n");
            return PROGRAM_CONTINUES;
        }
        else if (all_words[3][0] == '\0' && all_words[2][0] != '\0' && all_words[1][0] != '\0') /*resistance against the form*/
        {
            USER *cur = head->next_user; /*to find the user*/
            while (1)
            {
                if (cur == NULL) /*means that the linked list is over and no user have been found*/
                {
                    printf("\nwrong username or password\n");
                    return PROGRAM_CONTINUES;
                }
                else if (!strcmp(cur->username, all_words[1]) && !strcmp(cur->password, all_words[2]))
                {
                    memcpy(current_user, cur, sizeof(USER));
                    printf("\nyou succesfuly entered your acount as: %s\n", current_user->username);
                    return PROGRAM_CONTINUES;
                }
                else /*going to the next element*/
                {
                    cur = cur->next_user;
                }
            }
        }
        else
        {
            printf("\nwrong input\n");
            return PROGRAM_CONTINUES;
        }
    }

    else if (!strcmp(all_words[0], "post"))
    {
        if (!current_user->username) /*user must be logged in*/
        {
            printf("\nyou have to login first!\n");
            return PROGRAM_CONTINUES;
        }

        else if (all_words[1][0] != '\0') /*resistance against form*/
        {
            POSTS *new_post = (POSTS *)malloc(sizeof(POSTS)); /*making a new post struct*/

            int words_count;
            words_count = words_count_finder(all_words);
            new_post->post_text = (char **)malloc(sizeof(char *) * (words_count + 1)); /*making elements for post counts*/
            if (!new_post->post_text)
            {
                printf("malloc err rec 150");
                return PROGRAM_CONTINUES;
            }
            post_text_copier(new_post->post_text, all_words, words_count);

            new_post->likes = 0; /*initing like counts*/

            USER *the_user = user_finder(head, current_user->username); /*finding the user and saving it in a temperrory struct*/
            POSTS *cur = the_user->posts_head;
            while (cur != NULL)
            {
                if (!cur->next) /*add the post to the last node of posts linked list*/
                {
                    node_post_adder(cur, new_post);
                    new_post->id = cur->id + 1;
                    break;
                }
                cur = cur->next;
            }

            accounts_file_writer(head);
            posts_file_writer(head);

            printf("\nyou succesfuly posted:\n");
            post_text_printer(new_post->post_text);
            return PROGRAM_CONTINUES;
        }
        else
        {
            printf("\nwrong input\n");
            return PROGRAM_CONTINUES;
        }
    }

    else if (!strcmp(all_words[0], "like"))
    {
        if (!current_user->username) /*if the current user didn't exist*/
        {
            printf("\nyou have to login to like a post!\n");
            return PROGRAM_CONTINUES;
        }
        else if (all_words[1][0] != '\0' && all_words[2][0] != '\0' && all_words[3][0] == '\0') /*resistance against the form*/
        {

            if (liked_post_checker(current_user->liked_posts_head, all_words)) /*1 means that the post is already liked*/
            {
                printf("\nyou already liked this post!\n");
                return PROGRAM_CONTINUES;
            }

            /*finding the current user in th linked list*/
            USER *the_user = user_finder(head, current_user->username);
            /*saving the username*/
            LIKED_POSTS *new_liked_post = (LIKED_POSTS *)malloc(sizeof(LIKED_POSTS));
            int word_length = word_length_finder(all_words[1]);
            new_liked_post->user = (char *)malloc(sizeof(char) * (word_length + 1));
            if (!new_liked_post->user)
            {
                printf("malloc err rec 222");
                return PROGRAM_CONTINUES;
            }
            copier(new_liked_post->user, all_words[1], word_length + 1);
            /*finding the id out of string*/
            int id = string_to_int(all_words[2]);
            new_liked_post->id = id;
            /*adding this post to the liked posts linked list*/
            node_likedposts_adder(the_user->liked_posts_head, new_liked_post);

            /*finding the post owner*/
            the_user = user_finder(head, all_words[1]);
            if (!the_user)
            {
                printf("\nno users with this username has been found!\n");
                return PROGRAM_CONTINUES;
            }
            /*finding the called post*/
            POSTS *the_post = post_finder(the_user->posts_head, id);
            if (!the_post)
            {
                printf("\nno posts with this id has been found!\n");
                return PROGRAM_CONTINUES;
            }
            (the_post->likes)++;
            posts_file_writer(head);

            printf("\nyou liked the post succesfuly!\n");

            return PROGRAM_CONTINUES;
        }
        else
        {
            printf("\nwrong input\n");
            return PROGRAM_CONTINUES;
        }
    }
    else if (!strcmp(all_words[0], "logout"))
    {
        if (!current_user->username) /*if the current user didn't exist*/
        {
            printf("\nyou have to either signup or login to exit your acount!\n");
            return PROGRAM_CONTINUES;
        }
        else if (all_words[1][0] == '\0') /*resistance against the form*/
        {
            current_user->username = NULL;
            printf("\nyou succesfully exited your acount!\n");
            return PROGRAM_CONTINUES;
        }
        else
        {
            printf("\nwrong input\n");
            return PROGRAM_CONTINUES;
        }
    }
    else if (!strcmp(all_words[0], "delete"))
    {
        if (!current_user->username) /*if the current user didn't exist*/
        {
            printf("\nyou have to login to delete a post!\n");
            return PROGRAM_CONTINUES;
        }
        else if (all_words[1][0] != '\0' && all_words[2][0] == '\0') /*resistance against the form*/
        {
            int id = string_to_int(all_words[1]);
            USER *the_user = user_finder(head, current_user->username);
            post_deleter(the_user->posts_head, id);
            accounts_file_writer(head);
            posts_file_writer(head);

            return PROGRAM_CONTINUES;
        }
        else
        {
            printf("\nwrong input\n");
            return PROGRAM_CONTINUES;
        }
    }
    else if (!strcmp(all_words[0], "find_user"))
    {
        if (!current_user->username) /*if the current user didn't exist*/
        {
            printf("\nyou have to login to find a user!\n");
            return PROGRAM_CONTINUES;
        }
        else if (all_words[2][0] == '\0' && all_words[1][0] != '\0')
        {
            USER *the_user = user_finder(head, all_words[1]);
            if (!the_user) /*uiser existance checker*/
            {
                printf("\nno users with this username has been found!\n");
                return PROGRAM_CONTINUES;
            }

            printf("\nthe user you are looking for is: %s\n", the_user->username);
            all_posts_printer(the_user->posts_head, the_user->username);

            return PROGRAM_CONTINUES;
        }
        else
        {
            printf("\nwrong input\n");
            return PROGRAM_CONTINUES;
        }
    }
    else if (!strcmp(all_words[0], "info"))
    {
        if (!current_user->username) /*if the current user didn't exist*/
        {
            printf("\nyou have to login to see your info!\n");
            return PROGRAM_CONTINUES;
        }
        else if (all_words[1][0] == '\0') /*resistance against the form*/
        {
            printf("\nyour username is: %s\n", current_user->username);
            printf("\nyour password is: %s\n", current_user->password);
            all_posts_printer(current_user->posts_head, current_user->username);
            return PROGRAM_CONTINUES;
        }
        else
        {
            printf("\nwrong input\n");
            return PROGRAM_CONTINUES;
        }
    }
    else if (!strcmp(all_words[0], "end")) /*calling for an end*/
    {
        if (all_words[1][0] == '\0') /*resistance against form*/
        {
            printf("\nprogram is over\n");
            return PROGRAM_ENDS;
        }
        else
        {
            printf("\nwrong input\n");
            return PROGRAM_CONTINUES;
        }
    }
    else
    {
        printf("\nwrong input\n");
        return PROGRAM_CONTINUES;
    }
}

int word_length_finder(char *word)
{
    int i;
    for (i = 0; word[i] != '\0'; i++)
    {
    }
    return i;
}

void node_adder(USER *prev, USER *new_node)
{
    new_node->next_user = prev->next_user;
    prev->next_user = new_node;
}

void node_post_adder(POSTS *prev, POSTS *new_node)
{
    new_node->next = prev->next;
    prev->next = new_node;
}

void node_likedposts_adder(LIKED_POSTS *prev, LIKED_POSTS *new_node)
{
    new_node->next = prev->next;
    prev->next = new_node;
}

int words_count_finder(char array[MAX_LEN][MAX_LEN])
{
    int i;
    for (i = 0; array[i][0] != '\0'; i++)
        ;
    return i - 1;
}

int post_text_copier(char **dest, char start[MAX_LEN][MAX_LEN], int words_count)
{
    int flag = 0;
    for (int i = 0; i < words_count; i++)
    {
        int word_length = word_length_finder(start[i + 1]);
        dest[i] = (char *)malloc(sizeof(char) * word_length);
        if (!dest[i])
        {
            printf("malloc error rec 262");
            return PROGRAM_CONTINUES;
        }
        copier(dest[i], start[i + 1], word_length);
        if (start[i][0] != ' ')
        {
            flag = 1;
        }
    }
    dest[words_count] = NULL;
    return flag;
}

USER *user_finder(USER *head, char *name)
{
    USER *cur = head->next_user;
    while (cur)
    {
        if (!strcmp(cur->username, name))
        {
            return cur;
        }
        cur = cur->next_user;
    }
    return NULL;
}

void post_text_printer(char **post_text)
{
    printf("\n");
    for (int i = 0; post_text[i] != NULL; i++)
    {
        if (post_text[i][0] == ' ')
        {
            printf(" ");
        }

        else
        {
            printf("%s ", post_text[i]);
        }
    }
    printf("\n");
}

void all_posts_printer(POSTS *head, char *username)
{
    POSTS *cur = head->next;
    if (!cur)
    {
        printf("\nthere is no post to show!\n");
    }

    while (cur)
    {
        if (cur->id)
        {
            printf("\n\nuser: %s\n", username);
            printf("\npost id: %d\n", cur->id);
            post_text_printer(cur->post_text);
            printf("\nlike counts: %d\n\n", cur->likes);
        }
        cur = cur->next;
    }
    return;
}

int liked_post_checker(LIKED_POSTS *liked_posts_head, char all_words[MAX_LEN][MAX_LEN])
{
    int id = string_to_int(all_words[2]);
    LIKED_POSTS *cur = liked_posts_head->next;
    while (cur)
    {
        if (!strcmp(all_words[1], cur->user) && id == cur->id)
        {
            return 1;
        }

        cur = cur->next;
    }
    return 0;
}

POSTS *post_finder(POSTS *head_post, int id)
{
    POSTS *cur = head_post->next;
    while (cur)
    {
        if (cur->id == id)
        {
            return cur;
        }
        cur = cur->next;
    }
    return cur;
}

int string_to_int(char *string)
{
    int num = 0;
    for (int i = 0; string[i] != '\0'; i++)
    {
        num = num * 10 + string[i] - 48;
    }
    return num;
}

void post_deleter(POSTS *posts_head, int id)
{
    POSTS *cur = posts_head;
    while (cur->next)
    {
        if (cur->next->id == id)
        {
            POSTS *temp = cur->next;
            cur->next = cur->next->next; /*cut the linked list*/
            free(temp);
            printf("\nyou succesfuly deleted the post with id: %d\n", id);
            return;
        }

        cur = cur->next;
    }
    printf("\nthere is no post with id: %d\n", id);
    return;
}

void accounts_file_writer(USER *head)
{
    int posts_count = 0;
    FILE *accounts = fopen("accounts.txt", "w");
    if (!accounts)
    {
        printf("unable to open the accounts file");
        return;
    }

    USER *cur = head->next_user;
    while (cur)
    {
        posts_count = 0;
        POSTS *curpost = cur->posts_head->next;
        while (curpost)
        {
            posts_count++;
            curpost = curpost->next;
        }
        fprintf(accounts, "\n%s %s %d\n", cur->username, cur->password, posts_count);
        cur = cur->next_user;
    }

    fclose(accounts);
    return;
}

void posts_file_writer(USER *head)
{
    USER *cur = head->next_user;
    FILE *posts = fopen("posts.txt", "w"); /*opening the file*/
    if (!posts)
    {
        printf("unable to open the posts file");
        return;
    }

    while (cur) /*crawling the users linked list*/
    {
        POSTS *curpost = cur->posts_head->next;
        while (curpost)
        {
            fprintf(posts, "\n");
            for (int i = 0; curpost->post_text[i] != NULL; i++)
            {
                if (curpost->post_text[i][0] == ' ')
                {
                    fprintf(posts, " ");
                }
                else
                {
                    fprintf(posts, "%s ", curpost->post_text[i]);
                }
            }
            fprintf(posts, "%s %d\n", cur->username, curpost->likes);
            curpost = curpost->next; /*going to the next post*/
        }
        cur = cur->next_user; /*moving forward*/
    }
    fclose(posts);
}

void input_catcher(char **input)
{
    int counter = 0, flag = 1;
    while (flag)
    {
        if (counter == 0)
        {
            input[0] = (char *)malloc(sizeof(char)); /*this might cause a program crash*/
            if (!input[0])
            {
                printf("malloc err inp 18");
                return;
            }
        }
        else
        {
            input[0] = (char *)realloc(input[0], sizeof(char) * (counter + 1)); /*realocation so we can build new memory elements*/
            if (!input[0])
            {
                printf("malloc err inp 27");
                return;
            }
        }
        input[0][counter] = getchar(); /*empting stdin*/
        if (input[0][counter] == '\n')
        {
            input[0][counter] = '\0';
            break;
        }
        counter++;
    }
    fflush(stdin);
    return;
}

void decoder(char *input, char all_words_1[MAX_LEN][MAX_LEN])
{
    char space_elements[MAX_LEN];
    for (int i = 0; i < MAX_LEN; i++)
    {
        NULL_initializer(all_words_1[i]);
    }
    NULL_initializer(space_elements); /*initing the two trash arrays with null*/
    space_preciser(input, space_elements);
    words_recognizer(input, space_elements, all_words_1);
}

void space_preciser(char *input, char space_elements[256])
{
    int i = 0, j = 0;
    while (input[i] != '\0')
    {
        if (input[i] == ' ')
        {
            space_elements[j] = i + 1; /*the number of space elements*/
            j++;
        }
        i++;
    }
    space_elements[j] = i + 1;
}

void words_recognizer(char *input, char *space_elements, char all_words_2[MAX_LEN][MAX_LEN])
{
    int i, word_length;
    for (i = 0; space_elements[i] != '\0'; i++)
    {
        if (i == 0)
        {
            word_length = space_elements[i] - 1; /*finding the word length*/
            if (!word_length)
            {
                all_words_2[i][0] = ' ';
                all_words_2[i][1] = '\0';
                continue;
            }
            copier(all_words_2[i], input, word_length); /*copying the word in allwords array*/
        }
        else
        {
            word_length = space_elements[i] - space_elements[i - 1] - 1; /*finding the word length*/
            if (!word_length)
            {
                all_words_2[i][0] = ' ';
                all_words_2[i][1] = '\0';
                continue;
            }

            copier(all_words_2[i], input + space_elements[i - 1], word_length); /*copying the word in allwords array*/
        }
    }
    return;
}

void copier(char *dest, char *head, int size)
{
    for (int i = 0; i < size; i++)
    {
        dest[i] = head[i];
    }
    dest[size] = '\0';
    return;
}

void NULL_initializer(char *array)
{
    for (int i = 0; i < 256; i++)
    {
        array[i] = '\0';
    }
}