const express = require('express');
const vm = require('vm');

const app = express();
const port = 3000;

app.use(express.json());
app.use(express.static('public'));

app.get('/', (req, res) => {
  res.send(`
    <!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>在线Node JS Shell</title>
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
    <style>
        :root {
            --primary: #4361ee;
            --primary-dark: #3a56d4;
            --secondary: #7209b7;
            --success: #4cc9f0;
            --light: #f8f9fa;
            --dark: #212529;
            --gray: #6c757d;
            --border-radius: 10px;
            --box-shadow: 0 10px 30px rgba(0, 0, 0, 0.1);
            --transition: all 0.3s ease;
        }

        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
        }

        body {
            background: linear-gradient(135deg, #1a2a6c, #b21f1f, #fdbb2d);
            background-size: 400% 400%;
            animation: gradient 15s ease infinite;
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
            padding: 20px;
            color: var(--dark);
        }

        @keyframes gradient {
            0% { background-position: 0% 50%; }
            50% { background-position: 100% 50%; }
            100% { background-position: 0% 50%; }
        }

        .container {
            max-width: 800px;
            width: 100%;
            background: rgba(255, 255, 255, 0.95);
            backdrop-filter: blur(10px);
            border-radius: var(--border-radius);
            box-shadow: var(--box-shadow);
            overflow: hidden;
            transition: var(--transition);
        }

        .container:hover {
            transform: translateY(-5px);
            box-shadow: 0 15px 40px rgba(0, 0, 0, 0.15);
        }

        .header {
            background: linear-gradient(to right, var(--primary), var(--secondary));
            color: white;
            padding: 25px 30px;
            text-align: center;
        }

        .header h1 {
            font-size: 2.2rem;
            margin-bottom: 10px;
            display: flex;
            align-items: center;
            justify-content: center;
            gap: 15px;
        }

        .header p {
            font-size: 1.1rem;
            opacity: 0.9;
        }

        .content {
            padding: 30px;
        }

        .info-card {
            background: linear-gradient(to right, #e0f7fa, #bbdefb);
            border-left: 5px solid var(--primary);
            border-radius: 8px;
            padding: 20px;
            margin-bottom: 25px;
            display: flex;
            align-items: center;
            gap: 15px;
        }

        .info-icon {
            font-size: 2rem;
            color: var(--primary);
        }

        .code-section {
            margin-bottom: 25px;
        }

        .code-section label {
            display: block;
            margin-bottom: 10px;
            font-weight: 600;
            color: var(--dark);
            font-size: 1.1rem;
        }

        #code {
            width: 100%;
            height: 150px;
            padding: 15px;
            border: 2px solid #e0e0e0;
            border-radius: var(--border-radius);
            font-family: 'Courier New', monospace;
            font-size: 1rem;
            resize: vertical;
            transition: var(--transition);
            background-color: #f8f9fa;
        }

        #code:focus {
            outline: none;
            border-color: var(--primary);
            box-shadow: 0 0 0 3px rgba(67, 97, 238, 0.2);
        }

        .button-container {
            display: flex;
            justify-content: center;
            margin: 20px 0;
        }

        #execute-btn {
            background: linear-gradient(to right, var(--primary), var(--secondary));
            color: white;
            border: none;
            border-radius: 50px;
            padding: 14px 35px;
            font-size: 1.1rem;
            font-weight: 600;
            cursor: pointer;
            transition: var(--transition);
            display: flex;
            align-items: center;
            gap: 10px;
            box-shadow: 0 5px 15px rgba(67, 97, 238, 0.4);
        }

        #execute-btn:hover {
            transform: translateY(-3px);
            box-shadow: 0 8px 20px rgba(67, 97, 238, 0.5);
        }

        #execute-btn:active {
            transform: translateY(1px);
        }

        .result-section {
            margin-top: 25px;
        }

        .result-section h3 {
            margin-bottom: 15px;
            color: var(--dark);
            display: flex;
            align-items: center;
            gap: 10px;
        }

        #result {
            background: white;
            border: 2px solid #e0e0e0;
            border-radius: var(--border-radius);
            padding: 20px;
            min-height: 80px;
            font-family: 'Courier New', monospace;
            white-space: pre-wrap;
            word-break: break-all;
            transition: var(--transition);
        }

        .result-success {
            border-color: #28a745;
            background-color: rgba(40, 167, 69, 0.05);
        }

        .result-error {
            border-color: #dc3545;
            background-color: rgba(220, 53, 69, 0.05);
        }

        .example-code {
            margin-top: 15px;
            font-size: 0.9rem;
            color: var(--gray);
            display: flex;
            flex-wrap: wrap;
            gap: 10px;
        }

        .code-snippet {
            background: #e9ecef;
            padding: 5px 10px;
            border-radius: 4px;
            cursor: pointer;
            transition: var(--transition);
            font-family: 'Courier New', monospace;
        }

        .code-snippet:hover {
            background: #dee2e6;
        }

        .footer {
            text-align: center;
            padding: 20px;
            border-top: 1px solid #e0e0e0;
            color: var(--gray);
            font-size: 0.9rem;
        }

        .loading {
            display: none;
            text-align: center;
            margin: 10px 0;
        }

        .spinner {
            border: 4px solid rgba(0, 0, 0, 0.1);
            border-left-color: var(--primary);
            border-radius: 50%;
            width: 30px;
            height: 30px;
            animation: spin 1s linear infinite;
            margin: 0 auto;
        }

        @keyframes spin {
            to { transform: rotate(360deg); }
        }

        @media (max-width: 768px) {
            .container {
                margin: 10px;
            }
            
            .header h1 {
                font-size: 1.8rem;
            }
            
            .content {
                padding: 20px;
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1><i class="fas fa-code"></i> 在线Node JS Shell</h1>
            <p>安全的学习环境，在VM沙箱中运行您的代码</p>
        </div>
        
        <div class="content">
            <div class="info-card">
                <i class="fas fa-info-circle info-icon"></i>
                <div>
                    <p>这是一个安全的Node JS Shell。你可以在这里进行Node JS学习。</p>
                    <p>请放心，你的所有操作都在vm沙箱中运行，不会产生其他影响。</p>
                </div>
            </div>
            
            <div class="code-section">
                <label for="code"><i class="fas fa-terminal"></i> 输入JavaScript代码:</label>
                <textarea id="code" placeholder="输入你的JavaScript代码...">1 + 1 * 2</textarea>
                
                <div class="example-code">
                    <span>试试这些例子:</span>
                    <span class="code-snippet" onclick="insertCode('1 + 1 * 2')">1 + 1 * 2</span>
                    <span class="code-snippet" onclick="insertCode('Math.sqrt(16)')">Math.sqrt(16)</span>
                    <span class="code-snippet" onclick="insertCode('new Date().toLocaleString()')">new Date()</span>
                </div>
            </div>
            
            <div class="button-container">
                <button id="execute-btn" onclick="executeCode()">
                    <i class="fas fa-play-circle"></i> 执行代码
                </button>
            </div>
            
            <div class="loading" id="loading">
                <div class="spinner"></div>
                <p>代码执行中...</p>
            </div>
            
            <div class="result-section">
                <h3><i class="fas fa-poll"></i> 执行结果:</h3>
                <div id="result">等待代码执行...</div>
            </div>
        </div>
        
        <div class="footer">
            <p>Copyleft 2025 在线Node JS Shell | lamaper</p>
        </div>
    </div>

    <script>
        function insertCode(code) {
            document.getElementById('code').value = code;
        }
        
        function executeCode() {
            const code = document.getElementById('code').value;
            const resultDiv = document.getElementById('result');
            const loadingDiv = document.getElementById('loading');
            const executeBtn = document.getElementById('execute-btn');
            
            // 显示加载动画
            loadingDiv.style.display = 'block';
            executeBtn.disabled = true;
            executeBtn.innerHTML = '<i class="fas fa-spinner fa-spin"></i> 执行中...';
            
            // 清除之前的结果样式
            resultDiv.className = '';
            
            fetch('/eval', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ code: code })
            })
            .then(response => response.json())
            .then(data => {
                // 隐藏加载动画
                loadingDiv.style.display = 'none';
                executeBtn.disabled = false;
                executeBtn.innerHTML = '<i class="fas fa-play-circle"></i> 执行代码';
                
                if (data.result) {
                    resultDiv.innerHTML = data.result;
                    resultDiv.classList.add('result-success');
                } else if (data.error) {
                    resultDiv.innerHTML = data.error;
                    resultDiv.classList.add('result-error');
                }
            })
            .catch(err => {
                // 隐藏加载动画
                loadingDiv.style.display = 'none';
                executeBtn.disabled = false;
                executeBtn.innerHTML = '<i class="fas fa-play-circle"></i> 执行代码';
                
                resultDiv.innerHTML = '网络错误: ' + err;
                resultDiv.classList.add('result-error');
            });
        }
        
        // 按Ctrl+Enter执行代码
        document.getElementById('code').addEventListener('keydown', function(e) {
            if (e.ctrlKey && e.key === 'Enter') {
                executeCode();
            }
        });
    </script>
</body>
</html>
    `);
});

app.post('/eval', (req, res) => {
  const userCode = req.body.code;
  console.log('Received code:', userCode);

  if (!userCode || typeof userCode !== 'string') {
    return res.json({ error: '请输入有效的代码' });
  }

  if (userCode.length > 1000) {
    return res.json({ error: '代码长度不能超过1000字符' });
  }

  const logs = [];
  const sandbox = {
    console: {
      log: (...args) => {
        try { logs.push(args.map(a => {
          if (typeof a === 'string') return a;
          try { return JSON.stringify(a); } catch(e){ return String(a); }
        }).join(' ')); } catch(e){}
      }
    }
  };

  const context = vm.createContext(sandbox);

  try {
    const result = vm.runInContext(userCode, context, { timeout: 1000 });

    let resultStr;
    try { resultStr = typeof result === 'undefined' ? undefined : String(result); }
    catch(e) { resultStr = '[unserializable result]'; }

    return res.json({ result: resultStr, logs });
  } catch (error) {
    let errorMessage;
    try { errorMessage = error && error.toString ? error.toString() : String(error); } catch(e){ errorMessage = 'Unknown error'; }
    return res.json({ error: errorMessage, logs });
  }
});

app.listen(port, () => {
  console.log(`题目运行在 http://localhost:${port}`);
});
