# NetworkLibrary

NetworkLibrary offer network function that is easy to use connecting with other.

## example

### snd/rcv data exameple

#### Server

    //create client instance
    CG::Server* server = new CG::Server();

    //init server config
    CG::ServerConfig serverConfig;

    serverConfig.ip = "127.0.0.1";
    serverConfig.port = 8080;


    //connect with client
    server->onConnect = [](CG::HostId hostId)
    {
      DebugLog("client와 연결되었습니다.");
    };

    //disconnect with client.
    server->onDisconnect = [](CG::HostId hostId)
    {
      DebugLog("client와 연결이 해제되었습니다.");
    };

    receive data from client
    server->onReceive = [&](CG::HostId hostId, char* data, int dataSize)
    {
    	//print receive message
    	char* receiveData = new char[dataSize];

    	memcpy(receiveData, data, dataSize);
    	receiveData[dataSize] = 0;

    	DebugLog("client로부터 메세지를 받았습니다. %s", receiveData);
    };

    //start server
    server->start(&serverConfig);

    //stop server
    server->stop();
    

#### Client

    //create client instance
    CG::Client* client = new CG::Client();

    //init client config
    CG::ClientConfig clientConfig;

    clientConfig.ip = "127.0.0.1";
    clientConfig.port = 8080;
    //clientConfig.dataConvertor = new CG::CGDataConvertor();

    //connect with server
    client->onConnect = [&](CG::HostId hostId)
    {
      DebugLog("server와 연결되었습니다.");

      //send test message
      char* strt = "hellohello";
      client->sendMessage(strt, 10);

      delete strt;
    };

    //disconnect with server.
    client->onDisconnect = [](CG::HostId hostId)
    {
      DebugLog("server와 연결이 종료되었습니다.");
    };

    //receive data from server
    client->onReceive = [&](CG::HostId hostId, char* data, int dataSize)
    {
      //print receive message
      char* receiveData = new char[dataSize];

      memcpy(receiveData, data, dataSize);
      receiveData[dataSize] = 0;

      DebugLog("server로부터 메세지를 받았습니다. %s", receiveData);
    };

    //start connecting with server
    client->start(&clientConfig);
    
    //stop
    //client->stop();
    
***

### snd/rcv packet exameple

#### packet

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

#### Server

    //create client instance
    CG::CGServer* server = new CG::CGServer();

    //init server config
    CG::ServerConfig serverConfig;

    serverConfig.ip = "127.0.0.1";
    serverConfig.port = 8080;


    //connect with client
    server->onConnect = [](CG::HostId hostId)
    {
      DebugLog("client와 연결되었습니다.");
    };

    //disconnect with client.
    server->onDisconnect = [](CG::HostId hostId)
    {
      DebugLog("client와 연결이 해제되었습니다.");
    };

    //receive packet from client
    server->registerPacket<CG::MessagePacket>([&](CG::HostId hostId, CG::NetworkPacket* packet)
    {
      CG::MessagePacket* p = (CG::MessagePacket*)packet;
      DebugLog("%s", p->str.c_str());
      
      server->sendPacket(hostId, p);
    });

    //start server
    server->start(&serverConfig);

    //stop server
    server->stop();

#### Client

    //create client instance
    CG::CGClient* client = new CG::CGClient();

    //init client config
    CG::ClientConfig clientConfig;

    clientConfig.ip = "127.0.0.1";
    clientConfig.port = 8080;

    //connect with server
    client->onConnect = [&](CG::HostId hostId)
    {
      DebugLog("server와 연결되었습니다.");

      //send test message
      std::string strt = "hellohello";
      CG::MessagePacket* packet = new CG::MessagePacket();
      packet->str = strt;

      client->sendPacket(packet);
    };

    //disconnect with server.
    client->onDisconnect = [](CG::HostId hostId)
    {
      DebugLog("server와 연결이 종료되었습니다.");
    };

    //receive packet from client
    client->registerPacket<CG::MessagePacket>([](CG::HostId hostId, CG::NetworkPacket* packet)
    {
      CG::MessagePacket* p = (CG::MessagePacket*)packet;
      DebugLog("receive packet");

    });

    //start connecting with server
    client->start(&clientConfig);
    //stop
    client->stop();
    
    
