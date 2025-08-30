#include "postgres.h"
#include "utils/guc.h"
#include "qdb_api.h"

/* GUC variables */
bool   qdb_enable = true;
double qdb_ci = 0.95;
double qdb_eps = 0.02;
int    qdb_max_shots = 20000;
int    qdb_qop_timeout_ms = 5000;
double qdb_cost_fee_lambda = 0.2;

void _PG_init(void); /* not used here; defined in qdb.c */

static void
assign_bounds_check_double(double *val, double minv, double maxv, const char *name)
{
  if (*val < minv) *val = minv;
  if (*val > maxv) *val = maxv;
  elog(LOG, "[qdb][guc] %s=%.6f", name, *val);
}

static void
assign_bounds_check_int(int *val, int minv, int maxv, const char *name)
{
  if (*val < minv) *val = minv;
  if (*val > maxv) *val = maxv;
  elog(LOG, "[qdb][guc] %s=%d", name, *val);
}

void
_PG_init_qdb_guc(void)
{
  /* registering from qdb.c already; but keep function should you choose split init */
}

/* Called via internal static initializer when library is loaded */
void
InitPostgresGUCs(void) {}

static bool
assign_enable_hook(bool newval, void *extra)
{
  elog(LOG, "[qdb][guc] enable=%s", newval ? "on" : "off");
  return true;
}

/* GUC registration happens at library load */
void _PG_init_guc(void) __attribute__((constructor));
void
_PG_init_guc(void)
{
  DefineCustomBoolVariable(
    "qdb.enable", "Enable qdb planner/executor hooks.",
    NULL, &qdb_enable, true, PGC_USERSET, 0, assign_enable_hook, NULL, NULL);

  DefineCustomRealVariable(
    "qdb.ci", "Target confidence level for quantum estimates.",
    NULL, &qdb_ci, 0.95, 0.5, 0.9999, PGC_USERSET, 0, NULL, NULL, NULL);

  DefineCustomRealVariable(
    "qdb.eps", "Target relative error bound.",
    NULL, &qdb_eps, 0.02, 0.0001, 0.5, PGC_USERSET, 0, NULL, NULL, NULL);

  DefineCustomIntVariable(
    "qdb.max_shots", "Max shots for quantum backend.",
    NULL, &qdb_max_shots, 20000, 1, 10000000, PGC_USERSET, 0, NULL, NULL, NULL);

  DefineCustomIntVariable(
    "qdb.qop_timeout_ms", "Quantum op timeout in ms.",
    NULL, &qdb_qop_timeout_ms, 5000, 100, 600000, PGC_USERSET, 0, NULL, NULL, NULL);

  DefineCustomRealVariable(
    "qdb.cost_fee_lambda", "Weight for fee in cost function.",
    NULL, &qdb_cost_fee_lambda, 0.2, 0.0, 10.0, PGC_USERSET, 0, NULL, NULL, NULL);
}
