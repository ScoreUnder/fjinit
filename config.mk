# fjinit version
VERSION = 0.9.2

# paths
PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

CC = musl-gcc
LD = $(CC)
CPPFLAGS =
CFLAGS   = -Wextra -Wall -Os
LDFLAGS  = -s -static
