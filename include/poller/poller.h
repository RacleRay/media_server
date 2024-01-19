#ifndef __POLLER__H__
#define __POLLER__H__

#include <map>

#include "event.h"


namespace RACLE {

class Poller {
public:
    virtual ~Poller() = default;

    virtual bool addIOEvent(IOEvent* event) = 0;
    virtual bool updateIOEvent(IOEvent* event) = 0;
    virtual bool removeIOEvent(IOEvent* event) = 0;
    virtual void handleEvent() = 0;

protected:
    using IOEventMap = std::map<int, IOEvent*>;

    IOEventMap mIOEventMap;
};

}  // namespace RACLE





#endif  //!__POLLER__H__