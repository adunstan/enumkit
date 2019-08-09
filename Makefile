
MODULES = $(TYPENAME)

DATA_built = $(TYPENAME)-install.sql

ENUMS = junk

SRCS += $(TYPENAME).c
OBJS = $(SRCS:.c=.o)

PGXS := $(shell pg_config --pgxs)
include $(PGXS)

$(TYPENAME).c: generic.c
	sed -e 's/TYPENAME/$(TYPENAME)/g' -e 's/TYPEVALS/$(ENUMS)/' $< > $@

$(TYPENAME).sql:	generic.sql
	sed -e 's/TYPENAME/$(TYPENAME)/g' $< > $@

$(TYPENAME)-install.sql: generic-install.sql
	sed -e 's/TYPENAME/$(TYPENAME)/g' $< > $@
