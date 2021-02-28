#include "helpers.h"

config conf;

bool quit; // global variable for exiting all loops and quitting the game

int err; // global variable for capturing error codes

// wrapper to return a formatted string of the correct size
// caller is responsible for freeing result
char *rprintf(char *fmtstr, ...) {
  va_list vargs;
  va_start(vargs, fmtstr);

  char *str;
  vasprintf(&str, fmtstr, vargs);

  va_end(vargs);
  return str;
}

void* salloc(size_t size){ //Named such after "Safe allocate"
  void *ptr = malloc(size);
  if(ptr == NULL){DEBUG("Error in salloc(): malloc() failed with size %lu", size);exit(EXIT_FAILURE);}
  else{return ptr;}
}

FILE* sfopen(char* s, char* m){
  FILE* f;
  if((f = fopen(s, m)) == NULL){DEBUG("Error in sfopen(): File not found: %s", s);exit(EXIT_FAILURE);}
  return f;
}

void sfclose(FILE* f){
  if(fclose(f) == EOF){DEBUG("Error in sfclose(): fclose() returned EOF");exit(EXIT_FAILURE);}
  return;
}

// unlike realloc, this also changes the pointer you pass in
void* resalloc(void** ptr, size_t size){
  void *ret;
  if(*ptr == NULL){
    ret = salloc(size);
  }
  else{
    ret = realloc(*ptr, size);
    if(ret == NULL){DEBUG("Error #1 in resalloc(): realloc() failed with size %lu", size);exit(EXIT_FAILURE);}
    if(*ptr == NULL){DEBUG("Error #2 in resalloc(): realloc() failed with size %lu", size);exit(EXIT_FAILURE);}
  }
  *ptr = ret;
  return ret;
}

int intcmp(const void *aa, const void *bb){
  const int *a = aa, *b = bb;
  return (*a < *b) ? -1 : (*a > *b);
}

void intsort(int* l, int s){
  qsort(l, s, sizeof(int), intcmp);
}

/*
 * makes a copy of obj on the heap, and returns a pointer to the copy.
 * size is the size of the object
 */
void *heap_copy(void *obj, size_t size) {
  void *ret = malloc(size);
  memcpy(ret, obj, size);
  return ret;
}

double linInterp(double y0, double y1, double x0, double x1, double x){
  return (y1-y0)/(x1-x0)*(x-x0)+y0;
}

double subLinInterp(double y0, double y1, double x0, double x1, double x){
  double a = (x1 - x0) / SQUARE((y1 - y0));
  return sqrt((x - x0) / a) + y0;
}

double _smoothInterp(double x) {
  // (1-cos(x*pi)+1)/2 smoothly interpolates between (0,0) and (1,1)
  return (1 - cos(x*pi)) / 2;
}

// shifting the intial portion of a cos curve
double smoothInterp(double y0, double y1, double x0, double x1, double x) {
  return y0 + _smoothInterp((x - x0) / (x1 - x0)) * (y1 - y0);
}
