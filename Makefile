build:
	dune build

doc:
	dune build @doc

test:
	dune runtest

all: build doc test

install: build
	dune install

uninstall:
	dune uninstall

clean:
	dune clean

.PHONY: build doc test all install uninstall clean

VERSION=0.4
NAME=ocaml-gperftools-$(VERSION)

.PHONY: release
release:
	git tag -a -m $(VERSION) v$(VERSION)
	git archive --prefix=$(NAME)/ v$(VERSION) | gzip > $(NAME).tar.gz
	gpg -a -b $(NAME).tar.gz
