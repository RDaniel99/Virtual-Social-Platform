#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <vector>
#include <string>
#include <ctime>
#include <cstdio>
#include <cerrno>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

using namespace std;

#define PORT 2567

#define FILE_USERS_PASSWORDS    "users_passwords.txt"
#define FILE_USERS_NAMES        "users_names.txt"
#define FILE_USERS_FRIENDSHIPS  "users_friendships.txt"
#define FILE_USERS_POSTS        "users_posts.txt"

typedef int CId;

#endif //UTILS_H
