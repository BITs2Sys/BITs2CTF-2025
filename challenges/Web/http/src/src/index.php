<?php
session_start();
?>
<!doctype html>
<html>
<head>
    <meta charset="utf-8">
    <title>第0关</title>
    <link rel="stylesheet" href="css/default.css">
</head>
<body>
<div class="overlay">
    <h1>欢迎来到BITs2CTF 2025</h1>
    <h1>答案就在网页源代码里</h1>
    <!--恭喜你学会查看网页源代码-->
    <!--前往/re9uest.php进入下一关-->
    <br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>
    <br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>
    <br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>
    <br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>
    <br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>
    <p>是不是发现自己打不开源代码界面了？</p>
    <p>hint：直接在URL栏里动手脚</p>
</div>



<script>
    (function(){
        // 拦截常见打开开发者工具/查看源码的组合键
        document.addEventListener('keydown', function(e) {
            // F12
            if (e.key === 'F12') {
                e.preventDefault();  return false;
            }
            // Ctrl+U 或 Cmd+U
            if ((e.ctrlKey || e.metaKey) && e.key.toLowerCase() === 'u') {
                e.preventDefault();  return false;
            }
            // Ctrl+Shift+I / Ctrl+Shift+J / Ctrl+Shift+C
            if ((e.ctrlKey || e.metaKey) && e.shiftKey && ['I','J','C'].includes(e.key.toUpperCase())) {
                e.preventDefault();  return false;
            }
            // Ctrl+S 保存页面
            if ((e.ctrlKey || e.metaKey) && e.key.toLowerCase() === 's') {
                e.preventDefault();  return false;
            }
        }, false);

        // 禁用右键菜单（同样是弱防护）
        document.addEventListener('contextmenu', function(e) {
            e.preventDefault();
            return false;
        }, false);

        // 小 trick：若检测到 devtools 打开（不可靠），提示用户
        var devtoolsOpen = false;
        setInterval(function(){
            // 简单检测：使用时间消耗/断点检测/尺寸差等方法都不可靠；这里演示一个常见检测
            var widthThreshold = window.outerWidth - window.innerWidth > 160;
            var heightThreshold = window.outerHeight - window.innerHeight > 160;
            if (widthThreshold || heightThreshold) {
                if (!devtoolsOpen) {
                    devtoolsOpen = true;
                }
            } else {
                devtoolsOpen = false;
            }
        }, 1000);
    })();
</script>
</body>
</html>
