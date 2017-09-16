NAME = quartic

all:
	gcc -o $(NAME) $(NAME).c

clean:
	rm $(NAME).o

fclean: clean
	rm $(NAME)

re: fclean all
