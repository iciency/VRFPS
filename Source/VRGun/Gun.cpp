// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "BaseCharacter.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AGun::Fire()
{


	if (GetNetMode() != ENetMode::NM_Standalone) {
		APawn* Pawn = Cast<APawn>(GetOwner());
		if (Pawn) {
			if (Pawn->IsLocallyControlled()) {
				if (Pawn->HasAuthority()) {
					MulticastFire(true);
				}
				else {
					ServerFire();
				}
			}
		}
		else {

		}
	}

	OnFire();

	if (HasAuthority()) {
		// 실제 발사할 시작 위치
		FVector StartFireLocation; // Line Trace 시작 위치
		FVector EndFireLocation; // Line Trace 끝 위치
		FVector FireDirection;

		if (GetOwner() != nullptr)
		{

			FVector EyeLocation; // 현재 Eye(Actor Location + APawn::BaseEyeHeight) 위치
			FRotator EyeRotation; // 현재 카메라가 보고있는 방향

			if (!Cast<ABaseCharacter>(GetOwner())->UseVR) {
				UE_LOG(LogTemp, Log, TEXT("useVr false"));
			}
			

			if (Cast<ABaseCharacter>(GetOwner())->UseVR){
				UE_LOG(LogTemp, Log, TEXT("useVr true"));
				StartFireLocation = GetActorLocation();
				EndFireLocation = (GetActorRightVector() * 10000) + GetActorLocation();
				FireDirection = GetActorRightVector();
			}
			else {
				GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRotation);

				StartFireLocation = EyeLocation;

				FireDirection = EyeRotation.Vector();
				EndFireLocation = StartFireLocation + FireDirection * 10000;
			}
		}

		FHitResult Hit;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this); // 발사하는 Gun 엑터 제외
		QueryParams.AddIgnoredActor(GetOwner()); // 발사하는 Gun 엑터를 가지고있는 캐릭터도 제외
		if (GetWorld()->LineTraceSingleByObjectType(Hit, StartFireLocation, EndFireLocation, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllObjects), QueryParams))
		{
#if WITH_EDITOR
			DrawDebugLine(GetWorld(), StartFireLocation, EndFireLocation, FColor::Red, false, 10.f, SDPG_World, 1.f);
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.f, 12, FColor::Green, false, 10.f, SDPG_World, 1.f);
#endif

			UE_LOG(LogTemp, Log, TEXT("some one hit %s"), *Hit.Actor->GetName());

			ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Hit.Actor);
			if (BaseCharacter != nullptr)
			{
				FPointDamageEvent DamageEvent;
				DamageEvent.DamageTypeClass = nullptr;
				DamageEvent.Damage = 10;
				DamageEvent.HitInfo = Hit;
				DamageEvent.ShotDirection = FireDirection;

				BaseCharacter->TakeDamage(10, DamageEvent, nullptr, GetOwner());

			}
			else
			{
				FPointDamageEvent DamageEvent;
				DamageEvent.DamageTypeClass = nullptr;
				DamageEvent.Damage = 0;
				DamageEvent.HitInfo = Hit;
				DamageEvent.ShotDirection = FireDirection;

				Hit.Actor->TakeDamage(0, DamageEvent, nullptr, GetOwner());
			}
		}
		else
		{
#if WITH_EDITOR
			DrawDebugLine(GetWorld(), StartFireLocation, EndFireLocation, FColor::Blue, false, 10.f, SDPG_World, 1.f);
#endif
		}
	}
}

void AGun::ServerFire_Implementation()
{
	Fire();
	UE_LOG(LogTemp, Log, TEXT("server fire"));
	MulticastFire(false);
}

void AGun::MulticastFire_Implementation(bool NeedIncludeAutonomousProxy)
{
	UE_LOG(LogTemp, Log, TEXT("multicastFire fire"));

	if (HasAuthority() == false)
	{
		if (NeedIncludeAutonomousProxy)
		{
			Fire();
		}
		else
		{
			if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy)
			{
				Fire();
			}
		}
	}
}

FVector AGun::GetMuzzleLocation_Implementation() const
{
	return GetActorLocation();
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

