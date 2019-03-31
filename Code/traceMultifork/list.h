#ifndef LIST_H
#define LIST_H
#include <sys/types.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct node {
   pid_t pid;
   int key;

   struct node *next;
   struct node *prev;
};
struct node *head;
struct node *last;
struct node *current;

bool isEmpty();

int length();

void displayForward();

void displayBackward();

void insertFirst(int key, int pid);

void insertLast(int key, int pid);

struct node* deleteFirst();

struct node* deleteLast();

struct node* deletepid(int key);

bool insertAfter(int key, int newKey, int pid);

#endif // LIST_H
