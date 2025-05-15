CC     = cc
CFLAGS = -Wall -Wextra -Werror
RM     = rm -rf

NAME   = philosophers

MAN_NAME = philo
MAN_SRCS = philo/
MAN_OBJDIR = philo/objs
MAN_OBJS   = $(MAN_SRCS:srcs/%.c=$(MAN_OBJDIR)/%.o)

BON_SRCS = philo_bonus/
BON_OBJDIR = philo_bonus/objs

all: $(NAME)

$(MAN_NAME): $(MAN_OBJS)
	$(CC) $(CFLAGS) -o $(MAN_NAME) $(MAN_OBJS)


$(MAN_OBJS): $(MAN_SRCS) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(MAN_OBJDIR):
	mkdir -p $(MAN_OBJDIR)

clean:
	$(RM) $(MAN_OBJDIR)

fclean: clean
	$(RM) $(MAN_NAME)

re: fclean all

.PHONY: all clean fclean re
