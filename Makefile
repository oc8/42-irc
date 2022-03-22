NAME				= ircserv
NAME_BOT			= ircbot
LST_SRCS			= main.cpp \
server.cpp \
pars.cpp \
channel.cpp \
user.cpp \
commands2.cpp \
command.cpp \
utils.cpp
LST_SRCS_BOT	= main_bot.cpp \
Bot.cpp
SRCS_DIR			= srcs
OBJS_DIR			= objs
SRCS				= $(addprefix $(SRCS_DIR)/,$(LST_SRCS))
SRCS_BOT			= $(addprefix bot/$(SRCS_DIR)/,$(LST_SRCS))
OBJS				= $(LST_SRCS:%.cpp=$(OBJS_DIR)/%.o)
OBJS_BOT			= $(LST_SRCS_BOT:%.cpp=$(OBJS_DIR)/%.o)
CXXC				= clang++
FLAGS				= -Wall -Wextra -Werror -std=c++98 -pedantic-errors
FLAGS				= -fsanitize=address -fsanitize=undefined -Wall -Wextra -Werror -std=c++98 -pedantic-errors -g
CXXFLAGS			= $(FLAGS) -Iinc
CXXFLAGS_BOT	= $(FLAGS) -Ibot/inc
RM					= rm -rf
MKDIR				= mkdir -p
INC				= $(shell find ./inc -type f -name "*.hpp")
INC_BOT			= $(shell find ./bot/inc -type f -name "*.hpp")

all:			$(NAME) $(NAME_BOT)

$(OBJS_DIR):
				$(MKDIR) $@

$(NAME):		print $(OBJS_DIR) $(OBJS)
				$(CXXC) $(CXXFLAGS) $(OBJS) -o $(NAME)
				@printf "$(ERASE)$(ARROW)└─> $(FINISH)generate$(ARROW)\n$(END)"

$(NAME_BOT):print_bot $(OBJS_DIR) $(OBJS_BOT)
				$(CXXC) $(CXXFLAGS) $(OBJS_BOT) -o $(NAME_BOT)
				@printf "$(ERASE)$(ARROW)└─> $(FINISH)generate$(ARROW)\n$(END)"

$(OBJS_DIR)/%.o:$(SRCS_DIR)/%.cpp	$(INC)
				$(MKDIR) $(dir $@)
				@printf "$(ERASE)$(ARROW)└─[$(ACTION)$<$(ARROW)]"
				$(CXXC) $(CXXFLAGS) -o $@ -c $<

$(OBJS_DIR)/%.o:bot/$(SRCS_DIR)/%.cpp	$(INC_BOT)
				$(MKDIR) $(dir $@)
				@printf "$(ERASE)$(ARROW)└─[$(ACTION)$<$(ARROW)]"
				$(CXXC) $(CXXFLAGS_BOT) -o $@ -c $<

print:
				@printf "$(ERASE)$(BOLD)$(ARROW)┌──<$(TITLE)$(NAME)$(ARROW)>\n$(END)"

print_bot:
				@printf "$(ERASE)$(BOLD)$(ARROW)┌──<$(TITLE)$(NAME_BOT)$(ARROW)>\n$(END)"

clean:
				@printf "$(BOLD)$(ARROW)┌──<$(TITLE)ft_irc$(ARROW)>\n$(END)"
				$(RM) $(OBJS_DIR)
				@printf "$(ERASE)$(ARROW)└─> $(FINISH)clean$(ARROW)\n$(END)"

fclean:		clean
				$(RM) $(NAME)

re:				fclean all

.PHONY: 		clean fclean all re print print_bot
.SILENT:		clean fclean all re $(OBJS) $(OBJS_BOT) $(NAME) $(NAME_BOT) $(OBJS_DIR)

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
