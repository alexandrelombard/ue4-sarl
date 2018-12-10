#pragma once

#include "PerceptionList.generated.h"

USTRUCT(BlueprintType)
struct FPerceptionListData
{
	GENERATED_USTRUCT_BODY()

    /** The ID of agent concerned by the perception */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SARL")
		FString ID = "";

    /** The relative position of the perception (center of mass for the moment) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SARL")
		TArray<FVector> Perceptions;

	FPerceptionListData() {}
};

FORCEINLINE FArchive& operator<<(FArchive &Ar, FPerceptionListData& PerceptionData)
{
#if PLATFORM_LITTLE_ENDIAN
	Ar.SetByteSwapping(true);
#endif

	int32 PerceptionsCount = PerceptionData.Perceptions.Num();	// Useless, but won't compile if PerceptionData.Perceptions.Num() is written directly

	Ar << PerceptionData.ID;
    Ar << PerceptionsCount;

    for(int32 Idx = 0; Idx < PerceptionData.Perceptions.Num(); Idx++)
    {
        Ar << PerceptionData.Perceptions[Idx];
    }

	return Ar;
}