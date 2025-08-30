compose-up:
\tdocker compose up -d --build

compose-down:
\tdocker compose down -v

pg-bash:
\tdocker exec -it qdb-pg bash

psql:
\tPGPASSWORD=postgres psql -h localhost -U postgres -d tpch -c "LOAD 'qdb'; SELECT qdb_version(); SHOW qdb.enable;"

qexe-logs:
\tdocker logs -f qdb-qexe
