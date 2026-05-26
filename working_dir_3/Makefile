CC = gcc
CFLAGS = -Wall -Iinclude -g
LDFLAGS = -pthread

# MySQL path
MYSQL_PATH = /opt/homebrew/opt/mysql-client
CFLAGS += -I$(MYSQL_PATH)/include
LDFLAGS += -L$(MYSQL_PATH)/lib -lmysqlclient
TARGETS = shell server

# HW2
HW2_CMDS = bin/who bin/tell bin/yell bin/name

# HW3
HW3_CMDS = bin/login bin/register bin/listMail bin/mailto bin/delMail \
           bin/createGroup bin/delGroup bin/addTo bin/leaveGroup \
           bin/listGroup bin/remove bin/gyell bin/logout

# Shell
SRCDIR = src
OBJDIR = object
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

# color
COLOR_GREEN = \033[0;32m
COLOR_RESET = \033[0m

all: $(TARGETS) $(HW2_CMDS) $(HW3_CMDS)
	@echo "$(COLOR_GREEN)Build complete!$(COLOR_RESET)"
	@echo "  - ./shell"
	@echo "  - ./server"
	@echo "  - HW2 commands: who, tell, yell, name"
	@echo "  - HW3 commands: login, register, listMail, mailto, delMail,"
	@echo "                 createGroup, delGroup, addTo, leaveGroup,"
	@echo "                 listGroup, remove, gyell, logout"

# Shell
shell: $(OBJECTS)
	$(CC) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

# server（require MySQL）
server: bin_c/server.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# HW2
bin/who: bin_c/who.c | bin
	$(CC) $(CFLAGS) -o $@ $<

bin/tell: bin_c/tell.c | bin
	$(CC) $(CFLAGS) -o $@ $<

bin/yell: bin_c/yell.c | bin
	$(CC) $(CFLAGS) -o $@ $<

bin/name: bin_c/name.c | bin
	$(CC) $(CFLAGS) -o $@ $<

# HW3 外部命令
bin/login: bin_c/login.c | bin
	$(CC) $(CFLAGS) -o $@ $<

bin/register: bin_c/register.c | bin
	$(CC) $(CFLAGS) -o $@ $<

bin/listMail: bin_c/listMail.c | bin
	$(CC) $(CFLAGS) -o $@ $<

bin/mailto: bin_c/mailto.c | bin
	$(CC) $(CFLAGS) -o $@ $<

bin/delMail: bin_c/delMail.c | bin
	$(CC) $(CFLAGS) -o $@ $<

bin/createGroup: bin_c/createGroup.c | bin
	$(CC) $(CFLAGS) -o $@ $<

bin/delGroup: bin_c/delGroup.c | bin
	$(CC) $(CFLAGS) -o $@ $<

bin/addTo: bin_c/addTo.c | bin
	$(CC) $(CFLAGS) -o $@ $<

bin/leaveGroup: bin_c/leaveGroup.c | bin
	$(CC) $(CFLAGS) -o $@ $<

bin/listGroup: bin_c/listGroup.c | bin
	$(CC) $(CFLAGS) -o $@ $<

bin/remove: bin_c/remove.c | bin
	$(CC) $(CFLAGS) -o $@ $<

bin/gyell: bin_c/gyell.c | bin
	$(CC) $(CFLAGS) -o $@ $<

bin/logout: bin_c/logout.c | bin
	$(CC) $(CFLAGS) -o $@ $<

bin:
	mkdir -p bin

# clean
clean:
	rm -rf $(OBJDIR) shell server $(HW2_CMDS) $(HW3_CMDS)
	@echo "Clean complete"
	@echo "Original bin files (cat, echo, grep, ls, etc.) preserved."

# 完全清理（包含 bin 目錄下所有檔案）
distclean:
	rm -rf $(OBJDIR) shell server bin
	@echo "Distclean complete"

.PHONY: all clean distclean