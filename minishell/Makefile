NAME = minishell

CC = cc 

FLAGS = -g -Wall -Wextra 

RM = rm -f

SRCS = main.c ft_strcmp.c ft_split.c ft_substr.c ft_strlen.c ft_strdup.c builtin.c  ft_atoi.c is_numeric.c  \
ft_strjoin.c ft_strlcpy.c ft_strlcat.c ft_putstr_fd.c exucutable.c ft_strchr.c env.c ft_lstsize.c builtin2.c \
export.c ft_lstadd_back.c ft_strtrim.c export2.c unset.c rediractions1.c rediractions2.c rediractions3.c
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







