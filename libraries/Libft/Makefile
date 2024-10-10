# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lzhang2 <lzhang2@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/27 09:40:23 by lzhang2           #+#    #+#              #
#    Updated: 2024/10/10 15:17:57 by lzhang2          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

# 用于美化输出
GREEN		= \033[0;32m
RED			= \033[0;31m
RESET		= \033[0m

SRCS = ft_isalpha.c					\
       ft_isdigit.c					\
       ft_isalnum.c					\
       ft_isascii.c					\
       ft_isprint.c					\
	   ft_strlen.c					\
       ft_memset.c					\
       ft_bzero.c					\
       ft_memcpy.c					\
       ft_memmove.c					\
	   ft_strlcpy.c					\
       ft_strlcat.c					\
       ft_toupper.c					\
       ft_tolower.c					\
       ft_strchr.c					\
       ft_strrchr.c					\
       ft_strncmp.c					\
       ft_memchr.c					\
       ft_memcmp.c					\
       ft_strnstr.c					\
       ft_atoi.c					\
	   ft_calloc.c					\
	   ft_strdup.c					\
	   ft_substr.c					\
	   ft_strjoin.c					\
       ft_strtrim.c					\
	   ft_split.c					\
	   ft_itoa.c					\
	   ft_strmapi.c					\
	   ft_striteri.c				\
	   ft_putchar_fd.c				\
	   ft_putstr_fd.c 				\
	   ft_putendl_fd.c 				\
	   ft_putnbr_fd.c 				\
	   ft_printf.c					\
	   print_char.c 				\
	   print_decimal_10.c			\
	   print_hexadecimal_lower.c 	\
	   print_hexadecimal_upper.c 	\
	   print_pointer.c 				\
	   print_string.c 				\
	   print_unsigned_10.c			\
	   get_next_line.c				\
	   ft_strappend.c				\
	   ft_strcat.c					\
	   ft_strcpy.c
	   
SRCS_BONUS = ft_lstnew.c			\
			 ft_lstadd_front.c		\
			 ft_lstsize.c			\
			 ft_lstlast.c			\
			 ft_lstadd_back.c		\
			 ft_lstdelone.c			\
			 ft_lstclear.c			\
			 ft_lstiter.c			\
			 ft_lstmap.c


OBJS = $(SRCS:.c=.o)
OBJS_BONUS = $(SRCS_BONUS:.c=.o)
HEADER = libft.h
CC = cc 
FLAGS = -Wall -Wextra -Werror
RM = rm -rf

%.o: %.c		$(HEADER)
				@echo "\n$(NAME): $(GREEN)object files were created$(RESET)"
				$(CC) -I. -c $< -o $@

all: 			$(NAME)
# 如果对象文件已经生成，则使用 ar -rcs 命令创建静态库 libft.a。
$(NAME): 		$(OBJS)
				@echo "\n$(NAME): $(GREEN)$(NAME) was created$(RESET)"
				ar -rcs $(NAME) $(OBJS)

bonus:			$(OBJS) $(OBJS_BONUS)
				@echo "\n$(NAME): $(GREEN)$(NAME) was created with Bonus$(RESET)"
				ar -rcs $(NAME) $(OBJS) $(OBJS_BONUS)
	

clean:
				@echo "\n$(NAME): $(RED)object files were deleted$(RESET)"
				$(RM) $(OBJS) $(OBJS_BONUS)
				@echo
	
fclean: 		clean 
				@echo "$(NAME): $(RED)$(NAME) was deleted$(RESET)"
				$(RM) $(NAME) 
				@echo

rebonus:		fclean bonus

re:				fclean
				$(MAKE) all

.PHONY:			all clean fclean re bonus rebonus
