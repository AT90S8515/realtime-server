#pragma once

//#ifdef IS_LINUX

#include <memory>
#include <unordered_map>
#include <list>
#include <functional>
#include "realtime_srv/common/noncopyable.h"
#include "realtime_srv/common/copyable.h"
#include "realtime_srv/common/RealtimeSrvHelper.h"
#include "realtime_srv/rep/BitStream.h"

#include <realtime_srv/net/PktHandler.h>

namespace realtime_srv
{

class GameObj;
class ClientProxy;

typedef std::function<GameObj*( std::shared_ptr<ClientProxy>& )> NewPlayerCallback;
typedef std::function<InputState*()> CustomInputStateCallback;
typedef std::function<void( std::shared_ptr<ClientProxy> )> LetCliProxyGetWorldStateCb;

class NetworkMgr : noncopyable, public std::enable_shared_from_this<NetworkMgr>
{
public:
	typedef std::function<void( GameObjPtr&, ReplicationAction )> WorldRegistryCb;

	typedef std::unordered_map<muduo::net::UdpConnectionPtr,
		std::shared_ptr<ClientProxy>>	UdpConnToClientMap;

	typedef std::shared_ptr<UdpConnToClientMap> UdpConnToClientMapPtr;

	static const uint32_t	kNullCC = 0;
	static const uint32_t	kHelloCC = 'HELO';
	static const uint32_t	kWelcomeCC = 'WLCM';
	static const uint32_t	kResetCC = 'RSET';
	static const uint32_t	kResetedCC = 'RSTD';
	static const uint32_t	kStateCC = 'STAT';
	static const uint32_t	kInputCC = 'INPT';

	enum UpdateConnListFlag
	{
		INSERT,
		DELETE,
		UPDATE
	};

public:

	NetworkMgr( const ServerConfig _serverConfig );

	void Start();

	muduo::net::EventLoop* GetEventLoop() { return pktHandler_.GetBaseLoop(); }

	void SetRepStateDirty( int _objId, uint32_t inDirtyState );

	void OnObjCreateOrDestory( GameObjPtr& inGameObject, ReplicationAction inAction );

	void SetNewPlayerCallback( const NewPlayerCallback& cb )
	{ newPlayerCb_ = cb; }

	void SetWorldUpdateCallback( const std::function<void()>& cb )
	{ worldUpdateCb_ = cb; }

	void SetWorldRegistryCallback( const WorldRegistryCb& cb )
	{ worldRegistryCb_ = cb; }

	void SetCustomInputStateCallback( const CustomInputStateCallback& cb )
	{ customInputStatecb_ = cb; }

	void SetLetCliProxyGetWorldStateCallback( const LetCliProxyGetWorldStateCb& cb )
	{ letCliProxyGetWorldState_ = cb; }

	bool GetUnregistObjWhenCliDisconn() const
	{ return bUnregistObjWhenCliDisconn_; }

	// if true, Server will run as 'Not Destroy GameObj When Client Disconnect' mode.
	void SetUnregistObjWhenCliDisconn( bool _whehter )
	{ bUnregistObjWhenCliDisconn_ = _whehter; }

	double GetClientDisconnectTimeout() const
	{ return clientDisconnTimeout_; }

private:
	void Tick();
	void DoProcessPkt( ReceivedPacketPtr& recvedPacket );
	void PreparePacketToSend();
	void CheckForDisconnects();

	uint32_t	 HandleServerReset( std::shared_ptr<ClientProxy>& inClientProxy,
		InputBitStream& inInputStream );

	void	HandleInputPacket( std::shared_ptr<ClientProxy>& inClientProxy,
		InputBitStream& inInputStream );

	void	CheckPacketType( std::shared_ptr<ClientProxy>& inClientProxy,
		InputBitStream& inInputStream );

	void	WriteLastMoveTimestampIfDirty( OutputBitStream& inOutputStream,
		std::shared_ptr<ClientProxy>& inClientProxy );

	void DoPreparePacketToSend( std::shared_ptr<ClientProxy>& inClientProxy,
		const uint32_t inConnFlag );

	void	WelcomeNewClient( InputBitStream& inInputStream,
		const muduo::net::UdpConnectionPtr& inUdpConnetction,
		const pid_t inHoldedByThreadId );

	std::shared_ptr<ClientProxy> CreateNewClient(
		const muduo::net::UdpConnectionPtr& _udpConnetction,
		const pid_t _holdedByThreadId );

	void UpdateConnListForCheckDisconn(const muduo::net::UdpConnectionPtr& conn,
		UpdateConnListFlag flag, muduo::Timestamp time = muduo::Timestamp());

private:

	bool bUnregistObjWhenCliDisconn_;
	uint8_t actionCountPerTick_;

	LetCliProxyGetWorldStateCb letCliProxyGetWorldState_;
	NewPlayerCallback newPlayerCb_;
	std::function<void()> worldUpdateCb_;
	WorldRegistryCb worldRegistryCb_;
	CustomInputStateCallback customInputStatecb_;

	PktHandler pktHandler_;

	UdpConnToClientMap					udpConnToClientMap_;
	static muduo::AtomicInt32		kNewNetId;

	// check disconn
	typedef std::weak_ptr<muduo::net::UdpConnection> WeakUdpConnectionPtr;
	typedef std::list<WeakUdpConnectionPtr> WeakConnectionList;

	struct NodeForCheckDisconn : public realtime_srv::copyable
	{
		muduo::Timestamp lastRecvTime;
		WeakConnectionList::iterator position;
	};
	WeakConnectionList connListForCheckDisconn_;
	const double clientDisconnTimeout_;

};
typedef std::shared_ptr<NetworkMgr> NetworkMgrPtr;

}

//#else
//#include "realtime_srv/net/NetworkMgrWinH.h"
//#endif //IS_LINUX