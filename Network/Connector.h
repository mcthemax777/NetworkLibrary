#pragma once

#include <string>
#include <functional>
#include <map>

#include "Define.h"
#include "ConnectorInfo.h"
#include "NetworkPacket.h"

namespace CG
{
	typedef int8_t ConnectorType;

	enum CONNECTOR_TYPE
	{
		CONNECTOR_TYPE_SERVER = 0,
		CONNECTOR_TYPE_CLIENT,
		CONNECTOR_TYPE_PEER,
	};

	class ConnectConfig
	{
	public:
		std::string ip;
		unsigned short port;
		DataConvertor* dataConvertor;
	};

	class Connector
	{
	public:
		void start(ConnectConfig* config);
		void stop();

		std::function<void(HostId)> onConnect;
		std::function<void(HostId)> onDisconnect;
		std::function<void(HostId, char*, int)> onReceive;
		DataConvertor* dataConvertor;

		ConnectorType getConnectorType() { return type; }

		friend class Network;
		friend class ClientNetwork;
	protected:
		Connector();

		ConnectorType type;
		ConnectorInfo* connectorInfo;
		bool isConnected;



	public:
		template<typename T, typename std::enable_if<std::is_base_of<CG::NetworkPacket, T>::value>::type* = nullptr>
		void registerPacket(std::function<void(HostId, T*)> onReceiveNPacket)
		{
			T* t = new T();
			NetworkPacket* packet = (NetworkPacket*)t;
			packet->onReceiveNPacket = (void*)&onReceiveNPacket;
			onReceiveNPacket(1, nullptr);
			(*((std::function<void*(HostId, T*)> *)packet->onReceiveNPacket))(1, packet);

			if (npMap.insert(std::pair<npType_t, NetworkPacket*>(packet->header.npType, packet)).second == false)
			{
				ErrorLog("???");
			}

			if (npFunctionMap.insert(std::pair<npType_t, void*>(packet->header.npType, (void*)&onReceiveNPacket)).second == false)
			{
				ErrorLog("???");
			}


			/*NetworkPacket* p = packet->create();
			std::printf("%s\n\n\n\n", typeid(*p).name());*/
		}

		std::map<npType_t, NetworkPacket*> npMap;
		std::map<npType_t, void*> npFunctionMap;

		bool isCGModule;
	};
}
