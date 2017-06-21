// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGProject.h"
#include "RPGMonster.h"
#include "RPGProjectCharacter.h"
#include "TrollAIController.h"
#include "Item.h"


// Sets default values
ARPGMonster::ARPGMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//MosterCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MosterCollision"));
	//RootComponent = MosterCollision;
	//MosterCollision->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	//MosterCollision->CanCharacterStepUpOn = ECB_No;
	//MosterCollision->bShouldUpdatePhysicsVolume = true;
	//MosterCollision->bCheckAsyncSceneOnMove = false;
	//MosterCollision->SetCanEverAffectNavigation(false);
	//MosterCollision->bDynamicObstacle = true;

	//MosterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MosterMesh"));

	//MosterMesh->AttachTo(RootComponent);

	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Enemy_Troll(TEXT("SkeletalMesh'/Game/InfinityBladeAdversaries/Enemy/Enemy_Troll/SK_Troll_Poison.SK_Troll_Poison'"));
	//MosterMesh->SetSkeletalMesh(SK_Enemy_Troll.Object);

	//ConstructorHelpers::FObjectFinder<UStaticMesh> lWeaponMesh(TEXT("StaticMesh'/Game/InfinityBladeAdversaries/Enemy/Enemy_Troll/Meshes/SM_Troll_hammer.SM_Troll_hammer'"));
	//WeaponMesh = NewObject<UStaticMeshComponent>(this, TEXT("MonsterWeaponMesh"));
	//WeaponMesh->AttachTo(this->GetMesh(), TEXT("b_MF_Weapon_R_Socket"));
	//WeaponMesh->SetStaticMesh(lWeaponMesh.Object);

	//WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
	//WeaponCollision->SetBoxExtent(FVector(30, 40, 15));
	//WeaponCollision->AttachTo(WeaponMesh, "Center");

	AIControllerClass = ATrollAIController::StaticClass();

	bUseControllerRotationYaw = true;

	health = 1000;

}

// Called to bind functionality to input
void ARPGMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARPGMonster::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor->IsA(ARPGProjectCharacter::StaticClass()) && bAttacking && !bAttackComplite)
	{
		UGameplayStatics::ApplyDamage(OtherActor, 30, NULL, this, UDamageType::StaticClass());
		bAttackComplite = true;
	}
}

float ARPGMonster::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (health <= 0)
		return 0.0f;

	const float ActaulDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	StopAttack();

	if (ActaulDamage >= 0)
	{
		health -= ActaulDamage;
	}

	if (health <= 0)
	{
		Die(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
	else
	{
		OnHit(DamageAmount, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : NULL, DamageCauser);
	}

	return ActaulDamage;
}


void ARPGMonster::OnHit(float DamageTaken, FDamageEvent const & DamageEvent, APawn * PawnInstigator, AActor * DamageCauser)
{
	float BeHitAnimDuration = PlayAnimMontage(BeHitAnim);

	if (DamageTaken > 0)
	{
		ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
	}
}

void ARPGMonster::Die(float KillingDamage, FDamageEvent const & DamageEvent, AController * Killer, AActor * DamageCauser)
{
	UDamageType const* const DamageType =
	DamageEvent.DamageTypeClass ? Cast<const UDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject()) :
	GetDefault<UDamageType>();

	Killer = GetDamageInstigator(Killer, *DamageType);

	GetWorldTimerManager().ClearAllTimersForObject(this);

	float DeathAnimDuration = PlayAnimMontage(DeathAnim);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ARPGMonster::OnDieAnimationEnd, DeathAnimDuration, false);

	FActorSpawnParameters SpawnInfo;

	UWorld* const World = GetWorld(); // get a reference to the world
	if (World) {
		// if world exists
		AItem* AttackEppect = World->SpawnActor<AItem>(ItemClass, GetActorLocation(), GetActorRotation(), SpawnInfo);
	}
}

void ARPGMonster::OnDieAnimationEnd()
{
	this->SetActorHiddenInGame(true);
	SetLifeSpan(0.1f);
}

void ARPGMonster::FaceRotation(FRotator NewControlRotation, float DeltaTime)
{
	FRotator CurrentRotation = FMath::RInterpTo(GetActorRotation(), NewControlRotation, DeltaTime, 8.0f);

	Super::FaceRotation(CurrentRotation, DeltaTime);
}

void ARPGMonster::StartAttack()
{
	if (!bAttacking && health > 0)
	{
		bAttacking = true;
		float during = PlayAnimMontage(AttackAnim);
		GetWorldTimerManager().SetTimer(TimerHandle_EnemyStopAttack, this, &ARPGMonster::StopAttack, during, false);
	}
}

void ARPGMonster::StopAttack()
{
	if (bAttacking)
	{
		StopAnimMontage(AttackAnim);
		bAttacking = false;
		bAttackComplite = false;
	}
	
}

float ARPGMonster::Getcurrenthealth()
{
	return health;
}