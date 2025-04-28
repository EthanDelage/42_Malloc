NAME		=		libft_malloc_$(HOST_TYPE)$(LIB_EXT)

NAME_TEST	=		test_malloc

ifeq ($(HOST_TYPE),)
	HOST_TYPE := $(shell uname -m)_$(shell uname -s)
endif

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    LIB_EXT := .so
endif
ifeq ($(UNAME_S),Darwin)
    LIB_EXT := .dylib
endif

#######################
#	DIR
#######################

SRC_DIR		=		src/

INC_DIR		=		include/

TEST_DIR	=		tests/

BUILD_DIR	=		.build/$(HOST_TYPE)/

#######################
#	FILES
#######################

SRC			=		malloc.c \
					free.c \
					realloc.c

OBJ			=		$(addprefix $(BUILD_DIR), $(SRC:.c=.o))

DEPS		=		$(OBJ:.o=.d)

TEST_SRC	=		main.c

TEST_OBJ	=		$(addprefix $(BUILD_DIR)tests/, $(TEST_SRC:.c=.o))

TEST_DEPS	=		$(TEST_OBJ:.o=.d)

#######################
#	FLAGS
#######################

CFLAGS		=		-Wall -Werror -Wextra

IFLAGS		=		-I $(INC_DIR)

LFLAGS		=

DFLAGS		=		-MMD -MP

#######################
#	RULES
#######################

############
#	GENERAL
############

.PHONY:				all
all:				$(NAME)

.PHONY:				test
test:				$(NAME_TEST)

.PHONY:				run
run:				test
					./$(NAME_TEST) $(ARGS)

.PHONY:				clean
clean:
					$(RM) $(OBJ) $(DEPS) $(TEST_OBJ) $(TEST_DEPS)

.PHONY:				fclean
fclean:				clean
					$(RM) $(NAME) $(NAME_TEST)

.PHONY:				re
re:					fclean
					$(MAKE)

.PHONY:				rerun
rerun:				re
					$(MAKE) run

.PHONY:				format
format:
					find . -name "*.c" -o -name "*.h" | xargs clang-format -i

################
#	EXECUTABLES
################

-include			$(DEPS)
-include			$(TEST_DEPS)

$(NAME):			$(OBJ)
					$(CC) --shared $(CFLAGS) $(IFLAGS) $^ $(LFLAGS) -o $@

$(NAME_TEST):		$(TEST_OBJ)
					$(MAKE) $(NAME)
					$(CC) $(CFLAGS) $(IFLAGS) $^ $(LFLAGS) -o $@

##################
#	OBJECTS FILES
##################

vpath %.c $(SRC_DIR) $(TEST_DIR)

$(BUILD_DIR)%.o:	%.c
					mkdir -p $(shell dirname $@)
					$(CC) $(CFLAGS) $(DFLAGS) $(IFLAGS) -c $< -o $@
