NAME			= ircserv

LST_SRCS		= main.cpp \
server.cpp \
pars.cpp \
channel.cpp \
user.cpp \
commands2.cpp \
command.cpp \
bot.cpp \
utils.cpp
SRCS_DIR		= srcs
OBJS_DIR		= objs
SRCS			= $(addprefix $(SRCS_DIR)/,$(LST_SRCS))
OBJS			= $(LST_SRCS:%.cpp=$(OBJS_DIR)/%.o)
CXXC			= clang++
FLAGS			= -Wall -Wextra -Werror -std=c++98 -pedantic-errors
# FLAGS			= -fsanitize=address -fsanitize=undefined -Wall -Wextra -Werror -std=c++98 -pedantic-errors -g
CXXFLAGS		= $(FLAGS) -Iinc
RM				= rm -rf
MKDIR			= mkdir -p
INC			= $(shell find ./inc -type f -name "*.hpp")

all:			$(NAME)

$(OBJS_DIR):
				$(MKDIR) $@

$(NAME):		print $(OBJS_DIR) $(OBJS)
				$(CXXC) $(CXXFLAGS) $(OBJS) -o $(NAME)
				@printf "$(ERASE)$(ARROW)└─> $(FINISH)generate$(ARROW)\n$(END)"

$(OBJS_DIR)/%.o:$(SRCS_DIR)/%.cpp	$(INC)
				$(MKDIR) $(dir $@)
				@printf "$(ERASE)$(ARROW)└─[$(ACTION)$<$(ARROW)]"
				$(CXXC) $(CXXFLAGS) -o $@ -c $<

print:
				@printf "$(BOLD)$(ARROW)┌──<$(TITLE)$(NAME)$(ARROW)>\n$(END)"

clean:		print
				$(RM) $(OBJS_DIR)
				@printf "$(ERASE)$(ARROW)└─> $(FINISH)clean$(ARROW)\n$(END)"

fclean:		clean
				$(RM) $(NAME)

re:				fclean all

.PHONY: 		clean fclean all re
.SILENT:		clean fclean all re $(OBJS) $(NAME) $(OBJS_DIR) print client

ERASE		= \033[2K\r
GREY		= \033[30m
RED		= \033[31m
GREEN		= \033[32m
YELLOW	= \033[33m
BLUE		= \033[34m
PINK		= \033[35m
CYAN		= \033[36m
WHITE		= \033[37m
END		= \033[0m
BOLD		= \033[1m
UNDER		= \033[4m
SUR		= \033[7m

TITLE		= $(GREEN)$(BOLD)
ARROW		= $(GREY)$(BOLD)
ACTION	= $(YELLOW)
FINISH	= $(BLUE)$(BOLD)
