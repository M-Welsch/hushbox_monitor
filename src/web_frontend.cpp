#ifndef _WEB_FRONTEND_H
#include "web_frontend.h"
#define _WEB_FRONTEND_H_
#endif

String heading(const char *text, int level) {
    char buffer[64];
    sprintf(buffer, "<h%d>%s</h%d>", level, text, level);
    return buffer;
}