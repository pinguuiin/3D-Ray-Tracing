BBLACK=\033[1;30m
BRED=\033[1;31m
BGREEN=\033[1;32m
BYELLOW=\033[1;33m
BBLUE=\033[1;34m
BPURPLE=\033[1;35m
BCYAN=\033[1;36m
BWHITE=\033[1;37m
RESET_COLOR=\033[0m

NAME = miniRT
CC = cc
FLAGS = -Wall -Wextra -Werror -g

LIBS = -ldl -lglfw -pthread -lm

LIBMLX_DIR = ./src/MLX42
LIBMLX = $(LIBMLX_DIR)/build/libmlx42.a

LIBFT_DIR = ./src/libft
LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_GIT = git@github.com:pinguuiin/Libft.git

INC_DIR = include

HEADERS = $(INC_DIR)/minirt.h $(INC_DIR)/vector.h $(INC_DIR)/parser.h \
		  $(INC_DIR)/get_next_line_minirt.h
HEADER_DIR = -I./$(INC_DIR) -I$(LIBMLX_DIR)/$(INC_DIR) -I$(LIBFT_DIR)

SRC_DIR = ./src
OBJ_DIR = ./obj

SRC_FILES = hooks/camera_hooks.c \
			raytracer/reflection.c \
			raytracer/renderer.c \
			shapes/cylinder.c \
			shapes/plane.c \
			shapes/sphere.c \
			utils/color.c \
			vector/operations.c \
			vector/rotation.c \
			main.c \
			parser.c \
			parse_setting.c \
			parse_objects.c \
			parsing_utils.c \
			ft_strtod.c \
			parse_color.c \
			get_next_line_minirt.c \
			get_next_line_utils.c
#			test.c

SRCS = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJS = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

.SECONDARY: $(OBJS) $(LIBMLX) $(LIBFT)

all: $(NAME)

$(LIBMLX):
	@if [ ! -d "$(LIBMLX_DIR)" ]; then git clone https://github.com/codam-coding-college/MLX42.git $(LIBMLX_DIR); fi
	@cmake $(LIBMLX_DIR) -B $(LIBMLX_DIR)/build && make --no-print-directory -C $(LIBMLX_DIR)/build -j4

$(LIBFT):
	@if [ ! -d "$(LIBFT_DIR)" ]; then git clone $(LIBFT_GIT) $(LIBFT_DIR); fi
	@make --no-print-directory -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) $(HEADER_DIR) -c $< -o $@
	@echo "$(BGREEN) Compiled $^ $(RESET_COLOR)"

$(NAME): $(LIBMLX) $(LIBFT) $(OBJS)
	$(CC) $(FLAGS) $(OBJS) $(LIBMLX) $(LIBFT) $(LIBS) -o $(NAME)
	@echo "$(BPURPLE) Linked $(RESET_COLOR)"

clean:
	if [ -d "$(OBJ_DIR)" ]; then rm -rf $(OBJ_DIR); fi
	@if [ -d "$(LIBFT_DIR)" ]; then make clean -C $(LIBFT_DIR); fi
	@if [ -d "$(LIBMLX_DIR)" ]; then make clean -C $(LIBMLX_DIR)/build; fi
	@echo "$(BBLUE) Cleaned .o files $(RESET_COLOR)"

fclean: clean
	@make fclean -C $(LIBFT_DIR)
	@rm -rf $(LIBMLX_DIR) $(LIBFT_DIR)
	rm -rf $(NAME)
	@echo "$(BBLUE) Cleaned all $(RESET_COLOR)"

re: fclean all

.PHONY: all clean fclean re
