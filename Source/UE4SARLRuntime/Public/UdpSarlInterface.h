#pragma once

//Networking
#include "Networking.h"

#include "Influence.h"
#include "PerceptionList.h"

#include "UdpSarlInterface.generated.h"

UCLASS(BlueprintType)
class AUdpSarlInterface : public AActor
{
	GENERATED_UCLASS_BODY()
public:
	/** Data has been received */
	UFUNCTION(BlueprintImplementableEvent)
		void BPEvent_DataReceived(FInfluenceData ReceivedData);

public:
	FSocket * ListenSocket;
	FSocket * SendSocket;

	FUdpSocketReceiver* UdpReceiver = nullptr;

	void Recv(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt);
	
	/** Initialize the UDP emitter */
	UFUNCTION(BlueprintCallable, Category = "SARL")
		bool StartUdpEmitter(
			const FString& SocketName,
			const FString& IpAddress,
			const int32 Port);

	/** 
	 * Emit perceptions
	 * @param PerceiverID the ID of the perceiver
	 * @param BodyTransform the transform of the perceiver
	 * @param PerceivedActors the list of perceived actors
	 */
	UFUNCTION(BlueprintCallable, Category = "SARL")
		bool EmitPerceptions(const FString& PerceiverID, const FTransform& BodyTransform, const TArray<AActor*>& PerceivedActors);

	UFUNCTION(BlueprintCallable, Category = "SARL")
		bool EmitPerceptionList(FPerceptionListData Perceptions);

	/** Initialize the UDP receiver */
	UFUNCTION(BlueprintCallable, Category = "SARL")
		bool StartUdpReceiver(
			const FString& SocketName,
			const FString& IpAddress,
			const int32 Port);

public:

	/** Called whenever this actor is being removed from a level */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	TSharedPtr<FInternetAddr> RemoteAddr;

	TMap<FString, bool> PendingInfluences;
};