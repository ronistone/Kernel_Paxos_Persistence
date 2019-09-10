# Char device tests

This repository assesses char device performance with an echo module and a user-space application.

## Mount Environment With Vagrant
 - Install Virtualbox ``https://www.virtualbox.org/``
 - Install the most updated vagrant version of this link ``https://www.vagrantup.com/downloads.html``
 - Install virtualbox guest additions plugin using ``vagrant plugin install vagrant-vbguest``
 - Create and start the machine using (in the project root folder) ``vagrant up``
 - Connect in the machine using ``vagrant ssh``
 - The work directory is on ``/vagrant``


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
