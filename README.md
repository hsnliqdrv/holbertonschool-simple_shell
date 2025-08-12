# Simple Shell

This repository is for Holberton School's "Simple Shell" project

## Installation

sh```
#clone repository
git clone https://github.com/hsnliqdrv/holbertonschool-simple_shell
#compile shell
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```

## Usage

sh```
#Interactive mode
$ ./hsh
($) /bin/ls
hsh main.c shell.c
($)
($) exit
#Non-interactive mode
$ echo "/bin/ls" | ./hsh
hsh main.c shell.c test_ls_2
```

## Contributors

- Hasanali Qadirov (@hsnliqdrv)
- Jamil Hajizade (@Eisenwall)
