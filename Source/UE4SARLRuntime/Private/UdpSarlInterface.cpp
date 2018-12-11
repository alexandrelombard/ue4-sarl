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
	//Create Remote Address.
	RemoteAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	bool bIsValid;
	RemoteAddr->SetIp(*IpAddress, bIsValid);
	RemoteAddr->SetPort(Port);

	if(bIsValid)
	{
		const int32 SendSize = 2 * 1024 * 1024;

		SendSocket = FUdpSocketBuilder(*SocketName)
			.AsReusable()
//			.WithBroadcast()
			.WithSendBufferSize(SendSize);
	}

	return SendSocket != nullptr;
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

	if(ListenSocket != nullptr)
	{
		FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(100);
		UdpReceiver = new FUdpSocketReceiver(ListenSocket, ThreadWaitTime, TEXT("UDP RECEIVER"));
		UdpReceiver->OnDataReceived().BindUObject(this, &AUdpSarlInterface::Recv);

		// Everything is set, let's start
		UdpReceiver->Start();
	}

	return ListenSocket != nullptr;
}

bool AUdpSarlInterface::EmitPerceptions(
	FPerceptionListData Perceptions)
{
	FBufferArchive Writer;

#if PLATFORM_LITTLE_ENDIAN
	Writer.SetByteSwapping(true);
#endif

	int32 PerceptionsCount = Perceptions.Perceptions.Num();	// Useless, but won't compile if PerceptionData.Perceptions.Num() is written directly

	Writer << Perceptions.ID;
	Writer << PerceptionsCount;

	for (int32 Idx = 0; Idx < Perceptions.Perceptions.Num(); Idx++)
	{
		Writer << Perceptions.Perceptions[Idx];
	}

	//Writer << Perceptions;								// Serializing the perception list
	int32 SentBytes = 0;
	SendSocket->SendTo(Writer.GetData(), Writer.Num(), SentBytes, *RemoteAddr);	// Sending them

	return SentBytes == Writer.Num();
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

	if (!PendingInfluences.Contains(Data.ID))
		PendingInfluences.Add(Data.ID, false);

	// If there is a pending influence for the given ID, we won't accept more influences
	if(!PendingInfluences[Data.ID])
	{
		PendingInfluences[Data.ID] = true;

		AsyncTask(ENamedThreads::GameThread, [Data, this]() {
			// Calling in the GameThread, so influences can be applied
			BPEvent_DataReceived(Data);

			PendingInfluences[Data.ID] = false;
		});
	}
}