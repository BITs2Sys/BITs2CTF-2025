FROM python:3.11-slim

WORKDIR /app

# 安装 unzip 方便解压校徽
RUN apt-get update && apt-get install -y --no-install-recommends unzip && rm -rf /var/lib/apt/lists/*

# 拷贝源码和素材
COPY app.py /app/app.py
COPY requirements.txt /app/requirements.txt
COPY 北京理工大学校徽.zip /app/北京理工大学校徽.zip

# 安装依赖
RUN pip install --no-cache-dir -r /app/requirements.txt

# 解压校徽并统一命名
RUN mkdir -p /app/assets && \
    cd /app && \
    unzip -o 北京理工大学校徽.zip -d /app/assets && \
    python - <<'PY'
import os, glob, shutil
assets = "/app/assets"
pngs = [p for p in glob.glob(os.path.join(assets, "**", "*.png"), recursive=True)]
if not pngs:
    raise SystemExit("no png found in 北京理工大学校徽.zip")
# 取第一个 png，当成校徽
target = os.path.join(assets, "bit_logo.png")
shutil.copyfile(pngs[0], target)
PY

# Flask 监听 0.0.0.0:5000
ENV FLASK_APP=app.py
ENV FLASK_RUN_HOST=0.0.0.0
ENV FLASK_RUN_PORT=5000

EXPOSE 5000

# 运行
CMD ["python", "/app/app.py"]

