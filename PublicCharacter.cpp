// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGProject.h"
#include "PublicCharacter.h"
#include "SimpleWeapon.h"


// Sets default values
APublicCharacter::APublicCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	fHealth = 100.f;
}

USkeletalMeshComponent * APublicCharacter::GetSpecificPawnMesh() const
{
	return GetMesh();
}

FName APublicCharacter::GetWeaponAttachPoint() const
{
	return WeaponAttachPoint;
}

void APublicCharacter::SetWeapon()
{
	FActorSpawnParameters SpawnInfo;
	//GetWorld()->SpawnActor<UBoxComponent>(GetClass(), SpawnInfo);
	UWorld* const World = GetWorld(); // get a reference to the world
	if (World) {
		// if world exists
		ASimpleWeapon* NewWeapon = World->SpawnActor<ASimpleWeapon>(WeaponClass, SpawnInfo);
		Weapon = NewWeapon;
	}
	Weapon->SetOnwingPawn(this);
	Weapon->AttachMeshtoPawn();
}

void APublicCharacter::WeaponAttack()
{
	if (Weapon)
	{
		Weapon->StartAttack();
	}
}

void APublicCharacter::OnHit(float DamageTaken, FDamageEvent const & DamageEvent, APawn * PawnInstigator, AActor * DamageCauser)
{
	PlayAnimMontage(BeHitAnim);

	if (DamageTaken > 0)
	{
		ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
	}
}

void APublicCharacter::Die(float KillingDamage, FDamageEvent const & DamageEvent, AController * Killer, AActor * DamageCauser)
{
	UDamageType const* const DamageType =
		DamageEvent.DamageTypeClass ? Cast<const UDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject()) :
		GetDefault<UDamageType>();

	Killer = GetDamageInstigator(Killer, *DamageType);

	GetWorldTimerManager().ClearAllTimersForObject(this);

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_Pawn, ECR_Ignore);
		GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	}

	if (GetMovementComponent())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
	}

	if (Controller != NULL)
	{
		Controller->UnPossess();
	}

	float DeathAnimDuration = PlayAnimMontage(DeathAnim);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APublicCharacter::OnDieAnimationEnd, DeathAnimDuration, false);
}

void APublicCharacter::OnDieAnimationEnd()
{
	this->SetActorHiddenInGame(true);
	SetLifeSpan(0.1f);
}

float APublicCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	//if (fHealth <= 0)
	//	return 0.0f;

	const float ActaulDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//if (ActaulDamage >= 0)
	//{
	//	fHealth -= ActaulDamage;
	//}

	//if (fHealth <= 0)
	//{
	//	Die(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	//}
	//else
	//{
	//	OnHit(DamageAmount, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : NULL, DamageCauser);
	//}

	return ActaulDamage;
}

float APublicCharacter::Getcurrenthealth()
{
	return fHealth;
}
