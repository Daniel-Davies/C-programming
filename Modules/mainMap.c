#include "map.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

//definition of a compare function
int comparer(int k1, int k2) {
  if (k1 > k2) {return 1;}
  else if (k2 > k1) {return -1;}
  else {return 0;}
}

//main function- place an item into the map (done to test linking), and also print how many tests passed
int main() {
  //test linking of modules
  map *m = newMap(comparer, 10);
  int val = 67;
  int *s = &val;
  put(m, 1, s);
  void *result = lookup(m, 1);
  int *item = result;
  int ans = *item;
  printf("%d\n", ans);

  //print tests passed
  printf("Passed %d tests of 20\n", testing());
  return 0;
}
