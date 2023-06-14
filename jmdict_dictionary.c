#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct node
{
  char japanese[47];
  char english[47];
  char reading[47];
  // 47 because the longest english word is 46
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


node *table[N];
int main(void)
{

}

// Two ways to load, I read the csv file with c, or read the xml file with ruby and pass values to c.
// I will try both and see which is faster
bool load_with_ruby(char *japanese, char *english, char *reading)
{
  int index = hash(japanese);
  node* current;
  if (table[index] == NULL)
  {
    node *first_node = malloc(sizeof(struct node));
    first_node->japanese[0] = 0;
    first_node->next = NULL;
    table[index] = first_node;
    current = table[index];
  }
  else
  {
    current = table[index];
    while ( current->next != NULL)
    {
      current = current->next;
    }
    node *new = malloc(sizeof(struct node));
    current->next = new;
    current = current->next;
  }
  strcpy(current->japanese, japanese);
  strcpy(current->english, english);
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
    return "FAILURE";
  }
  else if (strcmp(current->japanese, word) == 0)
  {
    return current->english;
  }
  while (current->next != NULL)
  {
    if (strcmp(current->japanese, word) == 0)
        {
            return current->english;
        }
        else
        {
            current = current->next;
        }
  }
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
    free(current);
}
