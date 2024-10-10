NAME 			= pipex
NAME_BONUS		= pipex_bonus

GREEN           = \033[0;32m
RED             = \033[0;31m
RESET           = \033[0m

LIBFT           = ./libraries/Libft/libft.a
CC = cc
INCLUDE_DIRS    = -I./libraries/Libft
CFLAGS = -Wall -Werror -Wextra -g3

SRCS_DIR        = ./sources/
BONUS_SRCS_DIR  = ./bonus_sources/

SRCS = $(SRCS_DIR)pipex.c
# SRCS_BONUS = $(BONUS_SRCS_DIR)pipex_bonus.c

OBJS = $(SRCS:.c=.o)
# OBJS_BONUS = $(SRCS_BONUS:.c=.o)

REMOVE = rm -f

# 默认目标
all: 			${LIBFT} $(NAME)

# 链接目标文件生成最终的可执行文件
$(NAME): 		$(OBJS)
				$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)
				@echo "$(NAME): $(GREEN)$(NAME) was compiled.$(RESET)"
				@echo

# bonus: $(OBJS_BONUS)
# 				$(CC) $(CFLAGS) $(OBJS_BONUS) $(LIBFT) -o $(NAME_BONUS)
#                 @echo "$(NAME_BONUS): $(GREEN)$(NAME_BONUS) was compiled with Bonus.$(RESET)"
# 				@echo

%.o: %.c
				$(CC) $(CFLAGS) -c $< -o $@   

${LIBFT}:
				@echo
				make bonus -C libraries/Libft

clean:
				make clean -C libraries/Libft
				${REMOVE} $(OBJS) 
				# ${REMOVE} $(OBJS_BONUS)
				@echo "Cleaned object files."

# 完全清理，包含删除可执行文件
fclean: 		clean
				${REMOVE} ${NAME} ${NAME_BONUS} ${LIBFT}
				@echo "${NAME}: ${RED}${NAME} ${LIBFT} and ${NAME_BONUS} were deleted${RESET}"
				@echo

re: 			fclean all

# 声明伪目标
.PHONY: all clean fclean re bonus
