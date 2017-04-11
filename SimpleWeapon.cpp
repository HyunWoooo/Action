// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGProject.h"
#include "SimpleWeapon.h"
#include "RPGMonster.h"
#include "RPGProjectCharacter.h"
#include "SimpleEffect.h"

// Sets default values
ASimpleWeapon::ASimpleWeapon(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, "WeaponMesh");
	WeaponMesh->CastShadow = true;
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = WeaponMesh;
	WeaponCollision = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, "WeaponCollision");
}


void ASimpleWeapon::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor->IsA(ARPGMonster::StaticClass()) && (bDuringAttack && !bAttackComplite))
	{
		UGameplayStatics::ApplyDamage(OtherActor, 50, NULL, this, UDamageType::StaticClass());

		FActorSpawnParameters SpawnInfo;

		UWorld* const World = GetWorld(); // get a reference to the world
		if (World) {
			// if world exists
			ASimpleEffect* AttackEppect = World->SpawnActor<ASimpleEffect>(AttackEppectClass, GetActorLocation(), GetActorRotation(), SpawnInfo);
		}
		bAttackComplite = true;
	}
}


void ASimpleWeapon::SetOnwingPawn(APublicCharacter * NewOwner)
{
	if (MyPawn != NewOwner)
	{
		MyPawn = NewOwner;
	}
}

void ASimpleWeapon::AttachMeshtoPawn()
{
	if (MyPawn)
	{
		USkeletalMeshComponent* PawnMesh = MyPawn->GetSpecificPawnMesh();
		FName AttackPoint = MyPawn->GetWeaponAttachPoint();
		WeaponMesh->AttachTo(PawnMesh, AttackPoint);
	}
}

void ASimpleWeapon::StartAttack()
{
	ARPGProjectCharacter* Player = Cast<ARPGProjectCharacter>(MyPawn);

	if (Player->Currentstamina - 30.f <= 0)
		return;

	if (bDuringAttack)
	{
		bSaveAttack = true;
		return;
	}

		
	Player->Currentstamina -= 30.f;
	

	if (Player->bSprint)
	{
		float AnimDuring = MyPawn->PlayAnimMontage(SprintAttackAnim);
		bDuringAttack = true;

		FTimerHandle TimerHandle_StopAttack;
		GetWorldTimerManager().SetTimer(TimerHandle_StopAttack, this, &ASimpleWeapon::StopAttack, AnimDuring, false);
	}
	else
	{
		float AnimDuring = MyPawn->PlayAnimMontage(AttackAnim);
		bDuringAttack = true;

		FTimerHandle TimerHandle_StopAttack;
		GetWorldTimerManager().SetTimer(TimerHandle_StopAttack, this, &ASimpleWeapon::StopAttack, AnimDuring, false);
	}
}

void ASimpleWeapon::StopAttack()
{
	ARPGProjectCharacter* Player = Cast<ARPGProjectCharacter>(MyPawn);

	if (bSaveAttack)
	{
		bSaveAttack = false;

		if(Player->Currentstamina - 30.f >= 0)
		{
			bAttackComplite = false;
			ComboAttack();
			Player->Currentstamina -= 30.f;
		}
		else
		{
			StopAttack();
		}
	}
	else
	{
		MyPawn->StopAnimMontage(SprintAttackAnim);
		MyPawn->StopAnimMontage(AttackAnim);
		MyPawn->StopAnimMontage(Attack2Anim);
		bDuringAttack = false;
		bAttackComplite = false;
		bSaveAttack = false;
		iCombo = 0;
	}
}

void ASimpleWeapon::ComboAttack()
{
	float AnimDuring;
	FTimerHandle TimerHandle_StopAttack;

	switch (iCombo)
	{
	case 0:
		iCombo++;

		AnimDuring = MyPawn->PlayAnimMontage(Attack2Anim);

		GetWorldTimerManager().SetTimer(TimerHandle_StopAttack, this, &ASimpleWeapon::StopAttack, AnimDuring, false);
		break;
	case 1:
		iCombo = 0;

		AnimDuring = MyPawn->PlayAnimMontage(AttackAnim);

		GetWorldTimerManager().SetTimer(TimerHandle_StopAttack, this, &ASimpleWeapon::StopAttack, AnimDuring, false);
		break;
	}
}