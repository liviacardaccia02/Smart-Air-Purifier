#ifndef __CONNECTION_HANDLER__
#define __CONNECTION_HANDLER__

class ConnectionHandler
{
public:
    virtual void connect() = 0;
    virtual void checkConnection() = 0;
    virtual ~ConnectionHandler() = default;
};

#endif