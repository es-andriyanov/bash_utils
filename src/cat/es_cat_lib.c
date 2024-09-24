#include "es_cat_lib.h"

void es_cat(int argc, char** argv) {
  struct cat_flags flags = {0, 0, 0, 0, 0, 0};

  int line_num = 0;
  int found = 0;
  char prev = EOF;
  int first_file_index = get_keys(argc, argv, &flags);

  if (-1 != first_file_index) {
    for (int i = first_file_index; i < argc; i++) {
      make_cat(&flags, argv[i], &line_num, &found, &prev);
    }
  }
}

int get_keys(int argc, char** argv, cat_flags* flags) {
  int getkey = 0, ind = 0, res = 0;

  struct option options[] = {{"number-nonblank", no_argument, 0, 'b'},
                             {"number", no_argument, 0, 'n'},
                             {"squeeze-blank", no_argument, 0, 's'},
                             {0, 0, 0, 0}};

  while (-1 != (getkey = getopt_long(argc, argv, "beEnstTv", options, &ind))) {
    switch (getkey) {
      case 'b':
        flags->b = flags->n = 1;
        break;
      case 'e':
        flags->e = flags->v = 1;
        break;
      case 'E':
        flags->e = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 't':
        flags->t = flags->v = 1;
        break;
      case 'T':
        flags->t = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      default:
        fprintf(stderr,
                "usage: ./es_cat [-b | --number-nonblank] [-n | --number] [-s "
                "| --squeeze-blank] [-eEtTv] [file ...]\n");
        res = -1;
    }
  }
  make_res(argc, optind, &res);

  return res;
}

void make_res(int argc, int optind, int* res) {
  if (0 == argc - optind && -1 != *res) {
    *res = -1;
    fprintf(stderr, "es_cat: no file in command\n");
    fprintf(stderr,
            "usage: ./es_cat [-b | --number-nonblank] [-n | --number] [-s | "
            "--squeeze-blank] [-eEtTv] [file ...]\n");
  }

  if (-1 != *res) *res = optind;
}

void make_cat(cat_flags* flags, char* file_name, int* line_num, int* found,
              char* prev) {
  if (file_name != NULL) {
    FILE* txt = fopen(file_name, "r");

    if (txt != NULL) {
      char ch = '\n';
      int skip = 0;

      for (; (ch = getc(txt)) != EOF; *prev = ch) {
        print_by_flag(ch, prev, line_num, &skip, flags, found);
        *found = 1;
      }
      fclose(txt);
    } else {
      fprintf(stderr, "es_cat: %s: No such file or directory\n", file_name);
    }
  } else {
    fprintf(stderr, "es_cat: No path of the file\n");
  }
}

void print_by_flag(char ch, char* prev, int* line, int* skip, cat_flags* flags,
                   int* found) {
  if (flags->s) {
    if (ch == '\n' && (*prev == '\n' || !(*found))) {
      if (!(*skip)) {
        check_bnvet(ch, prev, line, flags, found);
        *skip = 1;
      }
    } else {
      *skip = 0;
    }
  }

  if (!(*skip)) {
    check_bnvet(ch, prev, line, flags, found);
  }
}

void check_bnvet(char ch, char* prev, int* line, cat_flags* flags, int* found) {
  if (flags->b || (flags->n && flags->b)) {
    bn_formatter(ch, prev, line, found);
    vet_formatter(ch, flags);
  }
  if (flags->n && !(flags->b)) {
    n_formatter(prev, line, found);
    vet_formatter(ch, flags);
  }
  if (!(flags->n) && !(flags->b)) {
    vet_formatter(ch, flags);
  }
}

void vet_formatter(char ch, cat_flags* flags) {
  int done = 0;

  if (flags->v) {
    if (ch >= 0 && (ch != 9 && ch != 10) && ch < 32) {
      putchar('^');
      putchar(ch + 64);
      done = 1;
    } else if (ch == 127) {
      putchar('^');
      putchar('?');
      done = 1;
    } else if (!isascii(ch) && !isprint(ch)) {
      putchar('M');
      putchar('-');
      ch = toascii(ch);

      if (iscntrl(ch)) {
        putchar('^');
        putchar(ch | 64);
      }
      putchar(ch);
      done = 1;
    }
  }
  if (flags->e) {
    if (ch == '\n') {
      putchar('$');
      putchar('\n');
      done = 1;
    }
  }
  if (flags->t) {
    if (ch == '\t') {
      putchar('^');
      putchar('I');
      done = 1;
    }
  }
  if (!done) {
    putchar(ch);
  }
}

void bn_formatter(char ch, char* prev, int* line, int* found) {
  if (ch != '\n' && (*prev == '\n' || !(*found))) {
    *found = 1;
    fprintf(stdout, "%6d\t", ++(*line));
  }
}

void n_formatter(char* prev, int* line, int* found) {
  if (*prev == '\n' || !(*found)) {
    *found = 1;
    fprintf(stdout, "%6d\t", ++(*line));
  }
}
