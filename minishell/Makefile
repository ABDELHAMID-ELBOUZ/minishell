NAME = mini_shell

CC = cc 

FLAGS = -Wall -Wextra -Werror

RM = rm -f

SRCS = main.c ft_strncmp.c echo.c ft_split.c ft_substr.c ft_strlen.c ft_strdup.c builtins.c pwd.c exit.c ft_atoi.c is_numeric.c  \
ft_strjoin.c ft_strlcpy.c ft_strlcat.c ft_putstr_fd.c exucutable.c ft_strchr.c env.c ft_lstsize.c

OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(FLAGS) $^ -o $(NAME) -lreadline

%.o : %.c minishell.h
	$(CC) $(FLAGS) -c $< -o $@
clean :
	$(RM) $(OBJS)

fclean : clean
	$(RM) $(NAME)

re : fclean all







