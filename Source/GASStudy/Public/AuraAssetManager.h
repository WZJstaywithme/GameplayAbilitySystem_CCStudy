// Copyright CC Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

/**
 * this class need to Adding to DefaultEngin.ini in the config folder
 */
UCLASS()
class GASSTUDY_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UAuraAssetManager& Get();
	
protected:
	virtual void StartInitialLoading() override;
};
