<?php
/**
 * Document creator using PHPDoc (PHP Documentor).
 *
 * Run this script and you will get the API documents of `./src/` at
 * `./src/docs/`.
 *
 * It will:
 *   - Download `phpDocumentor.phar` at `./bin/` if ! exist.
 */

/*
========================================================================
    Constants
========================================================================
*/

define("HR", is_mode_cli() ? str_repeat('-', 72) . PHP_EOL : '<hr>' . \
    PHP_EOL);

const DIR_CURRENT     = '.';
const DIR_NAME_BIN    = "bin";
const DIR_NAME_DOC    = "docs";
const DIR_NAME_SRC    = "src";
const DIR_PARRENT     = '..';
const DIR_SEP         = DIRECTORY_SEPARATOR;
const INDENT          = "\t";
const NAME_PHPDOC_BIN = 'phpDocumentor.phar';
const NO              = false;
const PATH_DIR_BIN    = DIR_CURRENT . DIR_SEP . DIR_NAME_BIN . DIR_SEP;
const PATH_DIR_SRC    = DIR_PARRENT . DIR_SEP . DIR_NAME_SRC . DIR_SEP;
const URL_PHPDOC_BIN  = 'http://phpdoc.org/phpDocumentor.phar';
const YES             = true;

// Place `docs/` under `src/`
const PATH_DIR_DOC    = PATH_DIR_SRC . DIR_NAME_DOC . DIR_SEP;

/*
========================================================================
    Main
========================================================================
*/

/*
 * Setting up target files.
 * Add files here to be documented.
 * ---------------------------------------------------------------------
 */
$target_files = array();

$target_files[] = [
    'type'      => 'file',
    'name_file' => 'functions.php.inc',
    'path_dir'  => PATH_DIR_SRC,
];
$target_files[] = [
    'type'      => 'file',
    'name_file' => 'constants.php.inc',
    'path_dir'  => PATH_DIR_SRC,
];

/*
 * Check: PHPDoc existstance. Download if not found.
 * ---------------------------------------------------------------------
 */
$path_file_phpdoc = PATH_DIR_BIN . NAME_PHPDOC_BIN;

if (! file_exists($path_file_phpdoc)) {
    if (! dir_exists(PATH_DIR_BIN)) {
        if( touch_dir(PATH_DIR_BIN) ){
            touch( PATH_DIR_BIN . 'index.html' );
            
        }
    }

    echo "Downloading phpDocumentor.phar ...";
    $contents = file_get_contents(URL_PHPDOC_BIN);
    if (file_put_contents($path_file_phpdoc, $contents, LOCK_EX)) {
        echo "[done]." . PHP_EOL;
    }
}

$path_file_phpdoc = realpath($path_file_phpdoc);

/*
 * Checking destination directory. (Doc dir)
 * ---------------------------------------------------------------------
 */
if (! dir_exists(PATH_DIR_DOC)) {
    touch_dir(PATH_DIR_DOC);
}

$path_dir_destination = realpath(PATH_DIR_DOC);
$args_dir_destination = "-t '${path_dir_destination}'";

/*
 * Adding files do documentor
 * ---------------------------------------------------------------------
 */
$args_target_files = '';

foreach ($target_files as $file) {
    $path_file = $file['path_dir'] . $file['name_file'];
    
    switch($file['type']){
        case 'file':
            $option = '-f';
            break;
        case 'dir':
        case 'directory':
            $option = '-d'; 
            break;
        case 'ignore':
            $option = '-i';
            break;
    }

    if (file_exists($path_file)) {
        $path_file = realpath($path_file);
        $args_target_files .= "${option} '${path_file}' ";
    }
}

$args_target_files = trim($args_target_files);

/*
 * Create document
 * ---------------------------------------------------------------------
 */

$command = "php '${path_file_phpdoc}' ${args_target_files} ${args_dir_destination}";

/*
print_pretty($path_dir_destination, 'Create doc at');
print_pretty($args_dir_destination, 'Arg for destination');
print_pretty($target_files, 'List of target files');
print_pretty($args_target_files, 'Arg for target files');
print_pretty($path_file_phpdoc, 'Path to phpDocumentor.phar');
print_pretty($command, 'Command to execute');
*/

print_pretty($command, "Command to run:");
print_pretty(`$command`, "Running...");
print_pretty("DONE");

