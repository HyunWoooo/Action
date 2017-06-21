// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "RPGProjectCharacter.h"
#include "Item.generated.h"

UCLASS()
class RPGPROJECT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UShapeComponent* TBox;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* SM_TBox;

	ARPGProjectCharacter* PlayerController;

	UPROPERTY(EditAnywhere)
		FString ItemName = FString(TEXT(""));

	void GetPlayer(AActor* Player);

	void PickUp();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	bool bItemIsWithInRange;
	
};
