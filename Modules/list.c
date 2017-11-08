#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

//structure for each node of list
struct node {
  struct node *previous;
  struct node *next;
  char item[];
};
typedef struct node node;

//structure of list
struct list {
    node *first;
    node *last;
    node *current;
    int sizeOfItems; //holds the value of the user input, 'b', which is size of item
    bool emptyList; //need to know if list is empty for certain comparisons
};

typedef struct list list;

//generic error print function
static void fail(char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
    exit(1);
}

//function to create and initialise a new node
node *CreateNewNode(list *l) {
  node *new = malloc(sizeof(node) + l->sizeOfItems);
  new->next = NULL;
  new->previous = NULL;
  return new;
}

//function to set up a new (empty) list
list *newList(int b) {
  list *l = malloc(sizeof(list));
  l->sizeOfItems = b;
  l->first = CreateNewNode(l);
  l->last = CreateNewNode(l);
  l->first->next = l->last;
  l->last->previous = l->first;
  l->current = l->last; //set the pointer to the node after the gap
  l->emptyList = true;
  return l;
}

//set the current item to the first item (so that the user thinks they are in the gap
//between sentinel and first nodes)
void start(list *l) {
  l->current = l->first->next;
}

//set the current item to the sentinel node at the end (user thinks they are
//in the gap between the last item and sentinel)
void end(list *l) {
  l->current = l->last;
}

//function to check if user is at the start of the list
bool atStart(list *l) {
  return (l->current == l->first->next);
}

//function to check if user is at the end of the list
bool atEnd(list *l) {
  return (l->current == l->last);
}

//function to progress (by one) through the list, unless already at the end
void forward(list *l) {
  if (atEnd(l)) {fail("Forward on list failed- can't go beyond the end of the list");}
  else {l->current = l->current->next;}
}

//function to progress (by negative one) through the list, unless they are already at the start
void backward(list *l) {
  if (atStart(l)) {fail("Backward on list failed- can't go beyond the start of the list");}
  else {l->current = l->current->previous;}
}

//function that inserts an item into the list (and keeps the current
//pointer where it is)
void insertAfter(list *l, void *p) {
  node *newNode = CreateNewNode(l);
  memcpy(newNode->item, p, l->sizeOfItems);
  node *prevNode = l->current->previous;
  node *currNode = l->current;
  newNode->previous = prevNode;
  newNode->next = currNode;
  prevNode->next = newNode;
  l->current->previous = newNode;
}

//function that inserts an item into the list (and moves the current
//pointer to the new node, giving the impression of inserting before)
void insertBefore(list *l, void *p) {
  insertAfter(l, p);
  backward(l);
}

//another insert function to insert an item after the current node
void insertAfterAfter(list *l, void *p) {
  node *newNode = CreateNewNode(l);
  if (l->last->previous != l->current && !(atEnd(l))) {
    node *nextNode = l->current->next;
    node *currNode = l->current;
    newNode->next = nextNode;
    newNode->previous = currNode;
    memcpy(newNode->item, p, l->sizeOfItems);
    nextNode->previous = newNode;
    l->current->next = newNode;
  } else {fail("Request out of bound of list");}
}

//function to get the value of an item before the current position
void getBefore(list *l, void *p) {
  if (! atStart(l)) {memcpy(p, l->current->previous->item, l->sizeOfItems);}
  else {fail("Request out of bound of list");}
}

//function to get the value of an item at the current position
void getAfter(list *l, void *p) {
  if (atEnd(l)) {fail("Request out of bound of list");} else {memcpy(p, l->current->item, l->sizeOfItems);}
}

//function to set the value of an item before the current position
void setBefore(list *l, void *p) {
  if (! atStart(l)) {memcpy(l->current->previous->item, p, l->sizeOfItems);}
  else {fail("Request out of bound of list");}
}

//function to set the value of an item at the current position
void setAfter(list *l, void *p) {
  if (atEnd(l)) {fail("Request out of bound of list");} else {memcpy(l->current->item, p, l->sizeOfItems);}
}

//function to delete the value of an item before the current position
void deleteBefore(list *l) {
  node *prev;
  node *prevPrev;
  if (atStart(l)) {fail("Can't delete item before the start of the list");}
  else {
    prev = l->current->previous;
    prevPrev = prev->previous;
    prevPrev->next = l->current;
    l->current->previous = prevPrev;
    free(prev);
  }
}

//function to delete the value of an item at the current position
void deleteAfter(list *l) {
  node *next;// = CreateNewNode(l);
  node *prevNode;// = CreateNewNode(l);
  if (! atEnd(l)) {
    next = l->current->next;
    prevNode = l->current->previous;
    prevNode->next = l->current->next;
    next->previous = prevNode;
    free(l->current);
    l->current = next;
  } else {fail("Can't delete item after the end of the list");}
}

//part of the testing module- traverses a list (int only)
static void traverse(list *l) {
  int p;
  start(l);
    while (! atEnd(l)) {
      getAfter(l, &p);
      printf("%d ", p);
      forward(l);
    }
    printf("\n");
}

//part of the testing module- traverses a list backwards (int only)
static void traverseBack(list *l) {
  int p;
  end(l);
    while (! atStart(l)) {
      getBefore(l, &p);
      printf("%d ", p);
      backward(l);
    }
    printf("\n");
}

enum type { CHAR, INT, BOOL };

// Check that two ints, chars or bools are equal
int CheckEq(enum type t, int x, int y, int n) {
    n++;
    if (x != y) {
        if (t==CHAR) fprintf(stderr, "Test %d gives %c not %c", n, x, y);
        if (t==INT) fprintf(stderr, "Test %d gives %d not %d", n, x, y);
        if (t==BOOL && x) fprintf(stderr, "Test %d gives true not false", n);
        if (t==BOOL && y) fprintf(stderr, "Test %d gives false not true", n);
        exit(1);
    }
    return n;
}

//check if 2 pointers are equal
int CheckEqPtrs(node *x, node *y, int n) {
    n++;
    if (x != y) {
        fprintf(stderr, "Test %d shows a mismatch in pointers!\n", n);
        n--;
        exit(1);
    }
    return n;
}

// More specific versions of the eq function
int CheckEqChar(char x, char y, int n) { return CheckEq(CHAR, x, y, n); }
int CheckEqInt(int x, int y, int n) { return CheckEq(INT, x, y, n); }
int CheckEqBool(bool x, bool y, int n) { return CheckEq(BOOL, x, y, n); }

int testLists() {
  int n = 0;
  int TestItem = 42;
  char x = 'a';
  float testFloat = 5.5;
  //initialise 3 different kinds of lists
  list *intList = newList(sizeof(int));
  list *floatList = newList(sizeof(float));
  list *charList = newList(sizeof(char));

  //tests 1-6- check list initialisations
  n = CheckEqPtrs(intList->first->previous, NULL, n);
  n = CheckEqPtrs(floatList->last->next, NULL, n);
  n = CheckEqPtrs(charList->current->next, NULL, n);
  n = CheckEqPtrs(charList->current->previous, charList->first, n);
  n = CheckEqInt(floatList->sizeOfItems, sizeof(float), n);
  n = CheckEqInt(intList->sizeOfItems, sizeof(int), n);

  insertBefore(intList, &TestItem); TestItem = 10;
  insertAfter(intList, &TestItem);
  insertBefore(charList, &x);

  //tests 7-30- test the insert functions, forward function, and getAfter function
  intList->current = intList->first->next;
  getAfter(intList, &TestItem);
  n = CheckEqInt(TestItem, 10, n); forward(intList); getAfter(intList, &TestItem);
  n = CheckEqInt(TestItem, 42, n);
  n = CheckEqPtrs(intList->current->next, intList->last, n);
  n = CheckEqPtrs(intList->current->previous, intList->first->next, n);
  x = 'i'; getAfter(charList, &x); charList->current = charList->first->next;
  n = CheckEqChar(x, 'a', n);

  insertAfter(floatList, &testFloat);

  n = CheckEqPtrs(floatList->current->previous->previous, floatList->first, n);
  n = CheckEqPtrs(floatList->current->next, NULL, n);
  n = CheckEqPtrs(floatList->first->next, floatList->current->previous, n);
  n = CheckEqPtrs(floatList->last->previous, floatList->first->next, n);

  intList = newList(sizeof(int)); TestItem = 1000;

  insertBefore(intList, &TestItem); TestItem = 100;
  insertBefore(intList, &TestItem); TestItem = 200;
  insertBefore(intList, &TestItem); TestItem = 300;
  insertBefore(intList, &TestItem); TestItem = 400;
  insertBefore(intList, &TestItem); TestItem = 500;
  insertAfter(intList, &TestItem); TestItem = 600;
  insertAfter(intList, &TestItem); TestItem = 700;
  insertAfter(intList, &TestItem); TestItem = 800;
  insertAfter(intList, &TestItem); TestItem = 900;
  insertAfter(intList, &TestItem);

  intList->current = intList->first->next;
  getAfter(intList, &TestItem);
  n = CheckEqInt(TestItem, 500, n); forward(intList); getAfter(intList, &TestItem);
  n = CheckEqInt(TestItem, 600, n); forward(intList); getAfter(intList, &TestItem);
  n = CheckEqInt(TestItem, 700, n); forward(intList); getAfter(intList, &TestItem);
  n = CheckEqInt(TestItem, 800, n); forward(intList); getAfter(intList, &TestItem);
  n = CheckEqInt(TestItem, 900, n); forward(intList);

  TestItem = 0;
  getAfter(intList, &TestItem);

  n = CheckEqInt(TestItem, 400, n); forward(intList); getAfter(intList, &TestItem);
  n = CheckEqInt(TestItem, 300, n); forward(intList); getAfter(intList, &TestItem);
  n = CheckEqInt(TestItem, 200, n); forward(intList); getAfter(intList, &TestItem);
  n = CheckEqInt(TestItem, 100, n); forward(intList); getAfter(intList, &TestItem);
  n = CheckEqInt(TestItem, 1000, n);

  //tests 26-30 the start  and end function
  start(intList);
  n = CheckEqPtrs(intList->current->previous, intList->first, n); getAfter(intList, &TestItem);
  n = CheckEqInt(TestItem, 500, n);

  end(intList);

  n = CheckEqPtrs(intList->current, intList->last, n); backward(intList); getAfter(intList, &TestItem);
  n = CheckEqInt(TestItem, 1000, n); backward(intList); getAfter(intList, &TestItem);
  n = CheckEqInt(TestItem, 100, n);

  //tests 31-35- test the getBefore and backward functions
  end(intList); getBefore(intList, &TestItem); backward(intList);

  n = CheckEqInt(TestItem, 1000, n); getBefore(intList, &TestItem); backward(intList);
  n = CheckEqInt(TestItem, 100, n); getBefore(intList, &TestItem); backward(intList);
  n = CheckEqInt(TestItem, 200, n); getBefore(intList, &TestItem); backward(intList);
  n = CheckEqInt(TestItem, 300, n); getBefore(intList, &TestItem); backward(intList);
  n = CheckEqInt(TestItem, 400, n);

  start(intList);
  //tests 36- 38- test the atStart function
  n = CheckEqBool(atStart(intList), true, n); forward(intList);
  n = CheckEqBool(atStart(intList), false, n); end(intList);
  n = CheckEqBool(atStart(intList), false, n);

  end(intList);
  //tests 39 - 41- test the atEnd function
  n = CheckEqBool(atEnd(intList), true, n); backward(intList);
  n = CheckEqBool(atEnd(intList), false, n); start(intList);
  n = CheckEqBool(atEnd(intList), false, n);

  intList = newList(sizeof(int));
  //tests 42-43- test the atStart and atEnd functions on the empty list
  n = CheckEqBool(atEnd(intList), true, n);
  n = CheckEqBool(atStart(intList), true, n);

  insertBefore(intList, &TestItem); TestItem = 100;
  insertBefore(intList, &TestItem); TestItem = 200;
  insertBefore(intList, &TestItem); TestItem = 300;
  insertBefore(intList, &TestItem); TestItem = 400;
  insertBefore(intList, &TestItem); TestItem = 500;
  insertAfter(intList, &TestItem); TestItem = 600;
  insertAfter(intList, &TestItem); TestItem = 700;
  insertAfter(intList, &TestItem); TestItem = 800;
  insertAfter(intList, &TestItem); TestItem = 900;
  insertAfter(intList, &TestItem); TestItem = 55;

  //tests 44-47- test the setAfter function
  start(intList); setAfter(intList, &TestItem); TestItem = 70;
  forward(intList); setAfter(intList, &TestItem); TestItem = 15;
  end(intList); backward(intList); setAfter(intList, &TestItem); TestItem = 95;
  backward(intList); setAfter(intList, &TestItem);

  TestItem = 0;

  start(intList); getAfter(intList, &TestItem);

  n = CheckEqInt(TestItem, 55, n); forward(intList); getAfter(intList, &TestItem);
  n = CheckEqInt(TestItem, 70, n); end(intList); backward(intList); getAfter(intList, &TestItem);
  n = CheckEqInt(TestItem, 15, n); backward(intList); getAfter(intList, &TestItem);
  n = CheckEqInt(TestItem, 95, n);

  TestItem = -10;
  //tests 48-51- test the setBefore function
  start(intList); forward(intList); setBefore(intList, &TestItem); TestItem = -20;
  forward(intList); setBefore(intList, &TestItem); TestItem = -30;
  end(intList); setBefore(intList, &TestItem); TestItem = -40;
  backward(intList); setBefore(intList, &TestItem);

  TestItem = 0;

  start(intList); getAfter(intList, &TestItem);

  n = CheckEqInt(TestItem, -10, n); forward(intList); getAfter(intList, &TestItem);
  n = CheckEqInt(TestItem, -20, n); end(intList); getBefore(intList, &TestItem);
  n = CheckEqInt(TestItem,-30, n); backward(intList); getBefore(intList, &TestItem);
  n = CheckEqInt(TestItem, -40, n);

  //tests 52-55- test the deleteAfter function
  TestItem = 0;
  start(intList);
  deleteAfter(intList);
  getAfter(intList, &TestItem);

  n = CheckEqInt(TestItem, -20, n); forward(intList); getAfter(intList, &TestItem);
  n = CheckEqInt(TestItem, 700, n);

  end(intList); backward(intList);
  deleteAfter(intList); backward(intList);
  getAfter(intList, &TestItem);

  n = CheckEqInt(TestItem, -40, n); backward(intList); getAfter(intList, &TestItem);
  n = CheckEqInt(TestItem, 200, n);

  //tests 56-60- test the deleteBefore function
  start(intList);
  forward(intList);
  int currNodeValue = 0; getAfter(intList, &currNodeValue);
  deleteBefore(intList);
  getAfter(intList, &TestItem);

  n = CheckEqInt(TestItem, currNodeValue, n); //current item doesn't change

  n = CheckEqInt(TestItem, 700, n); forward(intList); getAfter(intList, &TestItem);
  n = CheckEqInt(TestItem, 800, n);

  end(intList);
  deleteBefore(intList); backward(intList); getAfter(intList, &TestItem);

  n = CheckEqInt(TestItem, 200, n); backward(intList); getAfter(intList, &TestItem);
  n = CheckEqInt(TestItem, 300, n);

  //traverse the remaining list from both ends
  printf("Traversal forwards: ");
  traverse(intList);
  printf("Traversal backwards: ");
  traverseBack(intList);

  return n;
}
