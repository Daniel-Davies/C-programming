#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

struct person {
  char *name;
  char *number;
  int age;
};

typedef struct person person;

float AvAge(int n, int ages[n]) {
  float total = 0;
  float average = 0.0;
  float number = n;
  for (int i = 0; i < n; i++) {
    total = total + ages[i];
  }
  average = (total / number);
  return average;
}

void traversePers(list *pers) {
  person getFrom;
  person *p = &getFrom;
  int *ages = malloc(50 * sizeof(int));
  int capacity = 50;
  int n = 0;
  start(pers);
    while (! atEnd(pers)) {
      getAfter(pers, p);
      ages[n] = p->age;
      n++;
      printf("Name: %s,  Age: %d,  Number: %s,\n", p->name, p->age, p->number);
      forward(pers);
      if (n == capacity) {
        capacity = capacity * (3/2);
        int *ages = realloc(ages, capacity * sizeof(int));
      }
    }
    printf("Average age of people = %.3f\n", AvAge(n, ages));
    printf("\n");
}

void run() {
  list *pers = newList(sizeof(person));
  person x;

  x.age = 30;
  x.name = "Bob";
  x.number = "01792884373";

  insertBefore(pers, &x); x.age = 40; x.name = "Anne"; x.number = "0274929327";
  insertBefore(pers, &x); x.age = 50; x.name = "Daniel"; x.number = "29499392";
  insertBefore(pers, &x);

  person changeVar;
  changeVar = x;
  changeVar.age = 18;

  start(pers);
  setAfter(pers, &changeVar);

  traversePers(pers);

  end(pers);
  deleteBefore(pers);

  traversePers(pers);
}

int main() {
  run();
  printf("------------------------------Autotesting-------------------------------\n");
  int k = testLists();
  if (k == 60) {
    printf("All 60 tests successful!\n");
  } else {printf("Test %d fails!\n", k);}

  return 0;
}
