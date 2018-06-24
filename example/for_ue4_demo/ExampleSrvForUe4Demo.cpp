#include <realtime_srv/RealtimeServer.h>
#include "Character.h"





class ExampleSrvForUe4Demo
{
public:
	ExampleSrvForUe4Demo()
	{
		// ��Linux�ϼ���һ�������в��� 1 ���ɱ�Ϊ�ػ�����,   
		// ��:	   ./rts_example_for_ue4_demo  1
		const uint8_t becomeDaemonCmdIndexOnLinux = 1;

		//	��Windows��, �����������:   ./example_for_ue4_demo.exe 0 0.3 0.8 1
		//	�����������ģ�� : 
		//	-	ģ���ӳ�Ϊ : ��ǰ�ӳ�+300������ӳ�
		//	-	ģ�ⶪ����Ϊ : �ٷ�֮��ʮ�Ķ�����
		//	-	ģ�����綶��Ϊ : 1Ϊ�������, 0Ϊ������
		const uint8_t SimulateLatencyCmdIndexOnWindows = 2;
		const uint8_t SimulateDropPacketChanceCmdIndexOnWindows = 3;
		const uint8_t SimulateJitterCmdIndexOnWindows = 4;

		bool whetherTobecomeDaemon = RealtimeSrvHelper::GetCommandLineArg(
			becomeDaemonCmdIndexOnLinux ) == "1";

		NewPlayerCallback newPlayerCb = std::bind(
			&ExampleSrvForUe4Demo::SpawnNewCharacterForPlayer,
			this, _1 );

		server_.Init( newPlayerCb, whetherTobecomeDaemon );
		server_.SimulateRealWorldOnWindows(
			SimulateLatencyCmdIndexOnWindows,
			SimulateDropPacketChanceCmdIndexOnWindows,
			SimulateJitterCmdIndexOnWindows );
	}

	void Run()
	{
		server_.Run();
	}

	GameObjPtr SpawnNewCharacterForPlayer( ClientProxyPtr newClientProxy )
	{
		GameObjPtr newGameObj = Character::StaticCreate();
		CharacterPtr character = std::static_pointer_cast< Character >( newGameObj );

		character->SetLocation( Vector3(
			2500.f + RealtimeSrvMath::GetRandomFloat() * -5000.f,
			2500.f + RealtimeSrvMath::GetRandomFloat() * -5000.f,
			0.f ) );
		character->SetRotation( Vector3(
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

	ExampleSrvForUe4Demo exmaple_server;
	exmaple_server.Run();
}