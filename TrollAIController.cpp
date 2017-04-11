// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGProject.h"
#include "TrollAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "RPGMonster.h"
#include "RPGProjectCharacter.h"


ATrollAIController::ATrollAIController(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	BrainComponent = BehaviroTreeCompo = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, "BehaviroTreeCompo");
	BlackboardCompo = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, "BlackboardCompo");
}

void ATrollAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	ARPGMonster* Monster = Cast<ARPGMonster>(InPawn);

	if (Monster && Monster->MonsterBehavior)
	{
		if (Monster->MonsterBehavior->BlackboardAsset)
		{
			BlackboardCompo->InitializeBlackboard(*Monster->MonsterBehavior->BlackboardAsset);
		}

		EnemyKeyID = BlackboardCompo->GetKeyID("Enemy");
		BehaviroTreeCompo->StartTree(*Monster->MonsterBehavior);
	}
}

void ATrollAIController::SetEnemy(APawn * InPawn)
{
	BlackboardCompo->SetValue<UBlackboardKeyType_Object>(EnemyKeyID, InPawn);

	SetFocus(InPawn);
}

void ATrollAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	FVector FocalPoint = GetFocalPoint();

	if (!FocalPoint.IsZero() && GetPawn())
	{
		FVector Direction = FocalPoint - GetPawn()->GetActorLocation();
		FRotator NewControlRotation = Direction.Rotation();
		NewControlRotation.Yaw = FRotator::ClampAxis(NewControlRotation.Yaw);
		SetControlRotation(NewControlRotation);

		APawn* P = GetPawn();

		if (P && bUpdatePawn)
		{
			P->FaceRotation(NewControlRotation, DeltaTime);
		}
	}
}

void ATrollAIController::FindClosestEnemy()
{
	APawn* P = GetPawn();

	if (P == NULL)
	{
		return;
	}

	const FVector Mylocal = P->GetActorLocation();
	float Bestdistsq = 256000.f;
	ARPGProjectCharacter* BestPawn = NULL;

	for (FConstPawnIterator iter = GetWorld()->GetPawnIterator(); iter; ++iter)
	{
		ARPGProjectCharacter* TestPawn = Cast<ARPGProjectCharacter>(*iter);
		if (TestPawn)
		{
			const float Distsq = (TestPawn->GetActorLocation() - Mylocal).SizeSquared();
			if (Distsq < Bestdistsq)
			{
				Bestdistsq = Distsq;
				BestPawn = TestPawn;
			}
		}
	}

	if (BestPawn)
	{
		SetEnemy(BestPawn);
	}
}

void ATrollAIController::AttackEnemy()
{
	ARPGMonster* Monster = Cast<ARPGMonster>(GetPawn());

	ARPGProjectCharacter* Enemy = GetEnemy();

	bool bCanAttack = false;

	if (Enemy)
	{
		const float Dist = (Enemy->GetActorLocation() - Monster->GetActorLocation()).Size2D();

		if (Dist < 300)
		{
			bCanAttack = true;
		}

		if (bCanAttack)
		{
			Monster->StartAttack();
		}
		else
		{
			Monster->StopAttack();
		}
	}
}

class ARPGProjectCharacter* ATrollAIController::GetEnemy() const
{
	if (BlackboardCompo)
	{
		return Cast<ARPGProjectCharacter>(BlackboardCompo->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));
	}

	return NULL;
}