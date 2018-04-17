<?php
/**
 * Common functions.
 * =====================================================================
 * @author KEINOS
 * @version 20180417
*/

/* -------------------------------------------------------------- [D] */

function die_if_web()
{
    if (! is_cli()) {
        header('Content-Type: text/plain; charset=utf-8', true, 500);
        die('This script must be run via command line.');
    }
}

/* -------------------------------------------------------------- [D] */

function dir_exists($path_dir)
{
    return is_dir((string) $path_dir);
}

/* -------------------------------------------------------------- [E] */

function echo_eol($string)
{
    echo PHP_EOL, $string, PHP_EOL;
}

function echo_hr($string = '-', $return = false)
{
    if (! defined('HR')) {
        $width = get_screen_width();
        $hr    = str_repeat($string, $width);
        define('HR', $hr);
    }

    if ($return) {
        return HR . PHP_EOL;
    }

    echo HR . PHP_EOL;
}

/* -------------------------------------------------------------- [G] */

function get_screen_width()
{
    $default_width = 70; //デフォルト幅

    if (! defined('SCREEN_WIDTH')) {
        if (! is_cli()) {
            $msg = 'This script must be run via command line.';
            die($msg);
        }

        $width = trim(`tput cols`); //バッククォートであることに注意
        $width = is_numeric($width) ? $width : $default_width;
        define('SCREEN_WIDTH', $width);
    }

    return SCREEN_WIDTH;
}

/* -------------------------------------------------------------- [F] */

function format_title($title, $width_indent = 0)
{
    $title  = trim((string) $title);
    $result = '';
    $indent = '';

    if ($width_indent) {
        $width  = get_screen_width() - $width_indent;
        $hr     = str_repeat('-', $width) . PHP_EOL;
        $indent = str_repeat(' ', $width_indent);
    } else {
        $hr = echo_hr('-', DO_NOT_ECHO);
    }

    $result .= $indent . $hr;
    $result .= $indent . '  ' . $title . PHP_EOL;
    $result .= $indent . $hr;

    return $result;
}

/* -------------------------------------------------------------- [I] */

function indent_string($string)
{
    if (is_cli()) {
        $result = '';
        $lines  = explode(PHP_EOL, $string);
        foreach ($lines as $line) {
            $line    = trim($line);
            $result .= INDENT . $line . PHP_EOL;
        }
    } else {
        $result = "<div style='padding-left:4em'>${string}</div>";
    }

    return $result;
}

function is_cli()
{
    $name_sapi = strtolower(php_sapi_name());

    return ( 'cli' === substr($name_sapi, 0, 3) );
}

/* -------------------------------------------------------------- [P] */

function print_pretty($contents, $title = '', $return = false)
{
    $title    = (string) trim($title);

    if (! is_string($contents)) {
        $contents = print_r($contents, true);
    } else {
        $contents = (string) $contents;
    }

    $has_title = (empty($title)) ? NO : YES;

    if ($has_title === YES) {
        $title    = format_title($title) . PHP_EOL;
        $contents = indent_string($contents) . PHP_EOL;
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
