// Copyright (C) 2025 nulled.softworks. All rights reserved.

#pragma once

#include "Modules/ModuleManager.h"

/**
 * Ns Backpack runtime module
 */
class FNsBackpackModule : public IModuleInterface
{

// Functions
public:

    //~ Begin IModuleInterface Interface
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    //~ End IModuleInterface Interface
};
