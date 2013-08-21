#pragma once

namespace network{
    namespace bittorrent{
        namespace core{
            class executable{
                public:
                    virtual void interrupt() = 0;                                   //interrupt execution
                    virtual void timed_join(long) = 0;                              //interrupt after 'value' milliseconds
                    virtual void yeild() = 0;                                       //start execution

                    virtual ~executable(){}
            };
        }
    }
}
