
#ifndef ITKTENSORFLOW_ITERP_H
#define ITKTENSORFLOW_ITERP_H

#ifdef __unix__
const char interp_section[] __attribute__((section(".interp"))) = "ld-linux-x86-64.so.2";
#endif

#endif // ITKTENSORFLOW_ITERP_H
