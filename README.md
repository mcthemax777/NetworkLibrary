# NetworkLibrary

NetworkLibrary offer network function that is easy to use connecting with other.

***

## environment

### thread
 - multi
 - single (client module)

### platform
 - linux (epoll)
 - mac (kqueue)
 - window (simple rcv, send) - just develop mode(develop iocp, later)

### language
 - c++ only

### exteral using
 - doxygen
***

## explain

### What is NetworkLibrary?

NetworkLibrary offer network function that is easy to use connecting with other.

To Use easily, we offer two modules
 - tcp snd/rcv transmission module
 - packet snd/rcv transmission module (Ofcourse, tcp)

#### simple architacture

<img src="https://user-images.githubusercontent.com/41141549/45155014-76fa3800-b214-11e8-80fe-a4971e4635cf.PNG" width="100%"></img>

#### simple class diagram

<img src="https://user-images.githubusercontent.com/41141549/45155090-bfb1f100-b214-11e8-86d0-3f8f0559c7b1.PNG" width="100%"></img>



***

## example

### 1. tcp snd/rcv exameple

#### Server

create server instance
~~~cpp
CG::Server* server = new CG::Server();
~~~

init server config
~~~cpp
CG::ServerConfig serverConfig;

serverConfig.ip = "127.0.0.1";
serverConfig.port = 8080;
~~~

connect with client
~~~cpp
server->onConnect = [](CG::HostId hostId)
{
  std::cout << "connected with client";
};
~~~

disconnect with client
~~~cpp
server->onDisconnect = [](CG::HostId hostId)
{
  std::cout << "disconnected with client";
};
~~~

receive data from client
~~~cpp
server->onReceive = [&](CG::HostId hostId, char* data, int dataSize)
{
    //print receive message
    char* receiveData = new char[dataSize + 1];

    memcpy(receiveData, data, dataSize);
    receiveData[dataSize] = 0;

    std::cout << "receive data from client - " << receiveData;
};
~~~

send data to client
~~~cpp
char* strt = "hellohello";
server->sendMessage(hostId, strt, 10);
~~~

start server
~~~cpp
server->start(&serverConfig);
~~~

stop server
~~~cpp
server->stop();
~~~


#### Client

create client instance
~~~cpp
CG::Client* client = new CG::Client();
~~~

init client config
~~~cpp
CG::ClientConfig clientConfig;

clientConfig.ip = "127.0.0.1";
clientConfig.port = 8080;
~~~

connect with server
~~~cpp
client->onConnect = [&](CG::HostId hostId)
{
  std::cout << "connected with server";
};
~~~

disconnect with server
~~~cpp
client->onDisconnect = [](CG::HostId hostId)
{
  std::cout << "disconnected with server";
};
~~~

receive data from server
~~~cpp
client->onReceive = [&](CG::HostId hostId, char* data, int dataSize)
{
  //print receive message
  char* receiveData = new char[dataSize + 1];

  memcpy(receiveData, data, dataSize);
  receiveData[dataSize] = 0;

  std::cout << "receive data from server - " <<receiveData;
};
~~~

send data to server
~~~cpp
char* strt = "hellohello";
client->sendMessage(strt, 10);
~~~

start client
~~~cpp
client->start(&clientConfig);
~~~

stop client
~~~cpp
client->stop();
~~~

### 2. packet snd/rcv exameple

#### packet

make packet class
~~~cpp
class MessagePacket : public NetworkPacket
{
public:
  MessagePacket()
  {
    //set type
    header.npType = MESSAGE_TYPE_MESSAGE;

    //init serial member value
    addMemberValue(&str);
  }

  CREATE_PACKET(CG::MessagePacket)

public:
  std::string str;
};
~~~


#### Server

create server instance
~~~cpp
CG::CGServer* server = new CG::CGServer();
~~~

init server config
~~~cpp
CG::ServerConfig serverConfig;

serverConfig.ip = "127.0.0.1";
serverConfig.port = 8080;
~~~

connect with client
~~~cpp
server->onConnect = [](CG::HostId hostId)
{
  std::cout << "connected with client";
};
~~~

disconnect with client
~~~cpp
server->onDisconnect = [](CG::HostId hostId)
{
  DebugLog("disconnected with client");
};
~~~

receive packet from client
~~~cpp
server->registerPacket<CG::MessagePacket>([&](CG::HostId hostId, CG::NetworkPacket* packet)
{
  CG::MessagePacket* p = (CG::MessagePacket*)packet;
  std::cout << "receive packet - str : " << p->str.c_str();
});
~~~

send packet to client
~~~cpp
MessagePacket* p = new MessagePacket();
p->str = "hello";

server->sendPacket(hostId, p);
~~~

start server
~~~cpp
server->start(&serverConfig);
~~~

stop server
~~~cpp
server->stop();
~~~


#### Client

create client instance
~~~cpp
CG::CGClient* client = new CG::CGClient();
~~~

init client config
~~~cpp
CG::ClientConfig clientConfig;

clientConfig.ip = "127.0.0.1";
clientConfig.port = 8080;
~~~

connect with server
~~~cpp
client->onConnect = [&](CG::HostId hostId)
{
  std::cout << "connected with server";
};
~~~

disconnect with server
~~~cpp
client->onDisconnect = [](CG::HostId hostId)
{
  std::cout << "disconnected with server";
};
~~~

receive packet from server
~~~cpp
client->registerPacket<CG::MessagePacket>([](CG::HostId hostId, CG::NetworkPacket* packet)
{
  CG::MessagePacket* p = (CG::MessagePacket*)packet;
  std::cout << "receive packet - str : " << p->str.c_str();

});
~~~

send packet to server
~~~cpp
MessagePacket* p = new MessagePacket();
p->str = "hello";

server->sendPacket(hostId, p);
~~~

start client
~~~cpp
client->start(&clientConfig);
~~~

stop client
~~~cpp
client->stop();
~~~
