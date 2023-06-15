#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct node
{
  char *japanese;
  char *english;
  char *reading;
  // char * for just a pointer, all words vary in size
  // TODO adjust sizes later
  struct node *next;
}
node;

const unsigned int N = 100;
// preload functions
unsigned int hash(const char *word);
bool load_with_ruby(char *japanese, char *english, char *reading);
void free_them_all(node *current);
bool unload(void);
void read_csv_with_c(void);
char *check_english(char *word);


node *table[N];
int main(void)
{
  read_csv_with_c();
  check_english("明かん");
  unload();
}

void read_csv_with_c(void) {
  char buffer[250];
    FILE* file = fopen("dictionary.csv", "r");
    if (file == NULL) {
        printf("Error opening file: dictionary.csv");
        return;
    }
    enum { NORMAL, QUOTED } state = NORMAL;
    enum { JAPANESE, ENGLISH, READING} word_type = JAPANESE;
    int c;
    int index = 0;
    int table_index;
    node *current;
    while ((c = fgetc(file)) != EOF) {
        switch (state) {
            case NORMAL:
                if (c == '"') {
                    state = QUOTED;
                } else if (c == ',') {
                    // Handle comma in a normal field
                    buffer[index] = 0;
                    if (word_type == JAPANESE)
                    {
                      table_index = hash(buffer);
                      printf("Table index now: %i\n", table_index);
                      if (table[table_index] == NULL)
                      {
                        printf("Making the node\n");
                        node *first_node = malloc(sizeof(struct node));
                        first_node->japanese = NULL;
                        first_node->english = NULL;
                        first_node->reading = NULL;
                        first_node->next = NULL;
                        printf("Set all parts of new node to NULL\n");
                        table[table_index] = first_node;
                        current = table[table_index];
                      }
                      else
                      {
                          printf("table has existing index\n");
                          current = table[table_index];
                          // Bug occurs somewhere around here
                          while ( current->next != NULL)
                          {
                            printf("jumping through nodes");
                            current = current->next;
                          }
                          printf("Allocating the memory");
                          node *new = malloc(sizeof(struct node));
                          new->japanese = NULL;
                          new->english = NULL;
                          new->reading = NULL;
                          new->next = NULL;
                          current->next = new;
                          current = current->next;
                      }
                        printf("creating string\n");
                        char string_tmp[strlen(buffer)];
                        strcpy(string_tmp, buffer);
                        printf("Malloc for string\n");
                        current->japanese = malloc(sizeof(string_tmp));
                        printf("copying\n");
                        strcpy(current->japanese, string_tmp);
                        index = 0;
                        printf("Current Japanese word in node current: %s\n", current->japanese);
                        printf("Moving to next word\n");
                        printf("Switching to English\n");
                        word_type = ENGLISH;
                    }
                    else if (word_type == ENGLISH)
                    {
                      buffer[index] = 0;
                      char string_tmp[strlen(buffer)];
                      strcpy(string_tmp, buffer);
                      current->english = malloc(sizeof(string_tmp));
                      strcpy(current->english, string_tmp);
                      index = 0;
                      printf("Current english word in node current: %s\n", current->english);
                      printf("Switching to READING\n");
                      word_type = READING;
                    }
                     else if (word_type == READING)
                    {
                      buffer[index] = 0;
                      char string_tmp[strlen(buffer)];
                      strcpy(string_tmp, buffer);
                      current->reading = malloc(sizeof(string_tmp));
                      strcpy(current->reading, string_tmp);
                      index = 0;
                      printf("Switching to Japanese\n");
                      word_type = JAPANESE;
                    }
                    printf("Skipping comma\n");
                } else if (c == '\n') {
                    // Handle end of line
                    buffer[index] = 0;
                    char string_tmp[strlen(buffer)];
                    strcpy(string_tmp, buffer);
                    current->reading = malloc(sizeof(string_tmp));
                    strcpy(current->reading, string_tmp);
                    index = 0;
                    printf("Switching to Japanese\n");
                    word_type = JAPANESE;
                    printf("End of line\n");
                } else {
                    // Handle other characters in a normal field
                    buffer[index] = c;
                    index += 1;
                    printf("Character: %c\n", c);
                }
                break;

            case QUOTED:
                if (c == '"') {
                    state = NORMAL;
                } else {
                    // Handle characters inside quotes
                    buffer[index] = c;
                    index += 1;
                    printf("Character inside quotes: %c\n", c);
                }
                break;
        }
    }
    fclose(file);
}
// Two ways to load, I read the csv file with c, or read the xml file with ruby and pass values to c.
// I will try both and see which is faster
// IMPORTANT This code, loading piece by piece with ruby, eventually gives memory and heap corruption errors. Logs point to something with the eiwa gem and nokogiri. To get around this, just have it so the dictionary can be loaded by c from a seperate file. Either directly xml, or seperately csv
bool load_with_ruby(char *japanese, char *english, char *reading)
{
  int index = hash(japanese);
  node* current;
  printf("This is null %p\n", NULL);
  if (table[index] == NULL)
  {
    printf("table index is null\n");
    node *first_node = malloc(sizeof(struct node));
    first_node->japanese = NULL;
    first_node->english = NULL;
    first_node->reading = NULL;
    first_node->next = NULL;
    table[index] = first_node;
    current = table[index];
  }
  else
  {
    printf("table has existing index\n");
    current = table[index];
    // Bug occurs somewhere around here
    while ( current->next != NULL)
    {
      printf("jumping through nodes");
      current = current->next;
    }
    printf("Allocating the memory");
    node *new = malloc(sizeof(struct node));
    new->japanese = NULL;
    new->english = NULL;
    new->reading = NULL;
    new->next = NULL;
    current->next = new;
    current = current->next;
  }
  // create dynamic memory for every string
  printf("copying strings");
  current->japanese = malloc(sizeof(japanese));
  strcpy(current->japanese, japanese);
  current->english = malloc(sizeof(english));
  strcpy(current->english, english);
  current->reading = malloc(sizeof(reading));
  strcpy(current->reading, reading);
  return true;
}

unsigned int hash(const char *word)
{
    // printf("Hashing!\n");
    int length = strlen(word);
    // the '\'' allows for ' values to be valid
    unsigned long hash = word[0] - '\'';
    // printf("%li\n", hash);
    int i;
    // iterate through word
    int letter;
    for (i = 1; i < length; i++)
    {
      letter = word[i] - '\'';
        // shift bits, add the value of the letter, add the i value to sepearate everything
      hash = ((hash << 5) + letter) + i;
    }
    unsigned int final_value = hash % 100;
    // I realized shifting bits would be good after seeing the djb2 from dan bernstein, that was the inspiration for this
    // This was better than my initial idea to primarily use i multplied by letters and then divided to make sure all values were different
    // this allows for a wider range, and then simply adding i and the letter makes it so that it's different than a word of the same size and letters but in a differnt order
    return final_value;
}
char *check_english(char *word)
{
  // this accepts japanese word, and finds english
  int index = hash(word);
  node *current = table[index];
  if (current == NULL)
  {
    printf("FAILURE");
    return "FAILURE";
  }
  else if (strcmp(current->japanese, word) == 0)
  {
    printf("Found the english word! %s\n", current->english);
    return current->english;
  }
  while (current->next != NULL)
  {
    if (strcmp(current->japanese, word) == 0)
        {
            printf("Found the english word! %s\n", current->english);
            return current->english;
        }
        else
        {
            current = current->next;
        }
  }
  printf("FAILURE");
  return "FAILURE";

}

bool unload(void)
{
  // Change this to more efficient once the dicitonary is bigger, don't iterate over entire table at that point
  int i;
  for (i = 0; i < N; i++)
  {
    node *current;
    if (table[i] != NULL)
    {
      current = table[i];
      free_them_all(current);
    }
  }
  return true;
}

void free_them_all(node *current)
{
    if (current->next != NULL)
    {
        free_them_all(current->next);
    }
    // printf("Freeing %s\n", current->word);
    if (current->japanese != NULL)
    {
      printf("freeing %s\n", current->japanese);
      free(current->japanese);
    }
    if (current->english != NULL)
    {
      printf("freeing %s\n", current->english);
      free(current->english);
    }
    if (current->reading != NULL)
    {
      printf("freeing %s\n", current->reading);
      free(current->reading);
    }
    free(current);
}
