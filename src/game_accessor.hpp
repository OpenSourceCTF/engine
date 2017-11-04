#ifndef ML_GAME_ACCESSOR_HPP
#define ML_GAME_ACCESSOR_HPP

struct game;
struct server_event;

struct game_accessor
{
private:
    game* g;

public:
    game_accessor();
    game_accessor(game* g);

    void set_game(game* g);
    void add_server_event(server_event && e);

    operator game*();
};

#endif

