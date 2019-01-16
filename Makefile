NAME := fluffy

SRCFILES := $(wildcard src/*.c)
OBJFILES := $(patsubst %.c,%.o,${SRCFILES})

CC := clang

CFLAGS := -std=c11 -pedantic-errors -Werror -Weverything \
			-fdiagnostics-show-option \
			-Wno-missing-prototypes -Wno-error=reserved-id-macro \
			-Wno-cast-qual -Wno-missing-prototypes -Wno-vla \
			-Wno-padded \
			-D_POSIX_C_SOURCE=200809L


all: ${NAME}

${NAME}:
	${CC} ${CFLAGS} ${SRCFILES} -o ${NAME}

clean:
	find -name '*.o' -delete
	rm -f ${NAME}

.PHONY: clean
