all: skiplist bptree merge_sort

skiplist: skiplist.c
	gcc skiplist.c -o skiplist

bptree: bptree.c
	gcc bptree.c -o bptree

merge_sort: merge_sort.c
	gcc merge_sort.c -o merge_sort

clean:
	rm skiplist bptree merge_sort
