all: skiplist bptree merge_sort quick_sort heap_sort binary_search_tree shell_sort

skiplist: skiplist.c
	gcc skiplist.c -o skiplist

bptree: bptree.c
	gcc bptree.c -o bptree

merge_sort: merge_sort.c
	gcc merge_sort.c -o merge_sort

quick_sort: quick_sort.c
	gcc quick_sort.c -o quick_sort

heap_sort: heap_sort.c
	gcc heap_sort.c -o heap_sort
    
binary_search_tree: binary_search_tree.c
	gcc binary_search_tree.c -o binary_search_tree

shell_sort: shell_sort.c
	gcc shell_sort -o shell_sort

clean:
	rm skiplist bptree merge_sort quick_sort heap_sort binary_search_tree shell_sort
