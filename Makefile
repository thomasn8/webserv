SRCS		=	srcs/main.cpp							\
				srcs/Trio.cpp							\
				srcs/config/parse_config.cpp			\
				srcs/config/parse_context.cpp			\
				srcs/config/utils.cpp					\
				srcs/config/Monitor_log_config.cpp		\
			  	srcs/Monitor.cpp						\
			  	srcs/Server.cpp							\
			  	srcs/Request.cpp						\
			  	srcs/MultipartData.cpp					\
			  	srcs/Location.cpp						\
				srcs/utils.cpp							\
			  	srcs/Response.cpp						\

OBJS		= ${SRCS:.cpp=.o}
CC			= c++
CFLAGS		= -Wall -Wextra -Werror
RM			= rm -f
NAME		= webserv

all:		${NAME}

.cpp.o:
			${CC} ${CFLAGS} -c $< -o ${<:.cpp=.o}

${NAME}:	${OBJS}
			$(CC) $(CFLAGS) -o $(NAME) ${OBJS}

clean:
			${RM} ${OBJS}

fclean:		clean
			${RM} ${NAME}
			${RM} logs/access.log && touch logs/access.log
			${RM} client.o

re:			fclean all

run:		
			./${NAME}

# #######################
# programme de test perso
client:
			c++ -Wall -Wextra -Werror client.cpp -o client
run-client:
			./client
# #######################

.PHONY:		all clean fclean re run client run-client
