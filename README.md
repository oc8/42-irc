# 42 IRC Project

This is an IRC project for the 42 school. It allows users to connect to an IRC server and participate in channels.

## Prerequisites

- A C compiler (such as GCC)

## Installation

1. Clone this repository to your computer
2. Navigate to the project directory
3. Compile the project with the command `make`

## Usage

To use the IRC client, run the `irc` executable with the following command:

```
./ircserv <port> <password>
```

```
./ircbot <ip> <port> <password>
```


## weechat
```
/server add ircserv <ip>/<port> -password=<password>
```

## Make

The following make targets are available:

- `make`: Compile the project
- `make clean`: Remove compiled files
- `make fclean`: Remove compiled files and the `irc` executable
- `make re`: Re-compile the project
