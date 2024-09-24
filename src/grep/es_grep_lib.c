#include "es_grep_lib.h"

void es_grep(int argc, char** argv) {
  grep_flags flags;

  init_grep(&flags);
  if (-1 != get_keys(argc, argv, &flags)) {
    make_grep(&flags);
  }
  free_grep(&flags);
}

void init_grep(grep_flags* flags) {
  flags->e = 0;
  flags->i = 0;
  flags->v = 0;
  flags->c = 0;
  flags->l = 0;
  flags->n = 0;
  flags->h = 0;
  flags->s = 0;
  flags->f = 0;
  flags->o = 0;

  flags->filesNum = 0;
  flags->files = calloc(1, sizeof(char*));

  flags->pttrnsNum = 0;
  flags->pttrns = NULL;
}

void free_grep(grep_flags* flags) {
  if (flags->filesNum == 0) {
    free(flags->files[0]);
  } else {
    for (int i = 0; i < flags->filesNum; i++) {
      free(flags->files[i]);
    }
  }

  free(flags->files);
  free(flags->pttrns);
}

int get_keys(int argc, char** argv, grep_flags* flags) {
  int getkey = 0, res = 0;

  while (-1 != (getkey = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL))) {
    switch (getkey) {
      case 'e':
        flags->e = 1;
        res += add_to_pttrns(optarg, flags);
        break;
      case 'i':
        flags->i = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'f':
        res += add_to_pttrns_from_file(optarg, flags);
        flags->f = 1;
        break;
      case 'o':
        flags->o = 1;
        break;
      default:
        print_usage();
        res += -1;
    }
  }
  update_res(&res, argc, optind, flags, argv);

  return res;
}

void update_res(int* res, int argc, int optind, grep_flags* flags,
                char** argv) {
  int e_required = 0;

  e_required = flags->e == 0 && flags->f == 0 ? 1 : 0;

  if (0 == *res) {
    if (argc - optind - e_required <= 0) {
      *res += -1;
      if (!(flags->s)) {
        fprintf(stderr, "es_grep: no file or pattern in command\n");
        print_usage();
      }
    }
  }

  if (0 == *res) {
    if (e_required) {
      *res += add_to_pttrns(argv[optind], flags);
    }
  }

  if (0 == *res) {
    for (int i = optind + e_required; i < argc; i++) {
      *res += add_files(argv[i], flags);
    }
  }
}

void print_usage() {
  fprintf(stderr,
          "usage: ./es_grep [-ivclnhso] [-e pattern] [-f file] "
          "[pattern] [file ...]\n");
}

int add_to_pttrns(char* adding, grep_flags* flags) {
  int res = 0;
  char* sep = "|";
  char* tmp = NULL;

  if (adding != NULL) {
    int len_old = 0;
    if (flags->pttrns) len_old = strlen(flags->pttrns);
    int len = strlen(adding);

    if (len_old == 0) {
      tmp = realloc(flags->pttrns, sizeof(char) * (len_old + len + 1));
    } else {
      tmp = realloc(flags->pttrns, sizeof(char) * (len_old + len + 2));
    }

    if (tmp != NULL) {
      flags->pttrns = tmp;

      if (len_old == 0) {
        memcpy(flags->pttrns + len_old, adding, len + 1);
      } else {
        memcpy(flags->pttrns + len_old, sep, 1);
        memcpy(flags->pttrns + len_old + 1, adding, len + 1);
      }

      flags->pttrnsNum++;
    }
  } else {
    res = -1;
  }

  return res;
}

int add_to_pttrns_from_file(char* filename, grep_flags* flags) {
  int res = 0;
  FILE* txt = fopen(filename, "r");

  if (txt != NULL) {
    char* string = malloc(1 * sizeof(char));

    if (string) {
      read_string_to_pttrns(txt, &string, flags);
      if (string) free(string);
    } else {
      res = -1;
      if (!(flags->s))
        fprintf(stderr, "es_grep: %s: Error to read file, no memory\n",
                filename);
    }

    fclose(txt);
  } else {
    res = -1;
    if (!(flags->s))
      fprintf(stderr, "es_grep: %s: No such file or directory\n", filename);
  }

  return res;
}

int add_files(char* filename, grep_flags* flags) {
  int res = 0;
  FILE* txt = fopen(filename, "r");
  char** tmpp = NULL;
  char* tmp = NULL;

  if (txt != NULL) {
    int len = strlen(filename);

    tmpp = (char**)realloc(flags->files, sizeof(char*) * (flags->filesNum + 1));
    if (tmpp != NULL) {
      flags->files = tmpp;

      tmp = calloc(len + 1, sizeof(char));
      if (tmp != NULL) {
        flags->files[flags->filesNum] = tmp;
        strcpy(flags->files[flags->filesNum], filename);
        flags->filesNum++;
      }
    }
    fclose(txt);
  } else {
    if (!(flags->s))
      fprintf(stderr, "es_grep: %s: No such file or directory\n", filename);
  }

  return res;
}

void read_string_to_pttrns(FILE* txt, char** string, grep_flags* flags) {
  int cap = 1, len = 0;
  char ch = '\0';
  char* tmp = NULL;

  while (fscanf(txt, "%c", &ch) == 1) {
    if (ch == '\n') {
      (*string)[len] = '\0';
      add_to_pttrns(*string, flags);
      len = 0;
    } else {
      (*string)[len] = ch;
      len++;
      if (len >= cap) {
        tmp = (char*)realloc(*string, sizeof(char) * cap * 2);

        if (tmp != NULL) {
          *string = tmp;
          for (int i = len; i < cap * 2; i++) (*string)[i] = '\0';
          cap *= 2;
        }
      }
    }
  }

  if (len > 0) {
    string[len] = '\0';
    add_to_pttrns(*string, flags);
  }
}

void make_grep(grep_flags* flags) {
  regex_t re;
  int comp_flags = REG_EXTENDED;
  int is_many = 1;

  if (flags->i) comp_flags |= REG_ICASE;

  if (flags->filesNum < 2 || flags->h == 1) is_many = 0;

  if (regcomp(&re, flags->pttrns, comp_flags) == 0) {
    for (int filenum = 0; filenum < flags->filesNum; filenum++) {
      FILE* fp = fopen(flags->files[filenum], "r");

      if (fp) {
        make_grep_file(fp, filenum, &re, is_many, flags);
        fclose(fp);
      } else {
        if (!(flags->s))
          fprintf(stderr, "es_grep: %s: No such file or directory\n",
                  flags->files[filenum]);
      }
    }
    regfree(&re);
  } else {
    fprintf(stderr, "es_grep: Incorrect regcomp\n");
  }
}

void make_grep_file(FILE* fp, int filenum, regex_t* re, int is_many,
                    grep_flags* flags) {
  size_t len = 1;
  ssize_t read = 0;
  int res = 0;
  int line = 0;
  char* buffer = (char*)malloc(sizeof(char));

  while ((read = getline(&buffer, &len, fp)) != -1) {
    line++;

    if (flags->o && !flags->v)
      res += o_formatter(buffer, re, flags, is_many, filenum, line);
    else if (flags->v)
      res += v_formatter(buffer, re, flags, is_many, filenum, line);
    else if (!flags->v && !flags->o)
      res += std_formatter(buffer, re, flags, is_many, filenum, line);
  }

  cl_formatter(is_many, res, filenum, flags);

  if (buffer) free(buffer);
}

int o_formatter(char* buffer, regex_t* re, grep_flags* flags, int is_many,
                int filenum, int line) {
  char* ps = buffer;
  int eflag = 0;
  regmatch_t pmatch[2];
  int stop = 0;

  while (regexec(re, ps, 1, pmatch, eflag) == 0) {
    if (!flags->l && !flags->c) {
      if (is_many) printf("%s:", flags->files[filenum]);

      if (flags->n) printf("%d:", line);

      for (regoff_t i = pmatch[0].rm_so; i < pmatch[0].rm_eo; i++) {
        printf("%c", ps[i]);
      }
      printf("\n");
    }
    stop = 1;
    ps += pmatch[0].rm_eo;
    eflag = REG_NOTBOL;
  }

  return stop;
}

int v_formatter(char* buffer, regex_t* re, grep_flags* flags, int is_many,
                int filenum, int line) {
  char* ps = buffer;
  int stop = 0;

  if (regexec(re, ps, 0, NULL, 0) != 0) {
    if (!flags->l && !flags->c && !flags->o) {
      if (is_many) printf("%s:", flags->files[filenum]);

      if (flags->n) printf("%d:", line);

      printf("%s", ps);
      last_enter(ps);
    }
    stop = 1;
  }

  return stop;
}

int std_formatter(char* buffer, regex_t* re, grep_flags* flags, int is_many,
                  int filenum, int line) {
  char* ps = buffer;
  int stop = 0;

  if (regexec(re, ps, 0, NULL, 0) == 0) {
    if (!flags->l && !flags->c) {
      if (is_many) printf("%s:", flags->files[filenum]);

      if (flags->n) printf("%d:", line);

      printf("%s", ps);
      last_enter(ps);
    }
    stop = 1;
  }

  return stop;
}

void last_enter(char* str) {
  if (str[strlen(str) - 1] != '\n') printf("\n");
}

void cl_formatter(int is_many, int res, int filenum, grep_flags* flags) {
  if (flags->l) {
    if (res > 0) printf("%s\n", flags->files[filenum]);
  } else if (flags->c && is_many) {
    printf("%s:%d\n", flags->files[filenum], res);
  } else if (flags->c && !is_many) {
    printf("%d\n", res);
  }
}
