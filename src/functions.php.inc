<?php
/**
 * Common functions.
 * =====================================================================
 * @author KEINOS
 * @version 20180417
*/

/* -------------------------------------------------------------- [D] */

function dir_exists($path_dir)
{
    return is_dir((string) $path_dir);
}


/* -------------------------------------------------------------- [I] */

function indent_string($string)
{
    $result = '';
    if (is_mode_cli()) {
        $lines = explode(PHP_EOL, $string);
        foreach ($lines as $line) {
            $line = trim($line);
            $result .= INDENT . $line . PHP_EOL;
        }
    } else {
        $result = "<div style='padding-left:4em'>${string}</div>";
    }

    return $result;
}

function is_mode_cli()
{
    $name_sapi = strtolower(php_sapi_name());

    return ( 'cli' === substr($name_sapi, 0, 3) );
}

/* -------------------------------------------------------------- [P] */

function print_pretty($contents, $title = '', $return = false)
{
    $title    = (string) trim($title);
    $contents = is_string($contents) ? $contents : print_r($contents, true);

    $has_title = (empty($title)) ? NO : YES;

    if (YES == $has_title) {
        if (is_mode_cli()) {
            $title = PHP_EOL . HR . $title . PHP_EOL . HR;
        } else {
            $title = htmlspecialchars($contents);
            $title = "<div style='font-weight:strong'>${title}</div>";
        }
        $contents = indent_string($contents);
    } else {
        $title    = '';
        $contents = $contents . PHP_EOL;
    }

    $result  = '';
    $result .= $title;
    $result .= $contents;

    if ($return) {
        return $result;
    } else {
        echo $result;
    }
}

/* -------------------------------------------------------------- [S] */

function set_env_utf8_ja($timezone = 'Asia/Tokyo')
{
    if (! function_exists('mb_language')) {
        die('This application requires mb_language.');
    }

    date_default_timezone_set($timezone);
    setlocale(LC_ALL, 'ja_JP');
    mb_language('ja');
    mb_internal_encoding('UTF-8');
    mb_http_output('UTF-8');
    ob_start("mb_output_handler");
}

/* -------------------------------------------------------------- [T] */

function touch_dir($path_dir)
{
    $mod = 0777;
    if (@mkdir($path_dir, $mod, true) && chmod($path_dir, $mod)) {
        return realpath($path_dir);
    } else {
        die("Error making dir at: ${path_dir_bin}");
    }
}

