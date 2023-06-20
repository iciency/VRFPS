// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Gun.h"
#include "BaseCharacter.generated.h"

UCLASS()
class VRGUN_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> EquipGunType;

	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Die();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDie();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
		void OnRep_IsDead();

	UPROPERTY(BlueprintReadWrite)
		bool UseVR;
protected:

	void Turn(float AxisValue);

	void LookUp(float AxisValue);

	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	void PressedFire();

	void ReleasedFire();

	UFUNCTION()
	virtual void OnRep_WeaponActor();

	/** [Client + Server] 총기를 셋업하는 함수 */
	virtual void SetupWeapon();

	UFUNCTION(BlueprintImplementableEvent)
		void OnSetupWeapon();


protected:
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_WeaponActor)
	AActor* WeaponActor;

	/** 최대 체력 */
	UPROPERTY(EditDefaultsOnly)
		int MaxHealth;

	/** 현재 체력*/
	UPROPERTY(BlueprintReadWrite)
		int CurrentHealth;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_IsDead)
		bool IsDead;

};