// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "VRGunPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class VRGUN_API AVRGunPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AVRGunPlayerState();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
		void SetTeam(int32 InTeamId);

	UFUNCTION(Reliable, Server, BlueprintCallable)
		void ServerSetTeam(int32 InTeamId);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
		FString GetUniqueIdString() const;

protected:
	UFUNCTION()
		void OnRep_TeamId();

	UFUNCTION(BlueprintImplementableEvent)
		void OnTeamChanged();
	UPROPERTY(ReplicatedUsing = OnRep_TeamId, BlueprintReadOnly)
		int TeamId;
		
};
