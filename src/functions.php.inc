<?php
/**
 * Common functions.
 * =====================================================================
 * @author KEINOS
 * @version 20180417
*/

/* -------------------------------------------------------------- [_] */

function _flush()
{
    ob_flush();
    flush();
}

/* -------------------------------------------------------------- [C] */

function cls()
{
    echo `clear`;
    _flush();
}

/* -------------------------------------------------------------- [D] */

function die_if_web()
{
    if (! is_cli()) {
        header('Content-Type: text/plain; charset=utf-8', true, 500);
        die('This script must be run via command line.');
    }
}


function dir_exists($path_dir)
{
    return is_dir((string) $path_dir);
}

/* -------------------------------------------------------------- [E] */

function echo_bl($string)
{
    echo PHP_EOL, $string, PHP_EOL;
}

function echo_eol($string)
{
    echo $string, PHP_EOL;
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

function echo_msg_plz_input()
{
    $msg  = 'トゥート・コマンドを入力してください。';
    $msg .= '（トゥート入力の終了は\'q;[enter]\'）';
    echo $msg . PHP_EOL;

    echo_hr();

    echo NAME_BOT . ':';

    _flush();
}

function echo_same_line($string)
{
    $width_screen = get_screen_width();
    $line_blank   = str_repeat(' ', $width_screen);
    $result       = $string . $line_blank;
    $result       = substr($result, 0, $width_screen);

    echo CR . $result;
}

function encode_qithub($mix)
{
    $result = json_encode((array)$mix);

    return urlencode($result);
}


function explode_str_to_cmd($string)
{
    $exp_pass1  = explode(PHP_EOL, $string, 2);
    $exp_pass2  = explode(' ', $exp_pass1[0], 2);
    $command    = $exp_pass2[0];
    $args       = (1 < count($exp_pass2)) ? $exp_pass2[1] : '';
    $args      .= (1 < count($exp_pass1)) ? $exp_pass1[1] : '';

    $option = [
        'name_cmd' => trim($command),
        'args_cmd' => trim($args),
    ];

    return $option;
}

/* -------------------------------------------------------------- [F] */

function fetch_args()
{
    global $argv;

    if (isset($argv[1])&& ! empty($argv[1])) {
        $argv_tmp    = $argv;
        $plugin_name = trim($argv_tmp[1]);
        unset($argv_tmp[0]); // unset script name
        unset($argv_tmp[1]); // unset 1st argument
        $plugin_args = implode(' ', $argv_tmp); // Glue the rest args.
        set_flag_argv_done(YES);
    } else {
        $plugin_name = 'help';
        $plugin_args = '';
    }

    $option = [
        'name_cmd' => trim($plugin_name),
        'args_cmd' => trim(uniform_eol($plugin_args)),
    ];

    return $option;
}

function fetch_option()
{
    $path = './option.json';

    if (! file_exists($path)) {
        return false;
    }

    $json  = file_get_contents($path);

    return json_decode($json, JSON_OBJECT_AS_ARRAY);
}

function fetch_option_default()
{
    $option = [
        'lang' => '',
    ];

    return $option;
}

function fetch_stdin($cmd_quit = CMD_QUIT)
{
    $result = '';

//    $stdin = fopen('php://stdin', 'r');
//    stream_set_blocking($stdin, false);

    while (true) {
        $in = fgets(STDIN);
        //$in = stream_get_contents($stdin);

        if ($in) {
            if ($cmd_quit === trim($in)) {
                break;
            }
            $result .= $in;
        }
    }

//    unset($in,$stdin);

    return trim(uniform_eol($result));
}

function format_title($title, $width_indent = 0)
{
    $title  = trim((string) $title);
    $result = '';
    $indent = '';

    if ($width_indent) {
        $width  = get_screen_width() - $width_indent;
        $hr     = str_repeat('-', $width);
        $indent = str_repeat(' ', $width_indent);
    } else {
        $hr = echo_hr('-', DO_NOT_ECHO);
    }

    $result .= $indent . $hr;
    $result .= $indent . '  ' . $title . PHP_EOL;
    $result .= $indent . $hr;

    return $result;
}

function fetch_toot_cmd()
{
    global $argv;

    cls();
    echo format_title(TITLE_APP);

    if (isset($argv[1])&& ! empty($argv[1])) {
        if (! is_argv_done()) {
            return fetch_args();
        }
    }

    echo_msg_plz_input();

    $input  = fetch_stdin();
    $option = explode_str_to_cmd($input);

    cls();
    echo format_title(TITLE_APP);
    echo indent_string('Result of conversion toot to Qithub API');

    return $option;
}

/* -------------------------------------------------------------- [G] */

function get_screen_width()
{
    $default_width = DEFAULT_WIDTH_SCREEN; //デフォルト幅

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

/* -------------------------------------------------------------- [H] */

function has($array, $key)
{
    if (isset($array[$key]) && ! empty($array[$key])) {
        return $array[$key];
    }

    return false;
}

/* -------------------------------------------------------------- [I] */


function indent_string($string)
{
    $result = '';
    $lines  = explode(PHP_EOL, $string);

    foreach ($lines as $line) {
        $line    = trim($line, PHP_EOL);
        $result .= INDENT . $line . PHP_EOL;
    }

    return $result;
}

function initialize_app()
{
    set_env_utf8_ja();
    die_if_web();

    @ob_end_flush();
    ob_start('mb_output_handler');

    initialize_option();
}

function initialize_option()
{
    $option = fetch_option();

    if (! $option) {
        cls();
        echo format_title(TITLE_APP);
        echo_eol('設定ファイルがありません。');
        $confirm_key = 'y';
        $msg  = 'デフォルトの設定でファイルを作成します。';
        $msg .= "（${confirm_key}）: ";
        request_user_confirm($msg, $confirm_key);
    } else {
        echo_eol('OK');
    }
}

function is_argv_done()
{
    if (defined('IS_ARGV_DONE')) {
        return IS_ARGV_DONE;
    }

    return NO;
}

function is_cli()
{
    $name_sapi = strtolower(php_sapi_name());

    return ( 'cli' === substr($name_sapi, 0, 3) );
}

/* -------------------------------------------------------------- [P] */

function pause_next()
{
    $continue_key = 'y';
    $question     = "Continue? (${continue_key}): ";

    return request_user_confirm($question, $continue_key);
}

function print_pretty($option)
{
    $title    = has($option, 'title') ?: '';
    $return   = has($option, 'return') ?: false;
    $contents = has($option, 'contents') ?: '';
    $title    = trim((string) $title);

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

/* -------------------------------------------------------------- [R] */

function request_user_confirm($question, $confirm_key)
{
    $confirm_list = explode(' ', $confirm_key);

    while (true) {
        echo_same_line(' ');
        echo CR;
        echo (string) $question;
        _flush();
        $input = trim(fgets(STDIN));
        echo CR, CU;
        if( array_search($input, $confirm_list) ){
            return ($confirm_key == $input);            
        }
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

function set_flag_argv_done($flag = YES)
{
    if (! defined('IS_ARGV_DONE') && $flag===YES) {
        define('IS_ARGV_DONE', YES);
    }
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

/* -------------------------------------------------------------- [U] */

function uniform_eol($string, $to = PHP_EOL)
{
    return strtr($string, array_fill_keys(["\r\n", "\r", "\n"], $to));
}
