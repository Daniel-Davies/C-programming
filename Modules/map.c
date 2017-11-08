#include "map.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

//decleration of a node- any type of value, and keys of type int
struct node {
  struct node *left;
  struct node *right;
  int key;
  char val[];
};

typedef struct node node;

//definition of a compare function to pass into the newMap function (used in testing)
int compare(int k1, int k2) {
  if (k1 > k2) {return 1;}
  else if (k2 > k1) {return -1;}
  else {return 0;}
}

//the type of some action function, passed to the iterate function (to be used in testing)
void act(int key, void *val, void *data) {
  int *value = val;
  int k = *value;
  int *input = data;
  int d = *input;

  int newVal = d + k;
  int *ptr = &newVal;

  memcpy(val, ptr, 10);
}

//definition of the map structure (tree)
struct map {
  struct node *root;
  bool isEmpty;
  int sizeOfItems;
  comparison *compare;
};

//function to create a new node
node *CreateNewNode(int size) {
  node *new = malloc(sizeof(node) + size);
  new->left = NULL;
  new->right = NULL;
  return new;
}

//function to set up a new (empty) map
map *newMap(comparison *compare, int size) {
  map *m = malloc(sizeof(map));
  m->root = CreateNewNode(size);
  m->isEmpty = true;
  m->compare = compare;
  m->sizeOfItems = size;
  return m;
}

//function to insert a node into a tree recursively
node *insertNode(map *m, node *p, int key, void *value) {
  if (p == NULL || (m->compare(p->key, key) == 0)) {
    p = CreateNewNode(m->sizeOfItems);
    memcpy(p->val, value, m->sizeOfItems);
    p->key = key;
  }
  else if (m->compare(p->key, key) < 0) {
    p->left = insertNode(m, p->left, key, value);
  }
  else if (m->compare(p->key, key) > 0) {
    p->right = insertNode(m, p->right, key, value);
  }
  return p;
}

//function that deals with putting a node into a tree on user request
void put(map *m, int key, void *value) {
 if (m->isEmpty) {
   memcpy(m->root->val, value, m->sizeOfItems);
   m->root->key = key;
   m->isEmpty = false;
 }
 else insertNode(m, m->root, key, value);
}

//function to look up a value given a key in a tree
node *lookupNode(map *m, node *p, int key) {
 if (p == NULL) {return NULL;}
 else if (m->compare(p->key, key) < 0) {p = lookupNode(m, p->left, key);}
 else if (m->compare(p->key, key) > 0) {p = lookupNode(m, p->right, key);}
 return p;
}

//function to deal with a look up of a value given a key by the user
void *lookup(map *m, int key) {
  if (m->root == NULL) {return NULL;}
  else {
    node *found = lookupNode(m, m->root, key);
    if (found == NULL) {
      return NULL;
    } else {
      return (found->val);
    }
  }
  return NULL;
}

//function that iterates through a tree and applies the "act" function to each node
void goThrough(node *n, action *act, void *data) {
  if(n != NULL){
    act(n->key, n->val, data);
    goThrough(n->left, act, data);
    goThrough(n->right, act, data);
   }
}

//function that the user can call to go through the tree, applying act to each element
void iterate(map *m, action *act, void *data) {
  if(m->isEmpty) {printf("Can't traverse empty map!");} else {
    goThrough(m->root, act, data);
   }
}

//------------------------------Autotesting---------------------------------

//set up a generic comparison function
enum type { CHAR, INT, BOOL };

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


int testing() {
  int n = 0;
  map *mp = newMap(compare, sizeof(int));
  map *chrMp = newMap(compare, sizeof(char));

  //tests 1-4- test initialisation of map
  n = CheckEqPtrs(mp->root->left, NULL, n);
  n = CheckEqPtrs(mp->root->right, NULL, n);
  n = CheckEqInt(mp->sizeOfItems, sizeof(int), n);
  n = CheckEqBool(mp->isEmpty, true, n);

  int val = 67;
  int *s = &val;
  put(mp, 1, s); val = 43; s = &val;
  put(mp, -1, s); val = 100; s = &val;
  put(mp, 2, s); val = 12; s = &val;
  put(mp, 7, s); val = 15; s = &val;
  put(mp, 0, s);
  void *result = lookup(mp, 1);
  int *item = result;
  int ans = *item;

  //tests 5-10- test the put function and the lookup function (on ints)
  n = CheckEqBool(mp->isEmpty, false, n);
  n = CheckEqInt(ans, 67, n); result = lookup(mp, 2); item = result; ans = *item;
  n = CheckEqInt(ans, 100, n); result = lookup(mp, 7); item = result; ans = *item;
  n = CheckEqInt(ans, 12, n); result = lookup(mp, -1); item = result; ans = *item;
  n = CheckEqInt(ans, 43, n); result = lookup(mp, 0); item = result; ans = *item;
  n = CheckEqInt(ans, 15, n);

  result = lookup(mp, 50);

  //test 11- make sure that lookup returns NULL if a key doesn't exist
  n = CheckEqPtrs(result, NULL, n);

  char value = 'a';
  char *l = &value;
  put(chrMp, 12, l); value = '['; l = &value;
  put(chrMp, 5, l); value = 'x'; l = &value;
  put(chrMp, 0, l); value = '.'; l = &value;
  put(chrMp, 1, l);

  result = lookup(chrMp, 0);
  char *character = result;
  char data = *character;

  //tests 12-15- test the put function and the lookup function (on chars)
  n = CheckEqChar(data, 'x', n); result = lookup(chrMp, 12); character = result; data = *character;
  n = CheckEqChar(data, 'a', n); result = lookup(chrMp, 5); character = result; data = *character;
  n = CheckEqChar(data, '[', n); result = lookup(chrMp, 1); character = result; data = *character;
  n = CheckEqChar(data, '.', n);

  int passData = 5; //we add 5 to each element in the tree

  iterate(mp, *act, &passData);

  result = lookup(mp, 1);
  item = result;
  ans = *item;

  //tests 16-20- test the iterate function
  n = CheckEqInt(ans, 72, n); result = lookup(mp, 2); item = result; ans = *item;
  n = CheckEqInt(ans, 105, n); result = lookup(mp, 7); item = result; ans = *item;
  n = CheckEqInt(ans, 17, n); result = lookup(mp, -1); item = result; ans = *item;
  n = CheckEqInt(ans, 48, n); result = lookup(mp, 0); item = result; ans = *item;
  n = CheckEqInt(ans, 20, n);

  return n;
}
