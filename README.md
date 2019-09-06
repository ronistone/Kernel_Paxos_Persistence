# Char device tests

This repository assesses char device performance with an echo module and a user-space application.

## Compiling

Simply run ``make``

## Loading the module

Run: ``sudo insmod build/persistence.ko``

## Running user-space application

Run ``sudo ./build/user_chardev -c /dev/paxos/persistence0``,
or ``sudo ./build/user_chardev -c /dev/paxos/persistence0 -v`` to see all messages.

## Unloading everything

Kill the application with CTRL+C and check the throughput.

Unload the module with ``sudo rmmod persistence`` 
