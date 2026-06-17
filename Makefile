CC = gcc
CFLAGS = -shared -O2 -fPIC
PYTHON_INC = -I/usr/include/python3.14
INC = -I./headers
TARGET = build/mylist.so

# Перечисляем все .c файлы
SOURCES = c/list_module.c  c/core_module.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(PYTHON_INC) $(INC) -o $@ $^

clean:
	rm -f $(TARGET)

.PHONY: all clean