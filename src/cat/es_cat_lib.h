#ifndef ES_CAT_LIB_H
#define ES_CAT_LIB_H

#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct cat_flags {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} cat_flags;

void es_cat(int argc, char** argv);
int get_keys(int argc, char** argv, cat_flags* flags);
void make_res(int argc, int optind, int* res);
void make_cat(cat_flags* flags, char* file_name, int* line_num, int* found,
              char* prev);
void print_by_flag(char ch, char* prev, int* line, int* skip, cat_flags* flags,
                   int* found);

void check_bnvet(char ch, char* prev, int* line, cat_flags* flags, int* found);
void vet_formatter(char ch, cat_flags* flags);
void bn_formatter(char ch, char* prev, int* line, int* found);
void n_formatter(char* prev, int* line, int* found);

#endif
