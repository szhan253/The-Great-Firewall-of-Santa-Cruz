# The Great Firewall of Santa Cruz: Bloom Filters, Linked Lists, Binary Trees and Hash Tables
This is an assignment for CSE13S in Fall 2021. Please do not use the code for any class assignments. This is a program that read the input from the user and tries to catch the old and bad word from the user input. It uses the bloom filter and hash table to contain the oldspeak and newspeak which are given in two files. The program compares the user input with the given list and output a message that tells the user about what word they need to change.

## Building
Building the program with:

```
$ make
```

## Running
Running the program with:

###### banhammer.c
```
./banhammer [-hs] [-t size] [-f size]
```

## Help Message
###### banhammer.c
      -h           Program usage and help.
      -s           Print program statistics.
      -t size      Specify hash table size (default: 2^16).
      -f size      Specify Bloom filter size (default: 2^20).
