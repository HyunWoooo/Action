// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "PublicCharacter.h"
#include "RPGProjectCharacter.generated.h"


UCLASS(config=Game)
class ARPGProjectCharacter : public APublicCharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ARPGProjectCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	virtual void Tick(const float deltaTime) override;

	virtual void PostInitializeComponents() override;

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual void WeaponAttack() override;

	TArray<FString> Inventory;

	// 키를 누르면 스프린트 플래그를 설정합니다.
	UFUNCTION()
	void StartSprint();

	// 키를 떼면 스프린트 플래그를 지웁니다.
	UFUNCTION()
	void StopSprint();

	// 키를 누르면 스프린트 플래그를 설정합니다.
	UFUNCTION()
	void StartBlock();

	// 키를 떼면 스프린트 플래그를 지웁니다.
	UFUNCTION()
	void StopBlock();

	// 키를 누르면 픽업 플래그를 설정합니다.
	void BeginPickUp();

	// 키를 누르면 픽업 플래그를 지웁니다.
	void EndPickUp();

	// 키를 누르면 인벤토리 플래그를 설정합니다.
	void ShowInventory();

	UFUNCTION(BlueprintCallable, Category = "MyBasic|State")
	bool IsSprint();

	UFUNCTION(BlueprintCallable, Category = "MyBasic|State")
	bool IsBlock();

	UFUNCTION(BlueprintPure, Category = "Stamina")
	float Getcurrentstamina();

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<class ASimpleEffect> BlockEppectClass;

	virtual void OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ShieldMesh;
	
	bool bSprint;
	bool bBlock;

	bool bIsPickingUp;

	float Initailstamina;
	float Currentstamina;
};

