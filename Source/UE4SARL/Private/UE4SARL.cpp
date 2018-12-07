// Copyright (c) 2018 Isara Technologies. All Rights Reserved.

#include "IUE4SARL.h"

class FUE4SARL : public IUE4SARL
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE( FUE4SARL, UE4SARL )

void FUE4SARL::StartupModule()
{
	//
}


void FUE4SARL::ShutdownModule()
{
	//
}