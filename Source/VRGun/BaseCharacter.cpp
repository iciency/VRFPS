// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MaxHealth = 50;
	CurrentHealth = MaxHealth;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	WeaponActor = GetWorld()->SpawnActor(EquipGunType, &FTransform::Identity, SpawnParameters);

	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ABaseCharacter::OnRep_WeaponActor()
{
	SetupWeapon();
}

void ABaseCharacter::SetupWeapon()
{
	OnSetupWeapon();
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABaseCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABaseCharacter::LookUp);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ABaseCharacter::MoveRight);

	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &ABaseCharacter::PressedFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Released, this, &ABaseCharacter::ReleasedFire);
}
void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, WeaponActor);
	DOREPLIFETIME(ABaseCharacter, IsDead);
}

float ABaseCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth -= DamageAmount;

	UE_LOG(LogTemp, Log, TEXT("take damage %d"), CurrentHealth);
	if (CurrentHealth <= 0)
	{
		if (!IsDead)
		{
			IsDead = true;
			Die();
		}
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
void ABaseCharacter::MoveForward(float AxisValue)
{
	FRotator ControlRotation = FRotator(0, GetControlRotation().Yaw, 0);
	FVector WorldDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(WorldDirection, AxisValue);
}

void ABaseCharacter::MoveRight(float AxisValue)
{
	FRotator ControlRotation = FRotator(0, GetControlRotation().Yaw, 0);
	FVector WorldDirection = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(WorldDirection, AxisValue);
}

void ABaseCharacter::Turn(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void ABaseCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void ABaseCharacter::PressedFire()
{
	if (IsDead)
		return;

	AGun* Gun = Cast<AGun>(WeaponActor);
	if (Gun)
	{
		Gun->Fire();
	}	
}

void ABaseCharacter::ReleasedFire()
{
}

void ABaseCharacter::OnRep_IsDead()
{
	Die();
}

void ABaseCharacter::Die()
{
	UE_LOG(LogTemp, Log, TEXT("´©±º°¡ Á×À½!"));
	OnDie();
	//Destroy();
}



