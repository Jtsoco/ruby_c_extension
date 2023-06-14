#include <stdio.h>

int foobar(int five);
int arr_hunt(void);

int ARR[5] = {1, 2, 3, 4, 5};
int main(void)
{
  foobar(5);
  arr_hunt();
  int *six = malloc(sizeof(int));
  *six = 6;

}

int foobar(int five)
  {
    return five;
  }

int arr_hunt(void)
{
  return ARR[0];
}
