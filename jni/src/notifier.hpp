#ifndef NOTIFIER_HPP
#define NOTIFIER_HPP

#include <deque>
#include <list>
#include <boost/tr1/memory.hpp>

#include "listener_interface.hpp"

// Class for passing notifications

template<typename T>
class Notifier
{
    std::tr1::shared_ptr< std::deque<T> > nextNotificationBatch;
    std::list< std::tr1::weak_ptr< ListenerInterface<T> > > listeners;
    
public:
    Notifier() : nextNotificationBatch(new std::deque<T>())
    { }
    
    void QueueNotification(T notification)
    {
        nextNotificationBatch->push_back(notification);
    }
    
    void AddListener(std::tr1::weak_ptr< ListenerInterface<T> > listener)
    {
        listeners.push_back(listener);
    }
    
    void ProcessNotifications()
    {
        auto nots = nextNotificationBatch;
        nextNotificationBatch = std::tr1::shared_ptr< std::deque<T> >(new std::deque<T>);
        
        listeners.remove_if([](std::tr1::weak_ptr< ListenerInterface<T> > ptr) -> bool
        {
            if (ptr.lock())
            {
                return false;
            }
            return true;
        });

        for (auto n = nots->begin(); n != nots->end(); n++)
        {
            for (auto l = listeners.begin(); l != listeners.end(); l++)
            {
                auto listener = l->lock();
                if (listener)
                {
                    listener->AcceptNotification(*n);
                }
            }
        }
    }
};

#endif // NOTIFIER_HPP
