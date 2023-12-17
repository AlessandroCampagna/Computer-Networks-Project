# Instituto Superior Técnico - Computer Networks - Project 23/24

### Group: 17

ist1103210 - José Santos Corte \
ist1106751 - Alessandro Costa Campagna

## Introduction

This projects implements a simple client-server relationship for a hypothetical auction service.

## Code Structure

### Server

The AS server follows a hierarchical structure.

The main function lives inside the server module where the command line arguments are parsed and the TCP and UDP channels are forked.

The sockets module is used as an abstraction for opening the communication sockets inside the connection module where the requests and responses are received and sent.

The handler is responsible for processing all requests made by the clients, utilizing the data module to read and write to the database located inside the "ASDIR" folder.

The time module is used to format and obtain current time.

### Client

The Client or User follows hierarchical structure ( user -> handler -> connection ).

#### User:

The main function lives inside the user module where the user commands are received and signals are caught. The buffer containing the commands is then sent to the handler module for further processing.

#### Handler:

The handler module is responsible for processing all requests made by the clients. It receives the commands and signals from the user module and performs the necessary operations based on the command type as well as connection protocol(UDP or TCP).

#### Connection:

The connection module handles the connection between the client and the server. It is responsible for establishing the connection, sending requests, and receiving responses/files.

#### Additional info

    - The Folder ASSETS need to be on the same directoriy as src
    - Both the sent files and recieved ones need to be stored in the ASSETS folder
