#ifndef SERVER_STATE_HPP
#define SERVER_STATE_HPP

class DedicatedServer;

class ServerState
{
public:
    void Init(DedicatedServer* server)
    {
        this->server = server;
    }
    
    virtual void Update() = 0;

//protected:
    DedicatedServer* server;
};

#endif // !SERVER_STATE_HPP