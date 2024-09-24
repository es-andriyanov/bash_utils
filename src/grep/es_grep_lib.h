#ifndef ES_GREP_LIB_H
#define ES_GREP_LIB_H

#define _GNU_SOURCE
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct grep_flags {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  char** files;
  char* pttrns;
  int filesNum;
  int pttrnsNum;
} grep_flags;

void es_grep(int argc, char** argv);
void init_grep(grep_flags* flags);
void free_grep(grep_flags* flags);

int get_keys(int argc, char** argv, grep_flags* flags);
void update_res(int* res, int argc, int optind, grep_flags* flags, char** argv);
void print_usage();
void make_grep(grep_flags* flags);
void make_grep_file(FILE* fp, int filenum, regex_t* re, int is_many,
                    grep_flags* flags);
int v_formatter(char* buffer, regex_t* re, grep_flags* flags, int is_many,
                int filenum, int line);
int o_formatter(char* buffer, regex_t* re, grep_flags* flags, int is_many,
                int filenum, int line);
int std_formatter(char* buffer, regex_t* re, grep_flags* flags, int is_many,
                  int filenum, int line);
void cl_formatter(int is_many, int res, int filenum, grep_flags* flags);
void last_enter(char* str);

int add_to_pttrns(char* adding, grep_flags* flags);
int add_to_pttrns_from_file(char* filename, grep_flags* flags);
int add_files(char* filename, grep_flags* flags);
void read_string_to_pttrns(FILE* txt, char** string, grep_flags* flags);

#endif
