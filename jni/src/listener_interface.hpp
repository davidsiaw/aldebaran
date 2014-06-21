#ifndef LISTENER_INTERFACE_HPP
#define LISTENER_INTERFACE_HPP

template<typename T>
class ListenerInterface
{
public:
    virtual void AcceptNotification(T notification) = 0;
};

#endif // LISTENER_INTERFACE_HPP
