#pragma once

//Networking
#include "Networking.h"

#include "UdpInfluenceReceiver.generated.h"

USTRUCT(BlueprintType)
struct FInfluenceData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SARL")
		FString ID = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SARL")
		FVector Influence;

	FInfluenceData() {}
};

FORCEINLINE FArchive& operator<<(FArchive &Ar, FInfluenceData& InfluenceData)
{
	Ar << InfluenceData.ID;
	Ar << InfluenceData.Influence;

	return Ar;
}

UCLASS(BlueprintType)
class AUdpInfluenceReceiver : public AActor
{
	GENERATED_UCLASS_BODY()
public:
	/** Data has been received */
	UFUNCTION(BlueprintImplementableEvent)
		void BPEvent_DataReceived(const FInfluenceData& ReceivedData);

public:
	FSocket * ListenSocket;

	FUdpSocketReceiver* UdpReceiver = nullptr;

	void Recv(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt);

	UFUNCTION(BlueprintCallable, Category = "SARL")
		bool StartUdpReceiver(
			const FString& SocketName,
			const FString& IpAddress,
			const int32 Port);

public:

	/** Called whenever this actor is being removed from a level */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};