EXTENSION = qdb
MODULE_big = qdb
OBJS = qdb.o qdb_planner.o qdb_executor.o qdb_guc.o

DATA = qdb--1.0.sql
PGFILEDESC = "qdb - hybrid quantum/classical hooks and GUCs"

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
