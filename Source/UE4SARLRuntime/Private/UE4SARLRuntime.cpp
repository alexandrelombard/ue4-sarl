// Copyright (c) 2018 Isara Technologies. All Rights Reserved.

#include "IUE4SARLRuntime.h"

class FUE4SARLRuntime : public IUE4SARLRuntime
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE( FUE4SARLRuntime, UE4SARLRuntime)

void FUE4SARLRuntime::StartupModule()
{
	//
}

void FUE4SARLRuntime::ShutdownModule()
{
	//
}