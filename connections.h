#ifndef CONNECTIONS_H
#define CONNECTIONS_H

int initConnections(int threadCount);

void handleConnection(int clientSocket);