CC      = cc
CFLAGS  = -Wall -Wextra -Werror
RM      = rm -rf

NAME    = philo

OBJDIR  = objs
HEADER  = philo.h

SRC = \
	main.c \
	philo_utils.c \
	time.c \
	utils.c \
	init.c \
	monitor.c \
	validate.c \
	system.c

OBJS = $(SRC:%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJDIR)/%.o: %.c $(HEADER) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	$(RM) $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
