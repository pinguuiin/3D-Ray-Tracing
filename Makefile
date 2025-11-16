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
FLAGS = -Wall -Wextra -Werror -g -O3 -ffast-math -march=native -flto

LIBS = -ldl -lglfw -pthread -lm

LIBMLX_DIR = ./src/MLX42
LIBMLX = $(LIBMLX_DIR)/build/libmlx42.a

LIBFT_DIR = ./src/libft
LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_GIT = git@github.com:pinguuiin/Libft.git

INC_DIR = include

HEADERS = $(INC_DIR)/minirt.h $(INC_DIR)/vector.h $(INC_DIR)/hit.h \
		  $(INC_DIR)/get_next_line_revised.h $(INC_DIR)/parser.h
HEADER_DIR = -I./$(INC_DIR) -I$(LIBMLX_DIR)/$(INC_DIR) -I$(LIBFT_DIR)

SRC_DIR = ./src
OBJ_DIR = ./obj
BONUS_OBJ_DIR = ./obj_bonus

SRC_FILES = main.c \
			parser/handle_argv.c \
			parser/parser.c \
			parser/parse_setting.c \
			parser/parse_objects.c \
			parser/parsing_utils.c \
			parser/error_handling.c \
			parser/ft_strtod.c \
			parser/parse_triad.c \
			parser/get_next_line_revised.c \
			parser/get_next_line_utils.c \
			hooks/camera_hooks.c \
			hooks/hook_utils.c \
			raytracer/preprocessor.c \
			raytracer/reflection.c \
			raytracer/renderer.c \
			shapes/cylinder.c \
			shapes/plane.c \
			shapes/sphere.c \
			utils/color.c \
			vector/operations.c \
			vector/rotation.c

SRCS = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJS = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))
BONUS_OBJS = $(addprefix $(BONUS_OBJ_DIR)/, $(SRC_FILES:.c=.o))

.SECONDARY: $(OBJS) $(BONUS_OBJS) $(LIBMLX) $(LIBFT)

all: $(NAME)

$(LIBMLX):
	@if [ ! -d "$(LIBMLX_DIR)" ]; then git clone https://github.com/codam-coding-college/MLX42.git $(LIBMLX_DIR); fi
	@cmake $(LIBMLX_DIR) -B $(LIBMLX_DIR)/build && make --no-print-directory -C $(LIBMLX_DIR)/build -j4

$(LIBFT): phony
	@if [ ! -d "$(LIBFT_DIR)" ]; then git clone $(LIBFT_GIT) $(LIBFT_DIR); fi
	@make --no-print-directory -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(LIBMLX) $(LIBFT)
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) $(HEADER_DIR) -c $< -o $@
	@echo "$(BGREEN) Compiled $(notdir $<) $(RESET_COLOR)"

$(BONUS_OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(LIBMLX) $(LIBFT)
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) -D BONUS $(HEADER_DIR) -c $< -o $@
	@echo "$(BYELLOW) Bonus compiled $(notdir $<) $(RESET_COLOR)"

$(NAME): $(LIBMLX) $(LIBFT) $(OBJS)
	$(CC) $(FLAGS) $(OBJS) $(LIBMLX) $(LIBFT) $(LIBS) -o $(NAME)
	@echo "$(BPURPLE) Linked $(RESET_COLOR)"

clean:
	if [ -d "$(OBJ_DIR)" ]; then rm -rf $(OBJ_DIR); fi
	if [ -d "$(BONUS_OBJ_DIR)" ]; then rm -rf $(BONUS_OBJ_DIR); fi
	@if [ -d "$(LIBFT_DIR)" ]; then make clean -C $(LIBFT_DIR); fi
	@if [ -d "$(LIBMLX_DIR)" ]; then make clean -C $(LIBMLX_DIR)/build; fi
	@echo "$(BBLUE) Cleaned .o files $(RESET_COLOR)"

fclean:
	@if [ -f "$(NAME)" ] || [ -d "$(OBJ_DIR)" ] || [ -d "$(BONUS_OBJ_DIR)" ] || [ -d "$(LIBFT_DIR)" ] || [ -d "$(LIBMLX_DIR)" ]; then \
		if [ -d "$(OBJ_DIR)" ]; then rm -rf "$(OBJ_DIR)"; fi; \
		if [ -d "$(BONUS_OBJ_DIR)" ]; then rm -rf "$(BONUS_OBJ_DIR)"; fi; \
		if [ -d "$(LIBFT_DIR)" ]; then make -s fclean -C "$(LIBFT_DIR)"; rm -rf "$(LIBFT_DIR)"; fi; \
		if [ -d "$(LIBMLX_DIR)" ]; then make -s clean -C "$(LIBMLX_DIR)/build"; rm -rf "$(LIBMLX_DIR)"; fi; \
		if [ -f "$(NAME)" ]; then rm -rf "$(NAME)"; fi; \
		rm -f .bonus; \
		echo "$(BBLUE) Cleaned all $(RESET_COLOR)"; \
	else \
		echo "$(BCYAN) Nothing to clean. Everything looks tidy. $(RESET_COLOR)"; \
	fi;

re: fclean all

bonus: .bonus

.bonus: $(LIBMLX) $(LIBFT) $(BONUS_OBJS)
	@$(CC) $(FLAGS) -D BONUS $(BONUS_OBJS) $(LIBMLX) $(LIBFT) $(LIBS) -o $(NAME)
	@echo "$(BYELLOW) Built with bonus rules $(RESET_COLOR)"
	@touch .bonus

.PHONY: all clean fclean re phony bonus
