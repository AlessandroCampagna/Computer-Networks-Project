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

The sockets module is used as an abstraction for opening the communication sockets inside the handler.

The handler is responsible for processing all requests made by the clients, utilizing the data module to read and write to the database located inside the "ASDIR" folder.

The time module is used to format and obtain current time.

### Client