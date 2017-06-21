// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "RPGProject.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "RPGProjectCharacter.h"
#include "RPGMonster.h"
#include "SimpleEffect.h"
#include "Engine.h"

//////////////////////////////////////////////////////////////////////////
// ARPGProjectCharacter

ARPGProjectCharacter::ARPGProjectCharacter()
{
	Initailstamina = 100;
	Currentstamina = Initailstamina;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm

	//ConstructorHelpers::FObjectFinder<UStaticMesh> lWeaponMesh(TEXT("StaticMesh'/Game/Meshes/items/S_Sword_Basic.S_Sword_Basic'"));
	//WeaponMesh = NewObject<UStaticMeshComponent>(this, TEXT("CharacterWeaponMesh"));
	//WeaponMesh->AttachTo(this->GetMesh(), TEXT("Weapon"));
	//WeaponMesh->SetStaticMesh(lWeaponMesh.Object);

	//ConstructorHelpers::FObjectFinder<UStaticMesh> lShieldMesh(TEXT("StaticMesh'/Game/Meshes/items/SM_Shield_Buckler_Wood.SM_Shield_Buckler_Wood'"));
	//ShieldMesh = NewObject<UStaticMeshComponent>(this, TEXT("CharacterShieldMesh"));
	//ShieldMesh->AttachTo(this->GetMesh(), TEXT("Shield"));
	//ShieldMesh->SetStaticMesh(lShieldMesh.Object);

	//WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
	//WeaponCollision->SetBoxExtent(FVector(30, 5, 2));
	//WeaponCollision->AttachTo(WeaponMesh, "Center");
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARPGProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARPGProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARPGProjectCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ARPGProjectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ARPGProjectCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ARPGProjectCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ARPGProjectCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ARPGProjectCharacter::OnResetVR);

	// "Attack" 바인딩을 구성합니다.
	InputComponent->BindAction("Attack", IE_Pressed, this, &APublicCharacter::WeaponAttack);

	// "Sprint" 바인딩을 구성합니다.
	InputComponent->BindAction("Sprint", IE_Pressed, this, &ARPGProjectCharacter::StartSprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &ARPGProjectCharacter::StopSprint);

	// "Block" 바인딩을 구성합니다.
	InputComponent->BindAction("Block", IE_Pressed, this, &ARPGProjectCharacter::StartBlock);
	InputComponent->BindAction("Block", IE_Released, this, &ARPGProjectCharacter::StopBlock);

	InputComponent->BindAction("PickUp", IE_Pressed, this, &ARPGProjectCharacter::BeginPickUp);
	InputComponent->BindAction("PickUp", IE_Released, this, &ARPGProjectCharacter::EndPickUp);

	InputComponent->BindAction("Inventory", IE_Pressed, this, &ARPGProjectCharacter::ShowInventory);
}

void ARPGProjectCharacter::Tick(const float deltaTime)
{
	Super::Tick(deltaTime);

	if (Currentstamina >= Initailstamina)
		Currentstamina = 100;
	else
		Currentstamina += deltaTime * 20;

	if(bSprint)
		Currentstamina -= deltaTime * 30;
}



void ARPGProjectCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetWeapon();
}


void ARPGProjectCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ARPGProjectCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ARPGProjectCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ARPGProjectCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARPGProjectCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARPGProjectCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		if (bSprint)
			AddMovementInput(Direction, Value);
		else
			AddMovementInput(Direction, Value/2);
	}
}

void ARPGProjectCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		if (bSprint)
			AddMovementInput(Direction, Value);
		else
			AddMovementInput(Direction, Value / 2);
	}
}

void ARPGProjectCharacter::WeaponAttack()
{
	Super::WeaponAttack();
}


void ARPGProjectCharacter::StartSprint()
{
	if (bBlock)
		return;
	bSprint = true;
}

void ARPGProjectCharacter::StopSprint()
{
	bSprint = false;
}

void ARPGProjectCharacter::StartBlock()
{
	bBlock = true;
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;
}

void ARPGProjectCharacter::StopBlock()
{
	bBlock = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void ARPGProjectCharacter::BeginPickUp()
{
	bIsPickingUp = true;
}

void ARPGProjectCharacter::EndPickUp()
{
	bIsPickingUp = false;
}

void ARPGProjectCharacter::ShowInventory()
{
	for (auto& item : Inventory)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Item: %s"), *item));
	}
}

bool ARPGProjectCharacter::IsSprint()
{
	return bSprint;
}

bool ARPGProjectCharacter::IsBlock()
{
	return bBlock;
}

float ARPGProjectCharacter::Getcurrentstamina()
{
	return Currentstamina;
}

float ARPGProjectCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (fHealth <= 0)
		return 0.0f;

	const float ActaulDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (ActaulDamage >= 0)
	{
		if (bBlock)
		{
			fHealth -= ActaulDamage / 3;
			Currentstamina -= 30.f;

			FActorSpawnParameters SpawnInfo;

			UWorld* const World = GetWorld(); // get a reference to the world
			if (World) {
				// if world exists
				ASimpleEffect* BlockEppect = World->SpawnActor<ASimpleEffect>(BlockEppectClass, GetActorLocation(), GetActorRotation(), SpawnInfo);
			}
		}
		else
		{
			fHealth -= ActaulDamage;
		}
	}

	if (fHealth <= 0)
	{
		Die(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
	else
	{
		OnHit(DamageAmount, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : NULL, DamageCauser);
	}

	return ActaulDamage;
}

void ARPGProjectCharacter::OnHit(float DamageTaken, FDamageEvent const & DamageEvent, APawn * PawnInstigator, AActor * DamageCauser)
{
	if (bBlock)
		PlayAnimMontage(BeBlockAnim);
	else
		PlayAnimMontage(BeHitAnim);

	if (DamageTaken > 0)
	{
		ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
	}
}