a.out : rb_tree.o testnumber.o CacheBuff.o
	cc rb_tree.o testnumber.o CacheBuff.o
rb_tree.o : rb_tree.c rb_tree.h
	cc -g -c rb_tree.c rb_tree.h
testnumber.o : testnumber.c rb_tree.h
	cc -g -c testnumber.c rb_tree.h
CacheBuff.o : CacheBuff.c  rb_tree.h CacheBuff.h 
	cc -g -c CacheBuff.c  rb_tree.h CacheBuff.h 
clean: 
	rm -rf rb_tree.o testnumber.o CacheBuff.o
	rm -rf *.gch