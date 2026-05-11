<!doctype html>
<html>
<head>
    <meta charset="utf-8">
    <title>第4关</title>
    <link rel="stylesheet" href="css/default.css">
</head>
<body>
<div class="overlay">
    <h2>请以admin的身份访问此页面</h2>
    <?php
    session_start();
    $level = 4;
    $flag = @fgets(fopen("/flag", "r"));

    if (isset($_SESSION['level']) && !$_SESSION['level'] < $level) {
        if ($_COOKIE['admin'] === "1"){
            echo "<p>恭喜，你已经掌握基本的HTTP知识了</p>";
            echo "<p>这是你的奖励：".$flag."</p>";
        }
    }else{
        echo "<p>这是第四关，请先完成前面的关卡！</p>";
    }
    ?>
</div>
</body>
</html>
