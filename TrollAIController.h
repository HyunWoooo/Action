// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "TrollAIController.generated.h"

/**
 * 
 */

class UBehaviorTreeComponent;
class UBlackboardComponent;

UCLASS(config = Game)
class RPGPROJECT_API ATrollAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	ATrollAIController(const class FObjectInitializer& ObjectInitializer);

	virtual void Possess(APawn* InPawn) override;

	void SetEnemy(class APawn* InPawn);

	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn = true) override;

	UFUNCTION(BlueprintCallable, Category = Behavior)
	void FindClosestEnemy();

	UFUNCTION(BlueprintCallable, Category = Behavior)
	void AttackEnemy();


	class ARPGProjectCharacter* GetEnemy() const;

private:

	UPROPERTY(transient)
		UBehaviorTreeComponent* BehaviroTreeCompo;

	UPROPERTY(transient)
		UBlackboardComponent* BlackboardCompo;

protected:

	int32 EnemyKeyID;

	
	
};
