# minishell

## Description
A implementation of a simple Unix shell in C. This is a 42 team project by mrizakov & amaucher.

Key features include:

- Command Execution: Implementing command parsing and execution using system calls like execve.
- Built-in Commands: Supporting built-ins such as echo, cd, pwd, export, unset, env, and exit.
- Signal Handling: Managing signals like SIGINT and SIGQUIT.
- Environment Management: Handling environment variables within the shell session.
- I/O Redirection and Pipes: Implementing <, >, >> for redirection and | for piping commands.

## Project Structure

The project is splitted into the submodules Lexer, Parser and Executor.

### Lexer
The Lexer takes a stream of bytes, which the user types into the terminal via the readline() function. Through the implemented lexical analysis the bytestream is split up into different tokens. The Lexer also removes whitespaces at the end, merges WORDS (hell"o" -> hello) and expands variables (e.g. $PATH).

### Parser
The parser takes the linked list of tokens and generates another linked list of commands. Information about arguments, pipes, redirections and command type (BUILTIN or PATH) is extracted by the parsing algorithm and stored inside a command struct.

### Executor
The executor takes the linked list of commands and executes them in the desired way. If there is only a single BUILTIN command (e.g. cd) called, the command gets executed in the current minishell-process. If a PATH command or piped mixed commands are called (e.g. echo hello | grep hello) the minishell-process forks child processes to execute every command in its own child process and waits for its execution and grabs the exit code to give it back to the calling process.

## Installation

Compile with make.

## Usage

Run the shell by executing <./minishell>. The shell can be used like a regular bash shell but with limited functionality.
