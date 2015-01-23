all: search index

search: search.c
	gcc -o search search.c
index: index.c 
	gcc -o index index.c 
clean:
	rm search; rm index
