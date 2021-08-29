#pragma once

#include <bld_abstract_net.h>


class B_NullNetInterface: public bld_abstract_net
{
public:
    virtual unsigned long get_session_player_dpid()
    {
        return 0;
    }

    virtual unsigned long get_all_players_id()
    {
        return 0;
    }

    virtual bool is_net_game()
    {
        return false;
    }

    virtual bool is_server()
    {
        return false;
    }

    virtual unsigned long get_player_dpid()
    {
        return 0;
    }

    virtual long send_message(
        unsigned long idTo, void *lpData, unsigned long wDataSize)
    {
        return 0;
    }

    virtual long send_guaranteed_message(
        unsigned long idTo, void *lpData, unsigned long wDataSize)
    {
        return 0;
    }

    virtual bool start_server(
        const char *game_name, const char *player_name, int max_players,
        bool TCP)
    {
        return false;
    }

    virtual bool browse_sessions(const char *ip_address)
    {
        return false;
    }

    virtual bool get_browse_result(int index, bld_server_info *info)
    {
        return false;
    }

    virtual bool join_session(int index, const char *player_name)
    {
        return false;
    }

    virtual bool is_valid_protocol(bool tcp)
    {
        return false;
    }
};
