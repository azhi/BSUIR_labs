.PHONY: all clean test_hash test_tree

all: test_hash test_tree

test_hash: hash
	@if ./hash; then echo "Hash: OK"; else echo "Hash: FAILED"; fi

hash:
	$(DC) ../hash.d -g -unittest

test_tree: tree
	@if ./tree; then echo "Tree: OK"; else echo "Tree: FAILED"; fi

tree:
	$(DC) ../tree.d -g -unittest

clean:
	-rm -rf hash tree
