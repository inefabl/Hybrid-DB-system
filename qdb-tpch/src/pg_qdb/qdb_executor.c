#include "postgres.h"
#include "executor/executor.h"
#include "tcop/utility.h"
#include "utils/builtins.h"

#include "qdb_api.h"

void
qdb_ExecutorRun(QueryDesc *qd, ScanDirection direction, uint64 count, bool execute_once)
{
  if (qdb_enable)
  {
    /* Minimal demo: write one telemetry row per execution start */
    StartTransactionCommand();
    PushActiveSnapshot(GetTransactionSnapshot());
    (void) SPI_connect();
    SPI_execute("INSERT INTO qdb_telemetry_qop_demo(phase, note) VALUES('exec_run','qdb executor entered')", false, 0);
    SPI_finish();
    PopActiveSnapshot();
    CommitTransactionCommand();

    elog(LOG, "[qdb][exec] run with ci=%.3f eps=%.3f timeout=%dms", qdb_ci, qdb_eps, qdb_qop_timeout_ms);
  }

  /* Always delegate to standard executor */
  standard_ExecutorRun(qd, direction, count, execute_once);
}
