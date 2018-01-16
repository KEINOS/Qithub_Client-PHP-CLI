<?php

/* Validate phar settings */
if (ini_get('phar.readonly') || ini_get('phar.require_hash')) {
    error('Permission denied: Disable "phar.readonly" and "phar.require_hash"');
    die;
}


$phar = new Phar('../qithub.phar', 0);

$files[]="../src/index.php";
$files[]="../src/functions.php.inc";
$files[]="../src/constants.php.inc";

foreach($files as $file){
    if(file_exists($file)){
        $path_file = realpath($file);
        $phar->addFile($path_file);
    }
}
$phar->setStub('<?php
Phar::webPhar("qithub.phar", "index.php" );
__HALT_COMPILER(); ?>');

