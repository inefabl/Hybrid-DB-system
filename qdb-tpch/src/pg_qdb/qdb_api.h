#ifndef QDB_API_H
#define QDB_API_H

#include "nodes/pathnodes.h"
#include "nodes/plannodes.h"
#include "executor/executor.h"

/* Exposed by qdb_planner.c / qdb_executor.c */
extern PlannedStmt *qdb_planner_hook(Query *parse, int cursorOptions, ParamListInfo boundParams);
extern void qdb_ExecutorRun(QueryDesc *queryDesc, ScanDirection direction, uint64 count, bool execute_once);

/* GUCs (defined in qdb_guc.c) */
extern bool qdb_enable;
extern double qdb_ci;
extern double qdb_eps;
extern int qdb_max_shots;
extern int qdb_qop_timeout_ms;
extern double qdb_cost_fee_lambda;

#endif
