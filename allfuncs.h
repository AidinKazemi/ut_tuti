#ifndef ALLFUNCS
#define ALLFUNCS
#define MALOC_ERROR NULL
#define PROGRAM_CONTINUES 1
#define PROGRAM_ENDS 0
#define MAX_LEN 256

typedef struct user USER;
typedef struct posts POSTS;
typedef struct liked_posts LIKED_POSTS;
struct liked_posts
{
    char *user;
    int id;
    LIKED_POSTS *next;
};

struct posts
{
    char **post_text;
    int likes;
    int id;
    struct posts *next;
};

struct user
{
    char *username;
    char *password;
    struct posts *posts_head;
    struct user *next_user;
    LIKED_POSTS *liked_posts_head;
};

int word_length_finder(char *word);
void node_adder(USER *prev, USER *new_node); /*addes node to a certain point in array*/
int words_count_finder(char array[MAX_LEN][MAX_LEN]);
int post_text_copier(char **dest, char start[MAX_LEN][MAX_LEN], int words_count);
void node_post_adder(POSTS *prev, POSTS *new_node);
USER *user_finder(USER *head, char *name);
void post_text_printer(char **post_text);
void all_posts_printer(POSTS *head, char *username);
void node_likedposts_adder(LIKED_POSTS *prev, LIKED_POSTS *new_node);
POSTS *post_finder(POSTS *head_post, int id);
int string_to_int(char *string);
int liked_post_checker(LIKED_POSTS *liked_posts_head, char all_words[MAX_LEN][MAX_LEN]);
void post_deleter(POSTS *posts_head, int id);
void copier(char *dest, char *head, int size);
void accounts_file_writer(USER *head);
void posts_file_writer(USER *head);
int recognizer(char all_words[MAX_LEN][MAX_LEN], USER *current_user, USER *head);
void input_catcher(char **input);
void NULL_initializer(char *array);
void space_preciser(char *input, char space_element[MAX_LEN]);
void words_recognizer(char *input, char *space_elements, char all_words_2[MAX_LEN][MAX_LEN]);
void copier(char *dest, char *head, int size);
void decoder(char *input, char all_words_1[MAX_LEN][MAX_LEN]);
#endif