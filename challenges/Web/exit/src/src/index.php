<?php
highlight_file(__FILE__);
$end = "<?php echo 'Party is over. Please exitttt'; exit;?>";
$code = $end.$_POST["code"];
@file_put_contents($_POST["filename"], $code);