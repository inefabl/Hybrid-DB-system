-- SQL install script for qdb
CREATE FUNCTION qdb_version() RETURNS text
AS $$ SELECT 'qdb 1.0'::text; $$
LANGUAGE sql IMMUTABLE;

-- Telemetry table (minimal demo)
CREATE TABLE IF NOT EXISTS qdb_telemetry_qop_demo(
  ts timestamptz default now(),
  phase text,
  note text
);
