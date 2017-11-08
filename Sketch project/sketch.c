#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "display.h"

// TODO: upgrade the run function, adding functions to support it.

struct memorise {
  int PenUp;
  int CurrPosX;
  int CurrPosY;
  int PrevPosX;
  int PrevPosY;
  int RGBA;
  bool ColOn;
};

enum { DX = 0, DY = 1, DT = 2, PEN = 3 };
typedef unsigned char byte;
typedef struct memorise memorise;

void InitialiseStruct(memorise *mem) {
  mem->PenUp = 0;
  mem->CurrPosX = 0;
  mem->CurrPosY = 0;
  mem->PrevPosX = 0;
  mem->PrevPosY = 0;
  mem->RGBA = 0;
  mem->ColOn = false;
}

void OpOne(memorise *mem, display *d, int op, int inst) {
  if (inst == DX) {
    mem->PrevPosX = mem->CurrPosX;
    mem->CurrPosX = mem->CurrPosX + op;
  }
}

void OpTwo(memorise *mem, display *d, int op, int inst) {
  if (inst == DY) {
    mem->PrevPosY = mem->CurrPosY;
    mem->CurrPosY = mem->CurrPosY + op;
    if (mem->PenUp == 1) {
      int x0 = mem->PrevPosX;
      int x1 = mem->CurrPosX;
      int y0 = mem->PrevPosY;
      int y1 = mem->CurrPosY;
      int RGBA = mem->RGBA;
      if (mem->ColOn == false) {
        line(d, x0, y0, x1, y1);
      } else {
        cline(d, x0, y0, x1, y1, RGBA);
      }
    }
    mem->PrevPosX = mem->CurrPosX;
    mem->PrevPosY = mem->CurrPosY;
  }
}

void OpThree(memorise *mem, display *d, int op, int inst) {
  if (inst == DT) {pause(d, op);}

}

void OpFour(memorise *mem, int inst) {
  if (inst == PEN) {
    if (mem->PenUp == 1) {mem->PenUp = 0;}
    else {mem->PenUp = 1;}
  }
}

void OpFive(display *d, int inst) {
  if (inst == 4) {
    clear(d);
  }
}

void OpSix(display *d, int inst) {
  if (inst == 5) {
    key(d);
  }
}

void OpSeven(display *d, memorise *mem, int packed, int inst) {
  if (inst == 6) {
    if (packed == 0x000000FF) {
      mem->RGBA = 0;
      mem->ColOn = false;
    }
    else {
      mem->RGBA = packed;
      mem->ColOn = true;
    }
  }
}

int Opcode(byte b) {
  int inst = (b & 0xC0) >> 6;
  return inst;
}

void Operand(memorise *mem, display *d, byte b, int inst) {
  int op;
  int transfer;
  if ((b & 32) == 32 && inst != DT) {
    transfer = -1 << 6;
    op = transfer | (b & 63);
  } else {op = (b & 0x3F);}
  OpOne(mem,d, op, inst);
  OpTwo(mem, d, op, inst);
  OpThree(mem, d, op, inst);
}

void Operations(memorise *mem, display *d, int Oprand, int newInst) {
  OpOne(mem, d, Oprand, newInst);
  OpTwo(mem, d, Oprand, newInst);
  OpThree(mem, d, Oprand, newInst);
  OpFour(mem, newInst);
  OpFive(d, newInst);
  OpSix(d, newInst);
  OpSeven(d, mem, Oprand, newInst);
}

int PackToInt(int packed) {
  if ((packed & 128) == 128) {
    int transfer = -1 << 8;
    packed = packed | transfer;
  }
  return packed;
}

void OpLengthZero(FILE *in, memorise *mem, display *d, int newInst, int length) {
  if (length == 0) {Operations(mem, d, 0, newInst);}
}
 void OpLengthOne(FILE *in, memorise *mem, display *d, int newInst, int length) {
   if (length == 1) {
     byte b;
     b = fgetc(in);
     int packed = b;
     packed = PackToInt(packed);
     Operations(mem, d, packed, newInst);
   }
 }

 void OpLengthTwo(FILE *in, memorise *mem, display *d, int newInst, int length) {
   if (length == 2) {
     byte b;
     b = fgetc(in);
     int packed = b;
     packed = PackToInt(packed);
     b = fgetc(in);
     packed = packed << 8;
     packed = (packed | b);
     Operations(mem, d, packed, newInst);
   }
 }

 void OpLengthThree(FILE *in, memorise *mem, display *d, int newInst, int length) {
   if (length == 3) {
     byte b;
     int packed = 0;
     b = fgetc(in);
     packed = b << 8;
     b = fgetc(in);
     packed = (packed | b) << 8;
     b = fgetc(in);
     packed = (packed | b) << 8;
     b = fgetc(in);
     packed = (packed | b);
     Operations(mem, d, packed, newInst);
   }
 }

void interpret(FILE *in, display *d) {
  memorise mem;
  byte b;
  InitialiseStruct(&mem);
  int inst = 0;
  while (! feof(in)) {
    b = fgetc(in);
    inst = Opcode(b);
    if (inst < 3) {
      Operand(&mem, d, b, inst);
    }
    else {
      int length = (b & 48) >> 4;
      int newInst = b & 15;
      OpLengthZero(in, &mem, d, newInst, length);
      OpLengthOne(in, &mem, d, newInst, length);
      OpLengthTwo(in, &mem, d, newInst, length);
      OpLengthThree(in, &mem, d, newInst, length);
    }
  }
}

void run(char *filename, char *test[]) {
    FILE *in = fopen(filename, "rb");
    if (in == NULL) {
        fprintf(stderr, "Can't open %s\n", filename);
        exit(1);
    }
    display *d = newDisplay(filename, 200, 200, test);
    interpret(in, d);
    end(d);
    fclose(in);
}

// ----------------------------------------------------------------------------
// Nothing below this point needs to be changed.
// ----------------------------------------------------------------------------

// Forward declaration of test data.
char **lineTest, **squareTest, **boxTest, **oxoTest, **diagTest, **crossTest,
     **clearTest, **keyTest, **pausesTest, **fieldTest, **lawnTest;

void testSketches() {
    // Stage 1
    run("line.sketch", lineTest);
    run("square.sketch", squareTest);
    run("box.sketch", boxTest);
    run("oxo.sketch", oxoTest);

    // Stage 2
    run("diag.sketch", diagTest);
    run("cross.sketch", crossTest);

    // Stage 3
    run("clear.sketch", clearTest);
    run("key.sketch", keyTest);

    // Stage 4
    run("pauses.sketch", pausesTest);
    run("field.sketch", fieldTest);
    run("lawn.sketch", lawnTest);
}

int main(int n, char *args[n]) {
    if (n == 1) testSketches();
    else if (n == 2) run(args[1], NULL);
    else {
        fprintf(stderr, "Usage: sketch [file.sketch]");
        exit(1);
    }
}

// Each test is a series of calls, stored in a variable-length array of strings,
// with a NULL terminator.

// The calls that should be made for line.sketch.
char **lineTest = (char *[]) {
    "line(d,30,30,60,30)", NULL
};

// The calls that should be made for square.sketch.
char **squareTest = (char *[]) {
    "line(d,30,30,60,30)", "line(d,60,30,60,60)",
    "line(d,60,60,30,60)","line(d,30,60,30,30)", NULL
};

// The calls that should be made for box.sketch.
char **boxTest = (char *[]) {
    "line(d,30,30,32,30)", "pause(d,10)", "line(d,32,30,34,30)", "pause(d,10)",
    "line(d,34,30,36,30)", "pause(d,10)", "line(d,36,30,38,30)", "pause(d,10)",
    "line(d,38,30,40,30)", "pause(d,10)", "line(d,40,30,42,30)", "pause(d,10)",
    "line(d,42,30,44,30)", "pause(d,10)", "line(d,44,30,46,30)", "pause(d,10)",
    "line(d,46,30,48,30)", "pause(d,10)", "line(d,48,30,50,30)", "pause(d,10)",
    "line(d,50,30,52,30)", "pause(d,10)", "line(d,52,30,54,30)", "pause(d,10)",
    "line(d,54,30,56,30)", "pause(d,10)", "line(d,56,30,58,30)", "pause(d,10)",
    "line(d,58,30,60,30)", "pause(d,10)", "line(d,60,30,60,32)", "pause(d,10)",
    "line(d,60,32,60,34)", "pause(d,10)", "line(d,60,34,60,36)", "pause(d,10)",
    "line(d,60,36,60,38)", "pause(d,10)", "line(d,60,38,60,40)", "pause(d,10)",
    "line(d,60,40,60,42)", "pause(d,10)", "line(d,60,42,60,44)", "pause(d,10)",
    "line(d,60,44,60,46)", "pause(d,10)", "line(d,60,46,60,48)", "pause(d,10)",
    "line(d,60,48,60,50)", "pause(d,10)", "line(d,60,50,60,52)", "pause(d,10)",
    "line(d,60,52,60,54)", "pause(d,10)", "line(d,60,54,60,56)", "pause(d,10)",
    "line(d,60,56,60,58)", "pause(d,10)", "line(d,60,58,60,60)", "pause(d,10)",
    "line(d,60,60,58,60)", "pause(d,10)", "line(d,58,60,56,60)", "pause(d,10)",
    "line(d,56,60,54,60)", "pause(d,10)", "line(d,54,60,52,60)", "pause(d,10)",
    "line(d,52,60,50,60)", "pause(d,10)", "line(d,50,60,48,60)", "pause(d,10)",
    "line(d,48,60,46,60)", "pause(d,10)", "line(d,46,60,44,60)", "pause(d,10)",
    "line(d,44,60,42,60)", "pause(d,10)", "line(d,42,60,40,60)", "pause(d,10)",
    "line(d,40,60,38,60)", "pause(d,10)", "line(d,38,60,36,60)", "pause(d,10)",
    "line(d,36,60,34,60)", "pause(d,10)", "line(d,34,60,32,60)", "pause(d,10)",
    "line(d,32,60,30,60)", "pause(d,10)", "line(d,30,60,30,58)", "pause(d,10)",
    "line(d,30,58,30,56)", "pause(d,10)", "line(d,30,56,30,54)", "pause(d,10)",
    "line(d,30,54,30,52)", "pause(d,10)", "line(d,30,52,30,50)", "pause(d,10)",
    "line(d,30,50,30,48)", "pause(d,10)", "line(d,30,48,30,46)", "pause(d,10)",
    "line(d,30,46,30,44)", "pause(d,10)", "line(d,30,44,30,42)", "pause(d,10)",
    "line(d,30,42,30,40)", "pause(d,10)", "line(d,30,40,30,38)", "pause(d,10)",
    "line(d,30,38,30,36)", "pause(d,10)", "line(d,30,36,30,34)", "pause(d,10)",
    "line(d,30,34,30,32)", "pause(d,10)", "line(d,30,32,30,30)", "pause(d,10)",
    NULL
};

// The calls that should be made for box.sketch.
char **oxoTest = (char *[]) {
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "line(d,30,40,60,40)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "line(d,30,50,60,50)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "line(d,40,30,40,60)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "line(d,50,30,50,60)", NULL
};

// The calls that should be made for diag.sketch.
char **diagTest = (char *[]) {
    "line(d,30,30,60,60)", NULL
};

// The calls that should be made for cross.sketch.
char **crossTest = (char *[]) {
    "line(d,30,30,60,60)", "line(d,60,30,30,60)", NULL
};

// The calls that should be made for clear.sketch.
char **clearTest = (char *[]) {
    "line(d,30,40,60,40)", "line(d,30,50,60,50)", "line(d,40,30,40,60)",
    "line(d,50,30,50,60)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "clear(d)", "line(d,30,30,60,60)",
    "line(d,60,30,30,60)", NULL
};

// The calls that should be made for key.sketch.
char **keyTest = (char *[]) {
    "line(d,30,40,60,40)", "line(d,30,50,60,50)", "line(d,40,30,40,60)",
    "line(d,50,30,50,60)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)", "pause(d,63)",
    "pause(d,63)", "pause(d,63)", "key(d)", "clear(d)", "line(d,30,30,60,60)",
    "line(d,60,30,30,60)", NULL
};

// The calls that should be made for diag.sketch.
char **pausesTest = (char *[]) {
    "pause(d,0)", "pause(d,0)", "pause(d,127)", "pause(d,128)", "pause(d,300)",
    "pause(d,0)", "pause(d,71469)", NULL
};

// The calls that should be made for field.sketch.
char **fieldTest = (char *[]) {
    "line(d,30,30,170,30)", "line(d,170,30,170,170)",
    "line(d,170,170,30,170)", "line(d,30,170,30,30)", NULL
};

// The calls that should be made for field.sketch.
char **lawnTest = (char *[]) {
    "cline(d,30,30,170,30,16711935)", "cline(d,170,30,170,170,16711935)",
    "cline(d,170,170,30,170,16711935)", "cline(d,30,170,30,30,16711935)",
    NULL
};
