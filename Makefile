SRCS		=	srcs/main.cpp							\
				srcs/config/Trio.cpp							\
				srcs/config/parse_config.cpp			\
				srcs/config/parse_context.cpp			\
				srcs/config/utils.cpp					\
			  	srcs/Server.cpp							\
			  	srcs/config/Config.cpp					\
			  	srcs/config/Location.cpp				\


OBJS		= ${SRCS:.cpp=.o}
CC			= c++
CFLAGS		= -std=c++98 -Wall -Wextra -Werror
RM			= rm -f
NAME		= webserv

all:		${NAME}

${NAME}:	${OBJS}
			$(CC) $(CFLAGS) -o $(NAME) ${OBJS}

clean:
			${RM} ${OBJS}

fclean:		clean
			${RM} ${NAME}

re:			fclean all

run:		
			./${NAME}

.PHONY:		all clean fclean re run