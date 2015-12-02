all: skiplist bptree

skiplist: skiplist.c
	gcc skiplist.c -o skiplist

bptree: bptree.c
	gcc bptree.c -o bptree

clean:
	rm skiplist bptree
