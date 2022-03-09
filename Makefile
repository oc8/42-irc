NAME			= ircserv

LST_SRCS		= main.cpp \
server.cpp \
pars.cpp \
utils.cpp
SRCS_DIR		= srcs
OBJS_DIR		= objs
SRCS			= $(addprefix $(SRCS_DIR)/,$(LST_SRCS))
OBJS			= $(LST_SRCS:%.cpp=$(OBJS_DIR)/%.o)
CXXC			= clang++
FLAGS			= -Wall -Wextra -Werror -std=c++98 -pedantic-errors
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
				$(CXXC) $(CXXFLAGS) -o $@ -c $<
				@printf "$(ERASE)$(ARROW)└─[$(ACTION)$<$(ARROW)]"

client:
				@printf "$(BOLD)$(ARROW)┌──<$(TITLE)client$(ARROW)>\n"
				@$(CXXC) $(CXXFLAGS) -o client -c srcs/client.cpp
				@printf "$(ERASE)└─> $(FINISH)generate$(ARROW)\n$(END)"

client:			$(OBJS_DIR)/client.o
				$(CXXC) $(CXXFLAGS) $(OBJS_DIR)/client.o -o client
				printf "$(ERASE)└─> $(FINISH)generate client$(ARROW)\n$(END)"

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
