a.out : rb_tree.o receive.o CacheBuff.o
	cc -lpthread rb_tree.o receive.o CacheBuff.o
	
rb_tree.o : rb_tree.c rb_tree.h
	cc -g -c rb_tree.c 
receive.o : receive.c rb_tree.h CacheBuff.h
	cc   -lpthread -g -c  receive.c 
CacheBuff.o : CacheBuff.c  rb_tree.h CacheBuff.h 
	cc -g -c CacheBuff.c 


clean: 
	rm -rf rb_tree.o receive.o CacheBuff.o
	rm -rf *.gch a.out