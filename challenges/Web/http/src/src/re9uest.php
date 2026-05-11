<!doctype html>
<html>
<head>
    <meta charset="utf-8">
    <title>第1关</title>
    <link rel="stylesheet" href="css/default.css">
</head>
<body>
<div class="overlay">
    <h1>欢迎来到第1关，这里考察最基本的http请求</h1>
    <p>使用GET发送一个数组b1ts, 其中依次包含两个值, ctf和f1ag</p>
    <p>使用POST发送一个参数byte, 其值为123</p>
    <?php
    session_start();
    $level = 1;

    $param = $_GET["b1ts"];
    $post = $_POST["byte"];

    if (isset($param[0]) && $param[0] == "ctf") {
        if (isset($param[1]) && $param[1] == "f1ag") {
            if (isset($post) && $post == "123") {
                $_SESSION['level'] = 2;
                echo "<p>恭喜过关，请前往 /ta5k2.php</p>";
            }else{
                echo "<p>是不是忘了POST参数？</p>";
            }
        }else{
            echo "<p>只接收到了ctf</p>";
        }
    }
    ?>
</div>
</body>

</html>

