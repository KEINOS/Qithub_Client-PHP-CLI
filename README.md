
This is a prototype. Please wait until basic implementation done.

# Qithub CLI Client

`qithub.phar` is a single PHP command line application that calls Qithub API and returns the result.

## Install

Just donwload `qithub.phar` and run with arguments.


## Basic usage

```
$ php qithub.phar roll-dice 3d5
14
$ php qithub.phar roll-dice --help
This is a help of `roll-dice` plugin.
$
```

## Better usage

If you place the `qithub.phar` file under `/usr/local/bin/qithub` with 0755, you can run it as below.

```
$ qithub roll-dice 3d5
14
$ qithub roll-dice --help
This is a help of `roll-dice` plugin.
$
```


