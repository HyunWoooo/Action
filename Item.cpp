// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGProject.h"
#include "Item.h"
#include "Engine.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	TBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));

	RootComponent = TBox;

	SM_TBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box Mesh"));
	SM_TBox->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (PlayerController != NULL)
	{
		if (PlayerController->bIsPickingUp && bItemIsWithInRange)
		{
			PickUp();
		}
	}
}

void AItem::GetPlayer(AActor * Player)
{
	PlayerController = Cast<ARPGProjectCharacter>(Player);
}

void AItem::PickUp()
{
	PlayerController->Inventory.Add(*ItemName);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("You Picking Item")));
	Destroy();
}

void AItem::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor->IsA(ARPGProjectCharacter::StaticClass()))
	{
		bItemIsWithInRange = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Press E to picking %s"), *ItemName));

		GetPlayer(OtherActor);
	}
}

void AItem::NotifyActorEndOverlap(AActor * OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (OtherActor->IsA(ARPGProjectCharacter::StaticClass()))
	{
		bItemIsWithInRange = false;
	}
}

