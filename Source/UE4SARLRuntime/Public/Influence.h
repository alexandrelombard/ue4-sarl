#pragma once

#include "Influence.generated.h"

USTRUCT(BlueprintType)
struct FInfluenceData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SARL")
		FString ID = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SARL")
		FVector Influence;
};

FORCEINLINE FArchive& operator<<(FArchive &Ar, FInfluenceData& InfluenceData)
{
#if PLATFORM_LITTLE_ENDIAN
	Ar.SetByteSwapping(true);
#endif

	Ar << InfluenceData.ID;
	Ar << InfluenceData.Influence;

	return Ar;
}