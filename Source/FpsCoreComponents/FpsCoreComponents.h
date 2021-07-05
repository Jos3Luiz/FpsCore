// YourModuleNameModule.h
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FFpsCoreComponents : public IModuleInterface
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};