SUBDIRS := $(wildcard src/*/.)

.PHONY: all $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done

test:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir test; \
	done