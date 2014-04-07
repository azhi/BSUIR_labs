export DC=dmd
SHELL=/bin/bash
TIME=time

.PHONY: all clean gen perf test

all: hash tree

gen: ins_batch.txt search_batch.txt

perf: hash_perf.dat # tree_perf.dat

hash:
	$(DC) hash.d

tree:
	$(DC) tree.d

ins_batch.txt:
	./gen.sh add 50000 $@

search_batch.txt:
	./gen.sh find 250000 $@

hash_perf.dat: hash ins_batch.txt search_batch.txt
	$(TIME) sh -c "./hash --file $@ --batch ins_batch.txt"
	$(TIME) sh -c "./hash --file $@ --batch search_batch.txt"

tree_perf.dat: tree ins_batch.txt search_batch.txt
	$(TIME) sh -c "./tree --file $@ --batch ins_batch.txt"
	$(TIME) sh -c "./tree --file $@ --batch search_batch.txt"

test:
	make -C test

clean:
	-rm -rf hash tree hash_perf.dat tree_perf.dat ins_batch.txt search_batch.txt
	$(MAKE) -C test clean
