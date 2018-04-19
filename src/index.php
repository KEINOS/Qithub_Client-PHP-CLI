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


initialize_app();

/*
$i = 0;
while(true){
    move_up( $i . ' say hello world hogehoge hoge ');
    $i++;
}
*/

while (true) {
    $option_raw = fetch_toot_cmd();
    $option_enc = encode_qithub($option_raw);

    print_pretty([
        'contents' => $option_raw,
        'title'    => 'Command in Array',
    ]);
    print_pretty([
        'contents' => json_encode($option_raw, JSON_PRETTY_PRINT),
        'title'    => 'Command in JSON',
    ]);
    print_pretty([
        'contents' => $option_enc,
        'title'    => 'Qithub encoded'
    ]);
    
    echo_hr();
    pause_next();

    //ヘッダを１行上の先頭に移動
    //echo("\r\33[1A");
}

die;

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
