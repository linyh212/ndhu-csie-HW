CC = gcc
CFLAGS = -Wall -Iinclude -g
LDFLAGS = -pthread

TARGETS = shell server bin/who bin/tell bin/yell bin/name

# Shell 相關
SRCDIR = src
OBJDIR = object
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

# color
COLOR_GREEN = \033[0;32m
COLOR_RESET = \033[0m

all: $(TARGETS)
	@echo "$(COLOR_GREEN)Build complete!$(COLOR_RESET)"
	@echo "  - ./shell"
	@echo "  - ./server"
	@echo "  - bin/who, bin/tell, bin/yell, bin/name"

# Shell
shell: $(OBJECTS)
	$(CC) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

# server
server: server.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# external
bin/who: bin_c/who.c | bin
	$(CC) $(CFLAGS) -o $@ $<

bin/tell: bin_c/tell.c | bin
	$(CC) $(CFLAGS) -o $@ $<

bin/yell: bin_c/yell.c | bin
	$(CC) $(CFLAGS) -o $@ $<

bin/name: bin_c/name.c | bin
	$(CC) $(CFLAGS) -o $@ $<

bin:
	mkdir -p bin

# clean
clean:
	rm -rf $(OBJDIR) shell server bin/who bin/tell bin/yell bin/name
	@echo "Clean complete"
	@echo "Original bin files (cat, echo, grep, ls, etc.) preserved."

# 完全清理（包含 bin 目錄下所有檔案，慎用）
distclean:
	rm -rf $(OBJDIR) shell server bin
	@echo "Distclean complete"

.PHONY: all clean distclean