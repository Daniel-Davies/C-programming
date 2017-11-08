/* Classify a triangle according to the integer lengths of its sides. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// check whether a triangle is "flat"
int flat(long a, long b, long c) {
  if (b + c == a) { //check if the sum of two  smaller sides = the longest side
    return 1; //return true
  }
  return 0; //return false
}
//check whether the triangle is possible
int valid(long a, long b, long c) {
  if (a + b > c && a + c > b && b + c > a) { //check that 2 sides are greater than the third
    return 1; //return true
  }
  return 0; //return false
}
//function to check for an isosceles triangle
int isos(long a, long b, long c) {
  if (a == b && a != c) { //check that the smallest sides are equal (but not equal to the third)
    return 1; //return true
  }
  else if (b == c && a != b) { //check that the two biggest sides are equal (but not to the third)
    return 1; //return true
  }
  return 0; //return false
}
//routine to check if is a right angled triangle
int pythag(long a, long b, long c) {
  if ((a*a) + (b*b) == (c*c)) { //check that the square of the 2 smallest sides is equal to the square of the largest
    return 1; //return true
  }
  return 0; //return false
}
//routine to sort the side lengths
int sort(long *val1, long *val2) {
  long temp = 0; //declare a temporary variable
  if (*val2 > *val1) { //check that one value is greater than the other
    temp = *val1; //swap them
    *val1 = *val2;
    *val2 = temp;
  }
  return 0;
}

char *triangle(const char *length1, const char *length2, const char *length3) {
  char s1[100]; //declare variables to hold conversion from long to string
  char s2[100];
  char s3[100];
  if (strstr(length3, ".") != NULL) { //check if either of the lengths include a "."
    return "Illegal";
  }
  if (strstr(length2, ".") != NULL) {
    return "Illegal";
  }
  if (strstr(length1, ".") != NULL) {
    return "Illegal";
  }
  long side1 = strtol(length1, NULL, 10); //converts strings to long
  long side2 = strtol(length2, NULL, 10);
  long side3 = strtol(length3, NULL, 10);
  long limit = 2147483647;

  sprintf(s2,"%ld",side2); //convert the longs to strings
  sprintf(s1,"%ld",side1);
  sprintf(s3, "%ld", side3);

  int x = strlen(s1); //get their lengths
  int y = strlen(s2);
  int z = strlen(s3);

  if (x != strlen(length1) || y != strlen(length2) || z != strlen(length3)) { //check if length from conversion has been changed
    return "Illegal";
  }
  if (side1 <= 0 || side2 <= 0 || side3 <= 0) { //check for negative values
    return "Illegal";
  }

  if ((side1 > limit) || (side2 > limit) || (side3 > limit)) { //check whether either of the sides break the overflow limit
    return "Illegal";
  }
  //sort the sides into order
  sort(&side1, &side2);
  sort(&side1, &side3);
  sort(&side2, &side3);
  //check for each triangle type
  if (side1 == side2 && side1 == side3) {
    return "Equilateral";
  }
  if (flat(side1, side2, side3) == 1) {
    return "Flat";
  }
  if (isos(side1, side2, side3) == 1) {
    return "Isosceles";
  }
  if (pythag(side2, side3, side1) == 1) {
    return "Right";
  }
  if (valid(side1, side2, side3) == 1) {
    return "Scalene";
  }
  else { //if it is not scalene or flat, it must be impossible
    return "Impossible";
  }
}
// Increment the test number and check that two strings are equal.
int eq(int n, const char *actual, const char *expected) {
    n++;
    if (strcmp(actual, expected) != 0) {
        fprintf(stderr, "Test %d fails\n", n);
        fprintf(stderr, "result: %s\n", actual);
        fprintf(stderr, "instead of: %s\n", expected);
        exit(1);
    }
    return n;
}

// Run tests on the triangle function.
void test() {
    int n = 0;

    // Tests 1 to 2: check equilateral
    n = eq(n, triangle("8", "8", "8"), "Equilateral");
    n = eq(n, triangle("1073", "1073", "1073"), "Equilateral");

    // Tests 3 to 5: check isosceles
    n = eq(n, triangle("25", "25", "27"), "Isosceles");
    n = eq(n, triangle("25", "27", "25"), "Isosceles");
    n = eq(n, triangle("27", "25", "25"), "Isosceles");

    // Tests 6 to 14: check right angled
    n = eq(n, triangle("3", "4", "5"), "Right");
    n = eq(n, triangle("3", "5", "4"), "Right");
    n = eq(n, triangle("5", "3", "4"), "Right");
    n = eq(n, triangle("5", "12", "13"), "Right");
    n = eq(n, triangle("5", "13", "12"), "Right");
    n = eq(n, triangle("12", "5", "13"), "Right");
    n = eq(n, triangle("12", "13", "5"), "Right");
    n = eq(n, triangle("13", "5", "12"), "Right");
    n = eq(n, triangle("13", "12", "5"), "Right");

    // Tests 15 to 20: check scalene
    n = eq(n, triangle("12", "14", "15"), "Scalene");
    n = eq(n, triangle("12", "15", "14"), "Scalene");
    n = eq(n, triangle("14", "12", "15"), "Scalene");
    n = eq(n, triangle("14", "15", "12"), "Scalene");
    n = eq(n, triangle("15", "12", "14"), "Scalene");
    n = eq(n, triangle("15", "14", "12"), "Scalene");

    // Tests 21 to 25: check flat
    n = eq(n, triangle("7", "9", "16"), "Flat");
    n = eq(n, triangle("7", "16", "9"), "Flat");
    n = eq(n, triangle("9", "16", "7"), "Flat");
    n = eq(n, triangle("16", "7", "9"), "Flat");
    n = eq(n, triangle("16", "9", "7"), "Flat");

    // Tests 26 to 31: check impossible
    n = eq(n, triangle("2", "3", "13"), "Impossible");
    n = eq(n, triangle("2", "13", "3"), "Impossible");
    n = eq(n, triangle("3", "2", "13"), "Impossible");
    n = eq(n, triangle("3", "13", "2"), "Impossible");
    n = eq(n, triangle("13", "2", "3"), "Impossible");
    n = eq(n, triangle("13", "3", "2"), "Impossible");

    // Tests 32 to 42: check illegal
    n = eq(n, triangle("0", "0", "0"), "Illegal");
    n = eq(n, triangle("0", "10", "12"), "Illegal");
    n = eq(n, triangle("10", "12", "0"), "Illegal");
    n = eq(n, triangle("-1", "-1", "-1"), "Illegal");
    n = eq(n, triangle("-1", "10", "12"), "Illegal");
    n = eq(n, triangle("10", "-1", "12"), "Illegal");
    n = eq(n, triangle("10", "12", "-1"), "Illegal");
    n = eq(n, triangle("x", "y", "z"), "Illegal");
    n = eq(n, triangle("3", "4y", "5"), "Illegal");
    n = eq(n, triangle("10", "12", "13.4"), "Illegal");
    n = eq(n, triangle("03", "4", "5"), "Illegal");

    // Tests 43 to 47: check the upper limits on lengths
    n = eq(n, triangle("2147483647","2147483647","2147483647"),"Equilateral");
    n = eq(n, triangle("2147483648","2147483647","2147483647"),"Illegal");
    n = eq(n, triangle("2147483647","2147483648","2147483647"),"Illegal");
    n = eq(n, triangle("2147483647","2147483647","2147483648"),"Illegal");
    n = eq(n, triangle("2147483649","2147483649","2147483649"),"Illegal");

    // Tests 48 to 50: check for correct handling of overflow
    n = eq(n, triangle("1100000000","1705032704","1805032704"),"Scalene");
    n = eq(n, triangle("2000000001","2000000002","2000000003"),"Scalene");
    n = eq(n, triangle("150000002","666666671","683333338"),"Scalene");

    printf("Tests passed: %d\n", n);
}

// Run the program or, if there are no arguments, test it.
int main(int argc, char **argv) {
    if (argc == 1) {
        test();
    }
    else if (argc == 4) {
        char *result = triangle(argv[1], argv[2], argv[3]);
        printf("%s\n", result);
    }
    else {
        fprintf(stderr, "Use e.g.: ./triangle 3 4 5\n");
        exit(1);
    }
}
