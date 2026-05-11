<?php
show_source("h3r3.php");

class bits{
    private $cmd = "pwd";
    private $username = "idk666";
    private $password = "114514";

    public function __invoke(){
        if($this->username == "1diot9" && $this->password == "BITs2CTF"){
            if (!preg_match("/ |cat|less|more|head|tail|flag/", $this->cmd)) {
                echo "\n";
                system($this->cmd);
            }else{
                die("不要输入奇怪的命令！");
            }
        }else{
            echo "就差一点了";
        }
    }
}

class CTF{
    public $func = "unser";

    public function __set($a, $b){
        $method = $this->func;
        $method();
    }

    public function unser(){
        echo "你快学会php反序列化了";
    }
}

class anything{
    public $var1 = "aaa";
    public $var2;

    public function __toString(){
        $this->var2->sec = $this->var1;
        return "anything";
    }
}

class endpoint{
    public $bits2ctf = "welcome to bits2ctf!";
    public function __destruct(){
        echo $this->bits2ctf;
    }
}

$data = $_POST["data"];
if (isset($data)){
    unserialize($data);
}