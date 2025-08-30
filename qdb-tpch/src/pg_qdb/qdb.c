#include "postgres.h"
#include "fmgr.h"
#include "optimizer/planner.h"
#include "executor/executor.h"
#include "miscadmin.h"

#include "qdb_api.h"

PG_MODULE_MAGIC;

/* Keep previous hooks */
static planner_hook_type prev_planner_hook = NULL;
static ExecutorRun_hook_type prev_ExecutorRun = NULL;

void _PG_init(void);
void _PG_fini(void);

static PlannedStmt *qdb_planner_entry(Query *parse, int cursorOptions, ParamListInfo boundParams);
static void qdb_ExecutorRun_entry(QueryDesc *queryDesc, ScanDirection direction, uint64 count, bool execute_once);

void
_PG_init(void)
{
  /* install planner hook */
  prev_planner_hook = planner_hook;
  planner_hook = qdb_planner_entry;

  /* install executor run hook */
  prev_ExecutorRun = ExecutorRun_hook;
  ExecutorRun_hook = qdb_ExecutorRun_entry;

  elog(LOG, "[qdb] extension loaded (pid=%d)", MyProcPid);
}

void
_PG_fini(void)
{
  planner_hook = prev_planner_hook;
  ExecutorRun_hook = prev_ExecutorRun;
  elog(LOG, "[qdb] extension unloaded (pid=%d)", MyProcPid);
}

static PlannedStmt *
qdb_planner_entry(Query *parse, int cursorOptions, ParamListInfo boundParams)
{
  if (!prev_planner_hook)
    return qdb_planner_hook(parse, cursorOptions, boundParams);
  return prev_planner_hook(parse, cursorOptions, boundParams);
}

static void
qdb_ExecutorRun_entry(QueryDesc *qd, ScanDirection direction, uint64 count, bool execute_once)
{
  if (!prev_ExecutorRun)
    qdb_ExecutorRun(qd, direction, count, execute_once);
  else
    prev_ExecutorRun(qd, direction, count, execute_once);
}
