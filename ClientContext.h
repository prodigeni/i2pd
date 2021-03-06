#ifndef CLIENT_CONTEXT_H__
#define CLIENT_CONTEXT_H__

#include <map>
#include <mutex>
#include <memory>
#include "Destination.h"
#include "HTTPProxy.h"
#include "SOCKS.h"
#include "I2PTunnel.h"
#include "SAM.h"
#include "BOB.h"
#include "AddressBook.h"
#include "I2PControl.h"

namespace i2p
{
namespace client
{
	const char I2P_CLIENT_TUNNEL_NAME[] = "client.name";
	const char I2P_CLIENT_TUNNEL_PORT[] = "client.port";
	const char I2P_CLIENT_TUNNEL_DESTINATION[] = "client.destination";
	const char I2P_CLIENT_TUNNEL_KEYS[] = "client.keys";
	const char I2P_SERVER_TUNNEL_NAME[] = "server.name";
	const char I2P_SERVER_TUNNEL_HOST[] = "server.host";	
	const char I2P_SERVER_TUNNEL_PORT[] = "server.port";
	const char I2P_SERVER_TUNNEL_KEYS[] = "server.keys";
	const char TUNNELS_CONFIG_FILENAME[] = "tunnels.cfg";

	class ClientContext
	{
		public:

			ClientContext ();
			~ClientContext ();

			void Start ();
			void Stop ();

			ClientDestination * GetSharedLocalDestination () const { return m_SharedLocalDestination; };
			ClientDestination * CreateNewLocalDestination (bool isPublic = false, i2p::data::SigningKeyType sigType = i2p::data::SIGNING_KEY_TYPE_DSA_SHA1,
			    const std::map<std::string, std::string> * params = nullptr); // transient
			ClientDestination * CreateNewLocalDestination (const i2p::data::PrivateKeys& keys, bool isPublic = true, 
				const std::map<std::string, std::string> * params = nullptr);
			void DeleteLocalDestination (ClientDestination * destination);
			ClientDestination * FindLocalDestination (const i2p::data::IdentHash& destination) const;		
			ClientDestination * LoadLocalDestination (const std::string& filename, bool isPublic);

			AddressBook& GetAddressBook () { return m_AddressBook; };
		
		private:

			void ReadTunnels ();
	
		private:

			std::mutex m_DestinationsMutex;
			std::map<i2p::data::IdentHash, ClientDestination *> m_Destinations;
			ClientDestination * m_SharedLocalDestination;	

			AddressBook m_AddressBook;

			i2p::proxy::HTTPProxy * m_HttpProxy;
			i2p::proxy::SOCKSProxy * m_SocksProxy;
			std::map<int, std::unique_ptr<I2PClientTunnel> > m_ClientTunnels; // port->tunnel
			std::map<i2p::data::IdentHash, std::unique_ptr<I2PServerTunnel> > m_ServerTunnels; // destination->tunnel
			SAMBridge * m_SamBridge;
			BOBCommandChannel * m_BOBCommandChannel;
			I2PControlService * m_I2PControlService;

		public:
			// for HTTP
			const decltype(m_Destinations)& GetDestinations () const { return m_Destinations; };
	};
	
	extern ClientContext context;	
}		
}	

#endif
