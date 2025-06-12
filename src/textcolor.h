#ifndef TEXTCOLOR_H
#define TEXTCOLOR_H
#define black(text) "\033[30m" text "\033[0m"
#define red(text) "\033[31m" text "\033[0m"
#define green(text) "\033[32m" text "\033[0m"
#define yellow(text) "\033[33m" text "\033[0m"
#define blue(text) "\033[34m" text "\033[0m"
#define magenta(text) "\033[35m" text "\033[0m"
#define cyan(text) "\033[36m" text "\033[0m"
#define white(text) "\033[37m" text "\033[0m"
// Bold versions
#define bold_black(text) "\033[1;30m" text "\033[0m"
#define bold_red(text) "\033[1;31m" text "\033[0m"
#define bold_green(text) "\033[1;32m" text "\033[0m"
#define bold_yellow(text) "\033[1;33m" text "\033[0m"
#define bold_blue(text) "\033[1;34m" text "\033[0m"
#define bold_magenta(text) "\033[1;35m" text "\033[0m"
#define bold_cyan(text) "\033[1;36m" text "\033[0m"
#define bold_white(text) "\033[1;37m" text "\033[0m"
#endif