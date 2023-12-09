SUBDIRS := $(wildcard src/*/.)

.PHONY: all $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
	rm -f *.o
	rm -rf ASDIR/*

test:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir test; \
	done

print:
	./printas.sh


