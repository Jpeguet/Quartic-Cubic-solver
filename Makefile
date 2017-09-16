NAME = quartic
NAMEC = cubic

all:
	@gcc -o $(NAME) $(NAME).c
	@gcc -o $(NAMEC) $(NAMEC).c

fclean:
	@rm $(NAME)
	@rm $(NAMEC)

re: fclean all
