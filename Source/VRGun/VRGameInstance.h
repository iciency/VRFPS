// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "VRGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class VRGUN_API UVRGameInstance : public UGameInstance
{
	GENERATED_BODY()


	

	public:
		UPROPERTY(BlueprintReadWrite)
			bool UseVR;
		UFUNCTION(BlueprintCallable)
			bool OnGetUseVR();
};
