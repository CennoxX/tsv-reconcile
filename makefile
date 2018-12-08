all: reconcile.c 
	gcc -g -Wall -o reconcile reconcile.c

clean: 
	$(RM) reconcile
