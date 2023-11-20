SUBDIRS := $(wildcard src/*/.)

.PHONY: all $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@
	mv $@/*.o .

clean:
	rm -f *.o