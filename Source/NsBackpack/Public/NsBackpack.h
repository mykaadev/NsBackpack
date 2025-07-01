// Copyright (c) 2025 mykaa

#pragma once

#include "Modules/ModuleManager.h"

class FNsBackpackModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
