// Fill out your copyright notice in the Description page of Project Settings.


#include "VRGunPlayerState.h"
#include "Net/UnrealNetwork.h"

AVRGunPlayerState::AVRGunPlayerState()
{
	TeamId = 0;
}

void AVRGunPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVRGunPlayerState, TeamId);
}
void AVRGunPlayerState::SetTeam(int32 InTeamId)
{
	if (HasAuthority())
	{
		TeamId = InTeamId;

		OnTeamChanged();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetTeam can call only on server!"));
	}
}

void AVRGunPlayerState::ServerSetTeam_Implementation(int32 InTeamId)
{
	SetTeam(InTeamId);
}


FString AVRGunPlayerState::GetUniqueIdString() const
{
	return UniqueId.ToString();
}

void AVRGunPlayerState::OnRep_TeamId()
{
	OnTeamChanged();
}
