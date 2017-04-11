// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGProject.h"
#include "SimpleEffect.h"


// Sets default values
ASimpleEffect::ASimpleEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void ASimpleEffect::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	fTick += DeltaTime;

	if (fTick >= fDelay)
		Destroy();
}

