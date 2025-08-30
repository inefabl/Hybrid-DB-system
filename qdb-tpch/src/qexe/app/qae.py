from fastapi import APIRouter
from pydantic import BaseModel
from math import sqrt
import time

router = APIRouter()

class QAEReq(BaseModel):
    kind: str                 # ratio | mean | sum
    table: str
    predicate: str | None = None
    measure: str | None = None
    group_by: list[str] = []
    target_ci: float = 0.95
    target_eps: float = 0.02
    max_shots: int = 20000
    timeout_ms: int = 5000
    telemetry: dict | None = None

class QAEResp(BaseModel):
    p_hat: float | None = None
    mean: float | None = None
    sum: float | None = None
    ci_low: float | None = None
    ci_high: float | None = None
    shots_used: int
    t_ms: int
    backend: str = "sim"

@router.post("/qae", response_model=QAEResp)
def qae(req: QAEReq):
    # Very small simulation: derive shots from eps & ci, fabricate a plausible interval
    z = 1.96 if abs(req.target_ci - 0.95) < 1e-6 else 2.58
    p_prior = 0.2
    shots = min(req.max_shots, max(100, int((z*z * p_prior*(1-p_prior)) / (req.target_eps**2) )))
    t0 = time.time()

    # Fake stats
    phat = 0.2
    half = z*sqrt(phat*(1-phat)/shots)
    ci_low, ci_high = max(0.0, phat-half), min(1.0, phat+half)

    payload = QAEResp(
        p_hat = phat if req.kind == "ratio" else None,
        mean  = 123.45 if req.kind == "mean" else None,
        sum   = 1.23e9 if req.kind == "sum"  else None,
        ci_low= ci_low, ci_high= ci_high,
        shots_used = shots,
        t_ms = int((time.time()-t0)*1000),
        backend="sim"
    )
    return payload
