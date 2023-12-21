#ifndef __CLIENT_H__
#define _CLIENT_H__
#define DEFAULT_PLAYERCMD  "/usr/bin/mpg123  -   >/dev/null"

struct client_conf_st
{
    char *rcvport;
    char *mgroup;
    char *player_cmd;

};

#endif
