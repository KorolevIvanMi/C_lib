CC = gcc
CFLAGS = -shared -O2 -fPIC
PYTHON_INC = $(shell python3-config --includes 2>/dev/null || echo "-I/usr/include/python3.14")
INC = -I./headers
TARGET = lib/mylist.so

SOURCES = c/list_module.c  c/core_module.c c/read_module.c c/update_module.c c/delete_module.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(PYTHON_INC) $(INC) -o $@ $^

clean:
	rm -f $(TARGET)

.PHONY: all clean