#define FREE_IF_NONNULL_A(x) {\
  if (x != NULL) {\
    delete[] x;\
  }\
}

#ifndef NDEBUG
  #include <iostream>
  #define DEBUG_LOG(str) std::cout << str << std::endl
#else
  #define DEBUG_LOG(str) ((void)0)
#endif
