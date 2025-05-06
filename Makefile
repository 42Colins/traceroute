NAME = ft_traceroute

CFLAGS = -Wall -Wextra -Werror -g

libftFLAGS = -L./libft -lft

LIB = libft/libft.a

RM = $(RM) -f

HEADERS =	$(DIR_INCLUDES)ft_traceroute.h

MKDIR = mkdir -p

RM = rm -rf

DIR_OBJ = ./.obj/

DIR_SRCS = srcs/

DIR_INCLUDES = ./inc/

SRCS =			$(DIR_SRCS)main.c \
				$(DIR_SRCS)utils.c \
				$(DIR_SRCS)print.c \
				$(DIR_SRCS)init.c \
				$(DIR_SRCS)free.c \
				$(DIR_SRCS)udp.c \
 
OBJS = $(patsubst %.c, $(DIR_OBJ)%.o, $(SRCS))

all:	$(LIB) $(NAME)

$(DIR_OBJ)%.o: %.c $(HEADERS)
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -I$(DIR_INCLUDES) -I/usr/include -O3 -I./libft -c $< -o $@

$(NAME): $(OBJS) $(LIB)
	$(CC) $(OBJS) -g -L./libft -lft -lm -o $(NAME)

$(LIB):
	$(MAKE) -C libft

clean:
	$(MAKE) clean -C libft
	$(RM) $(OBJS)

fclean:	clean
	$(MAKE) fclean -C libft
	$(RM) $(NAME)

re:	fclean all