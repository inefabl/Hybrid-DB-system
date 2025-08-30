from fastapi import FastAPI
from .routers import qae, qextrem, qsel
from .settings import Settings

app = FastAPI(title="qexe", version="0.1.0")
app.include_router(qae.router, prefix="/qop")
app.include_router(qextrem.router, prefix="/qop")
app.include_router(qsel.router, prefix="/qop")

@app.get("/healthz")
def healthz():
    return {"ok": True, "backend": "qpanda", "mode": Settings().backend_mode}
