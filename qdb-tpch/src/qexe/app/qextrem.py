from fastapi import APIRouter
from pydantic import BaseModel
import time

router = APIRouter()

class QExtReq(BaseModel):
    kind: str           # min | max | topk
    k: int | None = None
    table: str
    predicate: str | None = None
    score_expr: str
    approx_score: dict | None = None
    max_iters: int = 30
    timeout_ms: int = 8000

class QExtResp(BaseModel):
    items: list[dict]   # [{id: ..., score: ...}, ...]
    iters_used: int
    t_ms: int
    backend: str = "sim"

@router.post("/qextrem", response_model=QExtResp)
def qextrem(req: QExtReq):
    t0 = time.time()
    k = req.k or 1
    # Mocked result
    items = [{"id": f"X{i}", "score": 1000 - i*3.14} for i in range(k)]
    return QExtResp(items=items, iters_used=min(req.max_iters, 10), t_ms=int((time.time()-t0)*1000))
