#pragma once

namespace network{
    typedef class GameOverableInterface{
        public:
            virtual void game_over() = 0;
            virtual ~GameOverableInterface(){}
    }game_overable;
}
