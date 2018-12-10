#pragma once

#include "Perception.generated.h"

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
	Ar << PerceptionData.ID;
    Ar << Perceptions.Num();

    for(int32 Idx = 0; Idx < Perceptions.Num(); Idx++)
    {
        Ar << Perceptions[Idx];
    }

	return Ar;
}