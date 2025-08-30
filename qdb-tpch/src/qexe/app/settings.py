from pydantic import BaseSettings

class Settings(BaseSettings):
    qpanda_api_key: str = "DUMMY_KEY"
    qpanda_endpoint: str = "https://api.qpanda.example"
    backend_mode: str = "sim"  # sim | remote
    max_concurrency: int = 8
    timeout_ms: int = 5000

    class Config:
        env_prefix = ""
        env_file = ".env"
