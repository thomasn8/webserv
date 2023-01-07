CC			= c++
CFLAGS		= -std=c++98 -Wall -Wextra -Werror
RM			= rm -f
NAME1		= select
NAME2		= poll

all:		${NAME1}

SRCS1	=	selectServer.cpp
SRCS1	=	pollServer.cpp
OBJS1	=	${SRCS1:.cpp=.o}
select:		${OBJS1}
			$(CC) $(CFLAGS) -o $(NAME1) ${OBJS1}

SRCS2	=	pollServer.cpp
OBJS2	=	${SRCS2:.cpp=.o}
poll:		${OBJS2}
			$(CC) $(CFLAGS) -o $(NAME2) ${OBJS2}

clean:
			${RM} ${OBJS1}
			${RM} ${OBJS2}

fclean:		clean
			${RM} ${NAME1}
			${RM} ${NAME2}

re:			fclean all
run:		
			./${NAME1}

.PHONY:		all clean fclean re run