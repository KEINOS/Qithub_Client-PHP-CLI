<?php

require_once('./utils/archive_src.php.inc');

if (is_phar_readonly()) {
    $msg  = 'Permission denied: ';
    $msg .= 'Disable "phar.readonly" and "phar.require_hash"';
    error($msg);
    die;
}

/*
 *  Settings
 * ---------------------------------------------------------------------
 */
$name_file_output = 'qithub.phar';
$name_file_main   = 'index.php';

$include_files = [
    $name_file_main,
    'functions.php.inc',
    'constants.php.inc',
];

/*
 *  Setup
 * ---------------------------------------------------------------------
 */

$include_files = set_files_under('src', $include_files);

$box_options = [
    "alias"  => $name_file_output,
    "chmod"  => "0755",
    "files"  => $include_files,
    "main"   => 'src/index.php',
    "output" => $name_file_output,
    "stub"   => true
];

/*
 *  Build Phar
 * ---------------------------------------------------------------------
 */

$box_json = json_encode($box_options, JSON_PRETTY_PRINT);

if (! file_put_contents(PATH_FILE_OPTION, $box_json)) {
    die('Failed to save json file');
}

if (file_exists(PATH_FILE_BOX) && file_exists(PATH_FILE_OPTION)) {
    $path_file_box  = PATH_FILE_BOX;
    $path_file_json = PATH_FILE_OPTION;
    $cmd = "php '${path_file_box}' build -c '${path_file_json}'";
    //echo $cmd . PHP_EOL;
    //echo '----------------------------' . PHP_EOL;
    echo `$cmd`;
} else {
    die("File not found.");
}
