#define FREE_IF_NONNULL_A(x) {\
  if (x != NULL) {\
    delete[] x;\
  }\
}