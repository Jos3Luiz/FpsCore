// YourModuleNameModule.h
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FFpsCoreInteractables : public IModuleInterface
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};