// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class VRGUN_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	virtual void Fire();

	/* 총알이 발사될 위치 반환 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetMuzzleLocation() const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 다른 클라에게 동기화
	UFUNCTION(Server, Reliable)
		void ServerFire();


	UFUNCTION(NetMulticast, Reliable)
		void MulticastFire(bool NeedIncludeAutonomousProxy);

	UFUNCTION(BlueprintImplementableEvent)
		void OnFire();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
