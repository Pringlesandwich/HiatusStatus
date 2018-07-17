// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

//forward decleration
class AGun;


/**
* Enum for gadget type. Used when assigning and selecting a Gadget.
* TODO - add to code
*/
UENUM(BlueprintType)
enum class GadgetType : uint8
{
	Scanner,
	Invisibility,
	DoubleDodge,
	RepairKit
};


/**
* This is the Class for the pawn that the player will control via the
* BP_FirstPersonPlayerController.
* 
* Responsibilities:
* Setting up the pawn and spawning and managing attached classes like the gun.
* Governing Inputs received and actions undertaken by the pawn itself. 
*/
UCLASS()
class TURNBASEDSHOOTER_API APlayerCharacter : public ACharacter
{

	GENERATED_BODY()
	
	//For creating a camera component for BP manipulation
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	//Getters and Setters
	UFUNCTION(BlueprintCallable)
	void SetHealth(float Delta);

	UFUNCTION(BlueprintCallable)
	void SetbCanDodge(bool Delta);

	UFUNCTION(BlueprintCallable)
	bool GetbCanDodge();

	UFUNCTION()
	int GetTeam();

	UFUNCTION()
	void SetTeam(int Delta);

public:
	//Check for Health and set bIsDead
	UFUNCTION(BlueprintCallable)
	virtual void CalculateDead();

	//Like SetHealth then calls CalculateDead()
	UFUNCTION(BlueprintCallable)
	virtual void CalculateHealth(float Delta);

	//safegaurd against potential unwelcome edits
#if WITH_EDITOR
	//Make Health = 100.0f and bIsDead = false
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// Assign object from blueprint to the forward declerated classes
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialise(AGun* GunToSet, UCharacterMovementComponent* MovementComponentToSet);

	//List of custom input based actions
	UFUNCTION()
	void MoveForward(float Val);

	UFUNCTION()
	void MoveRight(float Val);

	UFUNCTION()
	void OnStartJump();

	UFUNCTION()
	void OnStopJump();


	//On switch reset cetain skills - TODO - impliment with C++
	UFUNCTION()
	void SwitchFromPlayer();

	/**
	* Dodge move called in BP, impulses character in a direction
	* Currently requires Blueprint input
	*
	* @param	InitDodgeFriction	Friction to set for Dodge
	* @param	InitDodgeWalkSpeed	Walk speed to set for Dodge
	* @param	DodgeForce			Amount of force to add to Dodge
	* @param	TimeTillControllable	Dodge duration
	* @param	TimeTillNextDodge	Time until player can use Dodge again
	*/
	UFUNCTION(BlueprintCallable, Category = "Dodge")
	void Dodge(float InitDodgeFriction, float InitDodgeWalkSpeed, float DodgeForce, float TimeTillControllable, float TimeTillNextDodge);

	/**
	* Setting the friction and walk speed of the player CharacterMovement component
	* Component set in Initialise(), this function is called in Dodge()
	*
	* @param	Friction	Friction to set for Dodge
	* @param	WalkSpeed	Walk speed to set for Dodge
	*/
	UFUNCTION(BlueprintCallable, Category = "Dodge")
	void CharacterMovementDelta(float Friction, float WalkSpeed);
	
	//Handles firing communicates with Gun
	UFUNCTION(BlueprintCallable, Category = "Firing")
	void OnFire();

	//Player Health, to check death
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerCharacter")
	float Health = 100;

	//State of player, used by Game Mode
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerCharacter")
	bool IsDead = false;

	//Reference to Gun, set via Initialise(), called in OnFire()
	UPROPERTY()
	AGun* PlayerGun;

	//Reference to MovementComponent, Set in Initialise(), called in CharacterMovementDelta() va Dodge()
	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;

	//Speed player move forward, referenced in Dodge
	UPROPERTY()
	float MoveForwardSpeed;

	//Speed player move sideways, referenced in Dodge
	UPROPERTY()
	float MoveRightSpeed;

	//Used for cooldown in Dodge()
	UPROPERTY(EditAnywhere)
	bool bCanDodge = true;

	//Dodge durations
	UPROPERTY(EditAnywhere)
	float DodgeDuration;

	//Dodge cooldown
	UPROPERTY(EditAnywhere)
	float DodgeCooldown;

	//Team of player. 0 for Corp, 1 for Junk
	UPROPERTY(BlueprintReadWrite)
	int Team;

	//Amount player moves up during dodge, edited in blueprints
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Dodge)
	float ZDodgeAmount = 2000;

	//Amount player moves sideways during dodge, edited in blueprints
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Dodge)
	float XYDodgeAmount = 4000;

	//Dodge delegates for cooldown 
	FTimerDelegate FirstDodgeDel;
	FTimerDelegate SecondDodgeDel;

	//Timer handlers for for Dodge
	FTimerHandle FDodgeDuration;
	FTimerHandle FDodgeCooldown;

};
