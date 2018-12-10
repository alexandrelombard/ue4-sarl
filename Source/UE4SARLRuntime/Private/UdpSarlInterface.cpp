#include "../Public/UdpSarlInterface.h"

AUdpSarlInterface::AUdpSarlInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ListenSocket = NULL;
	SendSocket = NULL;
}

void AUdpSarlInterface::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	delete UdpReceiver;
	UdpReceiver = nullptr;

	//Clear all sockets
	if (ListenSocket)
	{
		ListenSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenSocket);
	}

	if (SendSocket)
	{
		SendSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(SendSocket);
	}
}

// Initialize UDP emitter
bool AUdpSarlInterface::StartUdpEmitter(
	const FString& SocketName,
	const FString& IpAddress,
	const int32 Port) {
	FIPv4Address Addr;
	FIPv4Address::Parse(IpAddress, Addr);

	//Create Socket
	FIPv4Endpoint Endpoint(Addr, Port);

	//BUFFER SIZE
	int32 BufferSize = 2 * 1024 * 1024;

	SendSocket = FUdpSocketBuilder(*SocketName)
		.AsNonBlocking()
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.WithReceiveBufferSize(BufferSize);

	return true;
}

// Start UDP Receiver
bool AUdpSarlInterface::StartUdpReceiver(
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

	FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(100);
	UdpReceiver = new FUdpSocketReceiver(ListenSocket, ThreadWaitTime, TEXT("UDP RECEIVER"));
	UdpReceiver->OnDataReceived().BindUObject(this, &AUdpSarlInterface::Recv);

	// Everything is set, let's start
	UdpReceiver->Start();

	return true;
}

bool AUdpSarlInterface::EmitPerceptions(
	FPerceptionListData& Perceptions)
{
	FArrayWriter Writer;
	Writer << Perceptions;								// Serializing the perception list
	int32 ReadBytes = 0;
	SendSocket->Send(Writer.GetData(), Writer.Num(), ReadBytes);	// Sending them

	return ReadBytes == Writer.Num();
}

void AUdpSarlInterface::Recv(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt)
{
	FInfluenceData Data;

#if PLATFORM_LITTLE_ENDIAN
	ArrayReaderPtr->SetByteSwapping(true);
#endif

	*ArrayReaderPtr << Data.ID;
	*ArrayReaderPtr << Data.Influence;

	//*ArrayReaderPtr << Data;

	AsyncTask(ENamedThreads::GameThread, [Data, this]() {
		// Calling in the GameThread, so influences can be applied
		BPEvent_DataReceived(Data);
	});
}