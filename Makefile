# OASIS_START
# DO NOT EDIT (digest: bc1e05bfc8b39b664f29dae8dbd3ebbb)

SETUP = ocaml setup.ml

build: setup.data
	$(SETUP) -build $(BUILDFLAGS)

doc: setup.data build
	$(SETUP) -doc $(DOCFLAGS)

test: setup.data build
	$(SETUP) -test $(TESTFLAGS)

all: 
	$(SETUP) -all $(ALLFLAGS)

install: setup.data
	$(SETUP) -install $(INSTALLFLAGS)

uninstall: setup.data
	$(SETUP) -uninstall $(UNINSTALLFLAGS)

reinstall: setup.data
	$(SETUP) -reinstall $(REINSTALLFLAGS)

clean: 
	$(SETUP) -clean $(CLEANFLAGS)

distclean: 
	$(SETUP) -distclean $(DISTCLEANFLAGS)

setup.data:
	$(SETUP) -configure $(CONFIGUREFLAGS)

.PHONY: build doc test all install uninstall reinstall clean distclean configure

# OASIS_STOP

VERSION=$(shell oasis query version)
NAME=ocaml-gperftools-$(VERSION)

.PHONY: release
release:
	git tag -a -m $(VERSION) v$(VERSION)
	rm -rf stage
	mkdir stage
	git archive --prefix=$(NAME)/ v$(VERSION) | tar --delete $(NAME)/web | tar x -C stage
	(cd stage/$(NAME)/ && oasis setup -setup-update none && cd .. && tar czf ../$(NAME).tar.gz $(NAME))
	rm -rf stage
	gpg -a -b $(NAME).tar.gz
