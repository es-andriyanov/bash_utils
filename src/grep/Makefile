CFLAGS=-Wall -Werror -Wextra
TARGET=es_grep
SOURCES=es_grep.c es_grep_lib.c
OBJECTS=$(SOURCES:.c=.o)
HEADERS=$(SOURCES:.c=.h)

.PHONY: all clean rebuild clang_n clang_i cppcheck test valgrind

all: $(TARGET)

clean:
	rm -rf $(TARGET) *.o *.log

rebuild: clean all

clang_n:
	clang-format -n $(SOURCES) $(HEADERS)

clang_i:
	clang-format -i $(SOURCES) $(HEADERS)

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem $(SOURCES) $(HEADERS)

test:
	bash grep_test.sh

valgrind:
	bash grep_leak_test.sh

$(TARGET): $(OBJECTS)
	gcc $(OBJECTS) -o $(TARGET)

.c.o:
	gcc $(CFLAGS) -c $< -o $@
