import os
import io
import uuid
import base64
import json
import re
import glob
import requests
from flask import Flask, request, render_template_string, redirect, url_for, flash
from werkzeug.utils import secure_filename
from PIL import Image

# ======================================
# 基本配置
# ======================================
app = Flask(__name__)
app.secret_key = "change-me-in-production"
app.config["MAX_CONTENT_LENGTH"] = 2 * 1024 * 1024  # 2MB
STORAGE_DIR = os.path.join(os.path.dirname(__file__), "storage")
os.makedirs(STORAGE_DIR, exist_ok=True)

ALLOWED_EXTS = {".jpg", ".jpeg", ".png"}
ALLOWED_FORMATS = {"JPEG", "PNG"}
TITLE = "请上传一张北京理工大学的美景照片吧，记得要带校徽哟"

# 火山方舟 / 豆包 API 配置
ARK_API_URL = "https://ark.cn-beijing.volces.com/api/v3/chat/completions"
ARK_MODEL_ID = "doubao-seed-1-6-251015"  # 你指定的模型
ARK_API_KEY = os.getenv("ARK_API_KEY", "c2b090d4-0d6c-48af-bb0a-07bd001f59cc")  # 请在容器运行时传入

# ======================================
# HTML 模板
# ======================================
TEMPLATE = """
<!doctype html>
<html lang="zh-CN">
<head>
  <meta charset="utf-8">
  <title>{{ title }}</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body{font-family: system-ui, -apple-system, Segoe UI, Roboto, Helvetica, Arial, "Noto Sans", "PingFang SC", "Microsoft Yahei", sans-serif;
         background:#0b1220;color:#e6edf3;display:flex;min-height:100vh;align-items:center;justify-content:center;margin:0}
    .card{width:min(720px, 92vw); background:#111827; border:1px solid #243244; border-radius:16px; padding:24px 28px; box-shadow:0 10px 30px rgba(0,0,0,.45)}
    h1{margin:0 0 6px; font-size:28px}
    p.sub{margin:0 0 18px; color:#9fb1c1}
    form{display:grid; gap:14px; margin-top:8px}
    input[type=file]{padding:12px; background:#0f172a; border:1px dashed #334155; border-radius:10px; color:#9fb1c1}
    button{all:unset; padding:10px 16px; background:#2563eb; color:white; border-radius:10px; text-align:center; cursor:pointer}
    .msg{margin-top:10px; padding:10px 12px; background:#072; border-radius:8px}
    .err{margin-top:10px; padding:10px 12px; background:#502; border-radius:8px}
    code{background:#0b1220; padding:2px 6px; border-radius:6px}
    .flag{margin-top:10px; padding:10px 12px; background:#0369a1; border-radius:8px; font-weight:bold;}
  </style>
</head>
<body>
  <div class="card">
    <h1>{{ title }}</h1>

    {% with messages = get_flashed_messages(with_categories=true) %}
      {% if messages %}
        {% for cat, m in messages %}
          <div class="{{ 'err' if cat=='error' else 'msg' }}">{{ m|safe }}</div>
        {% endfor %}
      {% endif %}
    {% endwith %}

    {% if flag %}
      <div class="flag">🎉 恭喜你！flag: <code>{{ flag }}</code></div>
    {% endif %}

    <form action="{{ url_for('upload') }}" method="post" enctype="multipart/form-data">
      <input type="file" name="image" accept=".jpg,.jpeg,.png,image/jpeg,image/png" required>
      <button type="submit">上传</button>
    </form>

  </div>
</body>
</html>
"""

# ======================================
# 工具函数
# ======================================

def fail(msg: str):
    flash(msg, "error")
    return redirect(url_for("index"))


def load_logo_b64() -> str:
    """
    从 ./assets 目录里找一张 png，当作“北京理工大学校徽”，并转成 base64
    Dockerfile 会保证这里有 assets/bit_logo.png
    """
    assets_dir = os.path.join(os.path.dirname(__file__), "assets")
    cand = []
    if os.path.isdir(assets_dir):
        cand = glob.glob(os.path.join(assets_dir, "*.png"))
    if not cand:
        raise RuntimeError("未找到校徽 PNG，请确认 Docker 构建时已解压到 ./assets/")
    logo_path = cand[0]
    with open(logo_path, "rb") as f:
        return base64.b64encode(f.read()).decode("ascii")


# 在应用启动时就把校徽读好
try:
    LOGO_B64 = load_logo_b64()
except Exception as e:
    # 启动时找不到也不要直接崩，用空字符串，但上传时会提示
    LOGO_B64 = ""


def parse_doubao_json(text: str):
    """
    豆包可能会多说几句，我们尽量把里面的 {...} 拿出来，再把 True/False 变成 true/false 去 json 解析
    """
    if not text:
        return None
    # 尝试直接 json
    try:
        return json.loads(text)
    except Exception:
        pass

    # 找到第一个 { ... }
    m = re.search(r"\{.*\}", text, re.S)
    if not m:
        return None
    chunk = m.group(0)
    # 粗暴把 True/False 换成小写
    chunk = chunk.replace("True", "true").replace("False", "false")
    # gift 可能是中文 100 字，不会受影响
    try:
        return json.loads(chunk)
    except Exception:
        return None


def call_doubao(user_image_b64: str):
    """
    请求火山方舟 chat.completions，把“校徽图 + 用户图 + prompt”一起发过去
    文档参考：chat.completions 标准接口、图片用 data:image/png;base64 形式传入内容列表中
    """
    if not ARK_API_KEY:
        return None, "后端未配置 ARK_API_KEY"

    if not LOGO_B64:
        return None, "后端未找到校徽 PNG"

    prompt_text = (
        '北京理工大学校徽如第一张图所示，请你判断第二张图片是否在真实世界中拍摄并且包含北京理工大学校徽。'
        '如果是在真实世界中拍摄并且包含北京理工大学校徽，请返回一个json数据{"isReal":True,"hasLogo":True,"gift":<此处请编写100字的夸奖>}，'
        '如果任何一项条件不符合，则gift字段值为空'
    )

    headers = {
        "Content-Type": "application/json",
        "Authorization": f"Bearer {ARK_API_KEY}",
    }

    payload = {
        "model": ARK_MODEL_ID,
        "messages": [
            {
                "role": "user",
                "content": [
                    {"type": "text", "text": prompt_text},
                    {
                        "type": "image_url",
                        "image_url": {
                            "url": f"data:image/png;base64,{LOGO_B64}"
                        },
                    },
                    {
                        "type": "image_url",
                        "image_url": {
                            # 用户上传的图片类型不一定是 png，但大多数模型对这个字段宽松
                            "url": f"data:image/png;base64,{user_image_b64}"
                        },
                    },
                ],
            }
        ],
        # 可以显式关掉流式
        "stream": False,
    }

    try:
        resp = requests.post(ARK_API_URL, headers=headers, json=payload, timeout=20)
    except Exception as e:
        return None, f"请求豆包失败: {e}"

    if resp.status_code != 200:
        return None, f"豆包返回非 200: {resp.status_code}, {resp.text}"

    data = resp.json()
    # 标准 ark 返回格式是 choices[0].message.content
    try:
        content = data["choices"][0]["message"]["content"]
    except Exception:
        return None, "豆包返回格式异常"

    # 方舟一般就是纯字符串
    if isinstance(content, str):
        parsed = parse_doubao_json(content)
    else:
        # 偶尔也可能是 list
        joined = "".join([c.get("text", "") for c in content if isinstance(c, dict)])
        parsed = parse_doubao_json(joined)

    if not parsed:
        return None, "豆包未返回可解析的 JSON"

    return parsed, None

# ======================================
# 路由
# ======================================

@app.route("/", methods=["GET"])
def index():
    return render_template_string(TEMPLATE, title=TITLE, flag=None)


@app.errorhandler(413)
def too_large(_e):
    flash("文件大小需 < 2MB。", "error")
    return redirect(url_for("index"))


@app.post("/upload")
def upload():
    if "image" not in request.files:
        return fail("没有选择文件。")

    file = request.files["image"]
    if file.filename == "":
        return fail("没有选择文件。")

    ext = os.path.splitext(file.filename)[1].lower()
    if ext not in ALLOWED_EXTS:
        return fail("仅允许上传 JPEG/PNG。")

    file_bytes = file.read()
    if not file_bytes:
        return fail("空文件。")
    if len(file_bytes) >= app.config["MAX_CONTENT_LENGTH"]:
        return fail("文件大小需 < 2MB。")

    # 检查图片合法性
    try:
        Image.open(io.BytesIO(file_bytes)).verify()
        img = Image.open(io.BytesIO(file_bytes))
        fmt = (img.format or "").upper()
        w, h = img.size
    except Exception:
        return fail("不是有效图片或图片已损坏。")

    if fmt not in ALLOWED_FORMATS:
        return fail("仅允许 JPEG 或 PNG 图片。")
    if not (w > 14 and h > 14):
        return fail(f"尺寸太小：当前 {w}×{h}，要求宽>14 且高>14。")
    if (w * h) >= 36_000_000:
        return fail(f"总像素过大：当前 {w*h:,}，要求 < 36,000,000。")

    # 保存一份 base64 以便调试
    b64_user = base64.b64encode(file_bytes).decode("ascii")
    uid = uuid.uuid4().hex
    out_path = os.path.join(STORAGE_DIR, f"{uid}.b64")
    with open(out_path, "w", encoding="ascii") as f:
        f.write(b64_user)

    # 调用豆包判断
    result, err = call_doubao(b64_user)
    if err:
        flash(f"上传成功，但调用豆包失败：{err}", "error")
        return render_template_string(TEMPLATE, title=TITLE, flag=None)

    is_real = bool(result.get("isReal"))
    has_logo = bool(result.get("hasLogo"))
    if is_real and has_logo:
        flag = os.getenv("GZCTF_FLAG", "flag{test_flag}")
        flash("豆包判定通过：真实世界拍摄且包含北京理工大学校徽 ✅", "ok")
        return render_template_string(TEMPLATE, title=TITLE, flag=flag)
    else:
        flash("豆包判定未通过：请确保是真实世界拍摄并且包含北京理工大学校徽。", "error")
        return render_template_string(TEMPLATE, title=TITLE, flag=None)


if __name__ == "__main__":
    # CTF 环境一般前面会用 gunicorn/nginx，这里留调试入口
    app.run(host="0.0.0.0", port=5000)

