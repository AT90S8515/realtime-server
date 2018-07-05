#include <realtime_srv/RealtimeServer.h>
#include "Character.h"


using namespace realtime_srv;


class ExampleSrvForUe4Demo : noncopyable
{
public:
	ExampleSrvForUe4Demo( bool willDaemonizeOnLinux = false )
		: server_( willDaemonizeOnLinux )
	{}

	void Run()
	{
		//	�����������:   ./example_for_ue4_demo.exe 0 0.3 0.8 1
		//	�����������ģ�� : 
		//	-	ģ���ӳ�Ϊ 0.3 : ��ǰ�ӳ�+300������ӳ�, ����ǰ�ӳ�Ϊ30ms,
		//			��ģ��֮����ӳ�ԼΪ30+300=330����
		//	-	ģ�ⶪ����Ϊ 0.8 : �ٷ�֮��ʮ�Ķ�����
		//	-	ģ�����綶��Ϊ : 1Ϊ�������, 0Ϊ������
		const uint8_t SimulateLatencyCmdIndex = 2;
		const uint8_t SimulateDropPacketChanceCmdIndex = 3;
		const uint8_t SimulateJitterCmdIndex = 4;

		server_.SimulateRealWorld(
			SimulateLatencyCmdIndex,
			SimulateDropPacketChanceCmdIndex,
			SimulateJitterCmdIndex );

		server_.Run( [&]( ClientProxyPtr cp ) { return SpawnNewCharacterForPlayer( cp ); } );
	}

	GameObjPtr SpawnNewCharacterForPlayer( ClientProxyPtr cliProxy )
	{
		GameObjPtr newGameObj = Character::StaticCreate();
		CharacterPtr newCharacter = std::static_pointer_cast< Character >( newGameObj );

		newCharacter->SetPlayerId( cliProxy->GetPlayerId() );
		newCharacter->SetLocation( Vector3(
			2500.f + RealtimeSrvMath::GetRandomFloat() * -5000.f,
			2500.f + RealtimeSrvMath::GetRandomFloat() * -5000.f,
			0.f ) );
		newCharacter->SetRotation( Vector3(
			0.f,
			RealtimeSrvMath::GetRandomFloat() * 180.f,
			0.f ) );

		return newGameObj;
	}

private:
	RealtimeServer server_;
};





int main( int argc, const char** argv )
{
	RealtimeSrvHelper::SaveCommandLineArg( argc, argv );

	// ��Linux��, ����һ�������в��� 1 ���ɱ�Ϊ�ػ�����,   
	// ��:	   ./rs_example_for_ue4_demo  1
	// ���Ӽ�Ϊǰ̨����
	const uint8_t DaemonizeCmdIndexOnLinux = 1;
	bool willDaemonizeOnLinux = RealtimeSrvHelper::GetCommandLineArg(
		DaemonizeCmdIndexOnLinux ) == "1";

	ExampleSrvForUe4Demo exmaple_server( willDaemonizeOnLinux );
	exmaple_server.Run();

}

