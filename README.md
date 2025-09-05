# QDB: Hybrid Quantum-Classical Database Execution Framework

本项目实现了一个基于 **PostgreSQL 扩展** 的量子/经典混合执行框架（QDB）。  
它通过 **Cost Model** 与 **External Executor (QExe)**，在查询优化阶段选择合适的执行路径，支持在 **TPC-H 基准测试**中验证加速效果。

---

## 📂 项目结构

.
├── src/ # 核心源码
│ ├── pg_qdb # PostgreSQL 扩展
│ ├── cost_model # 代价模型模块
│ └── qexe # 外部执行引擎服务
├── configs/ # 配置文件
├── tpch/ # TPC-H 数据与查询
├── runner/ # 测试与评估工具
├── docker/ # 各组件 Dockerfile
├── docker-compose.yml # 服务编排
├── docs/ # 项目文档
└── telemetry/ # 指标与日志




---

## 🚀 快速开始

### 1. 克隆与配置
```bash
git clone <repo_url>
cd qdb
cp .env.example .env

### 2.构建与启动
docker compose build
docker compose up -d pg qexe cost

检查服务是否运行：
docker compose ps

### 3. 初始化数据库
# 导入 schema 与索引
docker compose exec -T pg psql -U postgres -d tpch -f /tpch/schema.sql
docker compose exec -T pg psql -U postgres -d tpch -f /tpch/indexes.sql

# 装载小规模数据
docker compose exec tpch bash /tpch/load.sh 0.01


### 4.运行示例查询
进入 psql：
docker compose exec pg psql -U postgres -d tpch

执行：
/*+ QNODE(table=lineitem op=scan) */
EXPLAIN SELECT l_returnflag, COUNT(*) FROM lineitem GROUP BY 1;
预期：执行计划中出现 Custom Scan (QNode)。

### 5.运行 TPC-H 测试
docker compose run --rm runner python runner/run_tpch.py \
  --queries 1 3 --scale 0.01 --out runner/artifacts
分析结果：
python runner/metrics.py runner/artifacts
python runner/speedup_plot.py runner/artifacts


📊 工作流

用户查询 → PostgreSQL

pg_qdb 扩展：Planner Hook 阶段调用代价模型，判断是否注入 QNode

QNode → QExe：封装算子请求，调用外部执行服务

QExe：执行 demo 算法或量子 API，返回结果

PostgreSQL：继续执行剩余计划并返回最终结果

Runner + TPC-H：批量运行查询，收集并分析指标


🛠 开发

编译扩展：使用 docker/Dockerfile.pg 自动构建（包含 PostgreSQL 开发头文件）。

调试日志：查看 docker compose logs -f pg / qexe / cost。

结果分析：所有实验结果保存在 runner/artifacts/ 中。

📌 状态

 PostgreSQL 扩展注入 QNode

 成本模型框架

 QExe 服务与 OpenAPI 定义

 TPC-H 数据与测试工具链

 更多算子类型支持

 与真实量子 API 的集成