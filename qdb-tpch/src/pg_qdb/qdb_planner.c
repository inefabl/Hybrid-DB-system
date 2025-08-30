#include "postgres.h"
#include "optimizer/planner.h"
#include "nodes/relation.h"
#include "nodes/nodeFuncs.h"
#include "utils/guc.h"
#include "utils/lsyscache.h"

#include "qdb_api.h"

PlannedStmt *
qdb_planner_hook(Query *parse, int cursorOptions, ParamListInfo boundParams)
{
  PlannedStmt *plan;

  /* Call stock planner first to keep skeleton simple */
  plan = standard_planner(parse, cursorOptions, boundParams);

  if (qdb_enable)
  {
    /* Minimal demo: log basic info. In real impl, scan plan tree to attach QNodes */
    elog(LOG, "[qdb][planner] query planned: cmd=%d ci=%.3f eps=%.3f max_shots=%d",
         (int) parse->commandType, qdb_ci, qdb_eps, qdb_max_shots);
  }

  return plan;
}
