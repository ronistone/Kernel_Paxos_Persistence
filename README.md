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

Run ``sudo ./build/user_storage -w /dev/paxos/write-persistence1 -r /dev/paxos/read-persistence2``,  
or ``sudo ./build/user_storage -w /dev/paxos/write-persistence1 -r /dev/paxos/read-persistence2 -v`` to see all messages.

## To Test
Run ``sudo ./build/testes`` to send messages to the kernel space it will send to user_storage to consume

## Unloading everything

Kill the application with CTRL+C and check the throughput.

Unload the module with ``sudo rmmod persistence`` 
