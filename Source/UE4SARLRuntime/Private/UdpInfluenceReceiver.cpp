#include "../Public/UdpInfluenceReceiver.h"

AUdpInfluenceReceiver::AUdpInfluenceReceiver(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ListenSocket = NULL;
}

void AUdpInfluenceReceiver::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//~~~~~~~~~~~~~~~~

	delete UdpReceiver;
	UdpReceiver = nullptr;

	//Clear all sockets!
	//		makes sure repeat plays in Editor dont hold on to old sockets!
	if (ListenSocket)
	{
		ListenSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenSocket);
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//Rama's Start TCP Receiver
bool AUdpInfluenceReceiver::StartUdpReceiver(
	const FString& SocketName,
	const FString& IpAddress,
	const int32 Port) {
	FIPv4Address Addr;
	FIPv4Address::Parse(IpAddress, Addr);

	//Create Socket
	FIPv4Endpoint Endpoint(Addr, Port);

	//BUFFER SIZE
	int32 BufferSize = 2 * 1024 * 1024;

	ListenSocket = FUdpSocketBuilder(*SocketName)
		.AsNonBlocking()
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.WithReceiveBufferSize(BufferSize);
	;

	FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(100);
	UdpReceiver = new FUdpSocketReceiver(ListenSocket, ThreadWaitTime, TEXT("UDP RECEIVER"));
	UdpReceiver->OnDataReceived().BindUObject(this, &AUdpInfluenceReceiver::Recv);

	return true;
}

void AUdpInfluenceReceiver::Recv(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt)
{
	FInfluenceData Data;
	*ArrayReaderPtr << Data;
	BPEvent_DataReceived(Data);
}