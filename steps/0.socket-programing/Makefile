CC			= c++
CFLAGS		= -std=c++98 -Wall -Wextra -Werror
RM			= rm -f
NAME1		= loop
NAME2		= server
NAME3		= client

all:		${NAME1}

# server en loop mode 
SRCS1	=	socket_loop.cpp
OBJS1	=	${SRCS1:.cpp=.o}
loop:		${OBJS1}
			$(CC) $(CFLAGS) -o $(NAME1) ${OBJS1}

# server en 1 request mode 
SRCS2	=	socket.cpp
OBJS2	=	${SRCS2:.cpp=.o}
server:		${OBJS2}
			$(CC) $(CFLAGS) -o $(NAME2) ${OBJS2}

# 1 client TCP request pour tester la communication avec le server 
SRCS3	=	client.cpp
OBJS3	=	${SRCS3:.cpp=.o}
client:		${OBJS3}
			$(CC) $(CFLAGS) -o $(NAME3) ${OBJS3}

clean:
			${RM} ${OBJS1}
			${RM} ${OBJS2}
			${RM} ${OBJS3}

fclean:		clean
			${RM} ${NAME1}
			${RM} ${NAME2}
			${RM} ${NAME3}

re:			fclean all
run:		
			./${NAME1}

.PHONY:		all clean fclean re run loop server client