<!doctype html>
<html>
<head>
    <meta charset="utf-8">
    <title>第2关</title>
    <link rel="stylesheet" href="css/default.css">
</head>
<body>
<div class="overlay">
    <p>本关考验你的信息收集能力</p>
    <p>为了限制搜索引擎的爬取范围，你知道网址通常使用哪个文件作为君子协定吗？</p>
    <?php
    session_start();
    $level = 2;

    if (isset($_SESSION['level']) && !$_SESSION['level'] < $level) {
        $_SESSION['level'] = 3;
    }else{
        echo "<p>这是第二关，请先完成上一关！</p>";
    }
    ?>
</div>
</body>
</html>

