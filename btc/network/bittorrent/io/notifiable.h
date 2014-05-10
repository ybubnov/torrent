#pragma once

namespace network{
    namespace bittorrent{
        namespace io{
            class notifiable{
                public:
                    virtual void notify(double percents) = 0;
            };
        }
    }
}
