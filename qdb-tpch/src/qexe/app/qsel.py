from fastapi import APIRouter
from pydantic import BaseModel
from math import sqrt
import time

router = APIRouter()

class QSelReq(BaseModel):
    kind: str                  # predicate | join
    left: str | None = None
    right: str | None = None
    on: str | None = None
    predicate: str | None = None
    target_ci: float = 0.9
    target_eps: float = 0.05
    max_shots: int = 10000
    timeout_ms: int = 3000

class QSelResp(BaseModel):
    selectivity: float
    ci_low: float
    ci_high: float
    shots_used: int
    t_ms: int
    backend: str = "sim"

@router.post("/qsel", response_model=QSelResp)
def qsel(req: QSelReq):
    z = 1.64  # ~90%
    p = 0.1
    shots = min(req.max_shots, max(100, int((z*z * p*(1-p)) / (req.target_eps**2) )))
    half = z*sqrt(p*(1-p)/shots)
    t0 = time.time()
    return QSelResp(
        selectivity=p, ci_low=max(0.0, p-half), ci_high=min(1.0, p+half),
        shots_used=shots, t_ms=int((time.time()-t0)*1000), backend="sim"
    )
