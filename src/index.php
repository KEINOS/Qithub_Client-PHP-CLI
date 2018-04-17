<?php
/**
 * Qithub client PHP-CLI ver.
 *
 * A simple PHP command line client application for Qithub.
 *
 * @ver 20180113
 * @author KEINOS( https://github.com/KEINOS )
 */

require_once('./constants.php.inc');
require_once('./functions.php.inc');

set_env_utf8_ja();

die_if_web();

// Get args from CLI
if (isset($argv[1])&& ! empty($argv[1])) {
    $argv_tmp    = $argv;
    $plugin_name = trim($argv_tmp[1]);
    unset($argv_tmp[0]); // unset script name
    unset($argv_tmp[1]); // unset 1st argument
    $plugin_args = implode(' ', $argv_tmp); // Glue the rest args.
} else {
    $plugin_name = 'help';
    $plugin_args = '';
}

// Qithub API Endpoint
$url_endpoint = 'https://blog.keinos.com/qithub_dev/';

// REST Request URL
$plugin_name = urlencode($plugin_name);
$plugin_args = urlencode($plugin_args);
$url_request = "${url_endpoint}?plugin_name=${plugin_name}&args=${plugin_args}";

// Get Qithub encoded data from Qithub API
$response_raw = file_get_contents($url_request);

// Decode Qithub encoded data to PHP array
$response_json  = urldecode($response_raw);
$response_array = json_decode($response_json, JSON_OBJECT_AS_ARRAY);

// Show result(response)
if ('OK' === $response_array['result']) {
    echo $response_array['value'] . PHP_EOL;
} else {
    $value  = '';
    $value .= print_pretty($url_request, 'Request query:', true);
    $value .= print_pretty($response_array['value'], 'Value:', true);
    print_pretty($value, 'Bad Request');
}
