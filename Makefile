SRCS		=	srcs/main.cpp							\
				srcs/Trio.cpp							\
				srcs/config/parse_config.cpp			\
				srcs/config/parse_context.cpp			\
				srcs/config/utils.cpp					\
			  	srcs/Monitor.cpp						\
			  	srcs/Server.cpp							\
			  	srcs/Request.cpp						\
			  	srcs/Message.cpp						\
			  	srcs/Location.cpp						\
			  	srcs/utils.cpp							\
			  	# srcs/Response.cpp						


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