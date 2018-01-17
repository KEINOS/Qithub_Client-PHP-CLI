<?php

const DIR_SEP = DIRECTORY_SEPARATOR;
const DIR_CURRENT      = '.' . DIR_SEP;
const DIR_PARRENT      = '..' . DIR_SEP;
const NAME_DIR_SRC     = 'src';
const NAME_DIR_UTILS   = 'utils';
const NAME_FILE_OPTION = 'rebuild_phar.json';
const NAME_FILE_BOX    = 'box.phar';
const PATH_DIR_SRC     = NAME_DIR_SRC . DIR_SEP;
const PATH_DIR_UTILS   = DIR_CURRENT . NAME_DIR_UTILS . DIR_SEP;
const PATH_FILE_OPTION = DIR_CURRENT . NAME_FILE_OPTION;
const PATH_FILE_BOX    = PATH_DIR_UTILS . 'bin' .DIR_SEP . NAME_FILE_BOX;

function is_phar_readonly()
{
    return (ini_get('phar.readonly') || ini_get('phar.require_hash'));
}

function set_files_under($name_dir, $files)
{
    foreach ($files as $key => $name_file) {
        $path_file_tmp = $name_dir . DIR_SEP . $name_file;
        $files[$key] = $path_file_tmp;
    }

    return $files;
}
