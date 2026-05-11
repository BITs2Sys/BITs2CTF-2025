<!doctype html>
<html>
<head>
    <meta charset="utf-8">
    <title>第3关</title>
    <link rel="stylesheet" href="css/default.css">
</head>
<body>
<div class="overlay">
    <h2>本关只允许从本地进行访问！</h2>
    <?php
    session_start();
    $level = 3;
    $headers = getallheaders();

    if (isset($_SESSION['level']) && !$_SESSION['level'] < $level) {
        if (isset($headers["X-Forwarded-For"])){
            if ($headers["X-Forwarded-For"] === "127.0.0.1" || $headers["X-Forwarded-For"] === "localhost") {
                $_SESSION['level'] = 4;
                setcookie("admin", "0", time() + 3600);
                echo "访问 /f0ur.php 进入下一关";
            }else{
                echo "<p>再说一次，本关只允许从本地访问！</p>";
            }
        }
    }else{
        echo "<p>这是第三关，请先完成前面的关卡！</p>";
    }
    ?>
</div>
</body>
</html>
