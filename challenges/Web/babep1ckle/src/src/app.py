#!/usr/bin/env python3
# coding: utf-8
from flask import (
    Flask, request, session, redirect, url_for,
    render_template, render_template_string, abort, flash
)
import os, base64, pickle, jwt
from werkzeug.security import generate_password_hash, check_password_hash

class TestPayload:
    def __init__(self, message="TEST_OK"):
        self.message = message

    def __reduce__(self):
        return (str, (self.message,))

app = Flask(__name__)
app.secret_key = os.environ.get("CHAL_SECRET", "BITs2CTF2025")

USERS = {}

def verify_token_silent(token):
    try:
        payload = jwt.decode(token, app.secret_key, algorithms=["HS256"])
        return payload
    except Exception:
        return None

@app.route("/", methods=["GET"])
def root():
    return redirect(url_for("login"))

@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":
        username = (request.form.get("username") or "").strip()
        password = request.form.get("password") or ""
        confirm = request.form.get("confirm") or ""
        blacklist = ["session" ,"globals", "read", "lipsum", "request", "os", "popen", "import", "eval", "tac", "cat", "flag"]
        if not username:
            flash("用户名不能为空", "danger")
            return render_template("register.html")
        if len(username) >= 20:
            flash("用户名长度不能超过20个字符", "danger")
            return render_template("register.html")
        if any([n in username.lower() for n in blacklist]):
            flash("非法的输入", "danger")
            return render_template("register.html")
        if not password:
            flash("密码不能为空", "danger")
            return render_template("register.html")
        if password != confirm:
            flash("两次输入的密码不一致", "danger")
            return render_template("register.html")
        if username in USERS:
            flash("用户名已存在", "danger")
            return render_template("register.html")

        USERS[username] = generate_password_hash(password)
        flash("注册成功！请登录", "success")
        return redirect(url_for("login"))
    return render_template("register.html")

@app.route("/login", methods=["GET", "POST"])
def login():
    if request.method == "POST":
        username = (request.form.get("username") or "").strip()
        password = request.form.get("password") or ""

        if not username or not password:
            flash("请输如完整的用户名或密码", "danger")
            return render_template("login.html")
        if username not in USERS:
            flash("用户名不存在", "danger")
            return render_template("login.html")
        if not check_password_hash(USERS[username], password):
            flash("用户名或密码错误", "danger")
            return render_template("login.html")

        session.clear()
        session["username"] = username
        session["role"] = "guest"

        auth = request.headers.get("Authorization", "")
        token = None
        if auth.startswith("Bearer "):
            token = auth.split(" ", 1)[1].strip()
        else:
            token = request.form.get("token")

        if token:
            payload = verify_token_silent(token)
            if payload and payload.get("user") == "admin":
                session["role"] = "admin"
              
        return redirect(url_for("panel"))
    return render_template("login.html")

@app.route("/panel", methods=["GET"])
def panel():
    if "username" not in session:
        return redirect(url_for("login"))

    username = session.get("username", "guest")
    role = session.get("role", "guest")
    panel_content = """
    {% extends "base.html" %}

    {% block content %}
    """ + f"""
    <div class="card">
        <div class="card-header text-center">
            <h3>欢迎, { username }</h3>
        </div>"""+"""
        <div class="card-body">
            <div class="alert alert-info">
                <strong>角色:</strong> {{ role.capitalize() }}
            </div>
            
            <h4 class="mb-3">文档面板</h4>
            <p>欢迎使用文档面板。以下是一些可供浏览的资源。</p>
            
            <div class="mb-4">
                <h5>文档</h5>
                <ul>
                    <li>Flask config中存储的内容</li>
                    <li>JWT介绍</li>
                    <li>管理员笔记（受限）</li>
                </ul>
            </div>
            
            {% if role == 'admin' %}
            <hr>
            <div class="admin-section">
                <h4 class="mb-3">管理员工具</h4>
                <p>上传Base64编码的Pickle数据（仅管理员）</p>
                
                <form method="post" action="/admin/unpickle">
                    <div class="mb-3">
                        <textarea class="form-control" name="data" rows="5" 
                                  placeholder="输入Base64编码的Pickle数据"></textarea>
                    </div>
                    <button type="submit" class="btn btn-primary w-100">执行</button>
                </form>
            </div>
            {% endif %}
            
            <div class="mt-4 text-center">
                <a href="{{ url_for('login') }}" class="btn btn-outline-secondary">退出登录</a>
            </div>
        </div>
    </div>
    {% endblock %}
    """
    
    return render_template_string(panel_content, username=username, role=role)


@app.route("/admin/unpickle", methods=["POST"])
def admin_unpickle():
    dt = request.form.get("data", "")
    if not dt:
        return "请输入base64编码的pickle数据", 400
    
    try:
        raw = base64.b64decode(dt)
        obj = pickle.loads(raw)
        return str(obj)  
    except Exception as e:
        return f"反序列化失败: {type(e).__name__}: {str(e)}", 400
    
if __name__ == "__main__":
    app.run(debug=False)
