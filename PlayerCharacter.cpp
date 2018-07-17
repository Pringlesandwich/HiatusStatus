// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Gun.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}


// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	//Jump
	InputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::OnStartJump);
	InputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::OnStopJump);

	/**
	* REMOVED METHODS, LEFT FOR NOTES
	*
	//Camera Controls
	InputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);  // this is now done in Blueprint as I could not access the value of this in C++????
	InputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);

	//Dodge - this has been removed and is called in BP
	//InputComponent->BindAction("Dodge", IE_Pressed, this, &APlayerCharacter::Dodge);

	//firing projectile
	InputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::OnFire);
	
	//Test Method
	InputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::OnReleaseFire);

	//zoom
	InputComponent->BindAction("Aim", IE_Pressed, this, &APlayerCharacter::StartAiming);
	InputComponent->BindAction("Aim", IE_Released, this, &APlayerCharacter::StopAiming);
	*/
}


//Setters
void APlayerCharacter::SetHealth(float Delta) { Health = Delta; }
void APlayerCharacter::SetbCanDodge(bool Delta) { bCanDodge = Delta; }
void APlayerCharacter::SetTeam(int Delta) { Team = Delta; }


//Getters
bool APlayerCharacter::GetbCanDodge() { return bCanDodge; }
int APlayerCharacter::GetTeam() { return Team; }


void APlayerCharacter::CalculateDead()
{
	if (Health <= 0)
		IsDead = true;
	else
		IsDead = false;
}


void APlayerCharacter::CalculateHealth(float Delta)
{

	//UE_LOG(LogTemp, Warning, TEXT("Calculating Health - Damage: %f"), Delta);

	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("BPOnHit"), ar, NULL, true);

	Health -= Delta;
	CalculateDead();
}


#if WITH_EDITOR
void APlayerCharacter::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	IsDead = false;
	Health = 100;

	Super::PostEditChangeProperty(PropertyChangedEvent);

	CalculateDead();
}
#endif


void APlayerCharacter::Initialise(AGun * GunToSet, UCharacterMovementComponent* MovementComponentToSet)
{
	if (GunToSet != NULL && MovementComponentToSet != NULL)
	{
		PlayerGun = GunToSet;
		MovementComponent = MovementComponentToSet;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Initialise parameters are not correct, please check"));
	}
}


void APlayerCharacter::MoveForward(float Value)
{
	MoveForwardSpeed = Value;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		FRotator Rotation = Controller->GetControlRotation();

		// zero pitch if walking or falling
		if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling())
		{
			Rotation.Pitch = 0.0f;
		}

		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X); // create a X axis Vector with a direction relative to the character rotation
		AddMovementInput(Direction, Value); // add movement in direction
	}
}


void APlayerCharacter::MoveRight(float Value)
{
	MoveRightSpeed = Value;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation(); //Rotation is eqaul to character forward in map
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y); // create a Y axis Vector with a direction relative to the character rotation

		AddMovementInput(Direction, Value); // add movement in direction
	}
}


//Jump function, basic and need heavy modification???
void APlayerCharacter::OnStartJump()
{
	bPressedJump = true;
}


void APlayerCharacter::OnStopJump()
{
	bPressedJump = false;
}


//TODO - create on C++ migration
void APlayerCharacter::SwitchFromPlayer()
{
	//
}


void APlayerCharacter::Dodge(float InitDodgeFriction, float InitDodgeWalkSpeed, float DodgeForce, float TimeTillControllable, float TimeTillNextDodge)
{
	if (bCanDodge)
	{
		if (MoveForwardSpeed > 0.0f || MoveForwardSpeed < 0.0f || MoveRightSpeed > 0.0f || MoveRightSpeed < 0.0f)
		{
			MovementComponent->Velocity = FVector(0.0f, 0.0f, 0.0f);
			CharacterMovementDelta(0.0f, 0.0f);

			//set friction and walk speed to initial value
			CharacterMovementDelta(InitDodgeFriction, InitDodgeWalkSpeed);

			//Set DodgeVector
			FVector PlayerRotation = FirstPersonCameraComponent->GetComponentRotation().Vector();
			FRotator MovementRotation = FVector(MoveForwardSpeed, MoveRightSpeed, 0.0f).Rotation();
			FVector DeltaVector = MovementRotation.RotateVector(PlayerRotation);

			// add impulse through delta vector times 100 in x and y
			MovementComponent->AddImpulse(FVector(DeltaVector.X * DodgeForce, DeltaVector.Y * DodgeForce, 100.0f), true);
			
			bCanDodge = false;

			//wait TimeTillControllable then set defaults back
			FirstDodgeDel.BindUFunction(this, FName("CharacterMovementDelta"), 8.0f, 900.0f); //default 8.0, 900.0;
			GetWorldTimerManager().SetTimer(FDodgeDuration, FirstDodgeDel, TimeTillControllable, false);

			//Wait TimeTillNextDodge then enable bCanDodge
			SecondDodgeDel.BindUFunction(this, FName("SetbCanDodge"), true);
			GetWorldTimerManager().SetTimer(FDodgeCooldown, SecondDodgeDel, TimeTillNextDodge, false);
		}
	}
}


void APlayerCharacter::CharacterMovementDelta(float Friction, float WalkSpeed)
{
	MovementComponent->GroundFriction = Friction;
	MovementComponent->MaxWalkSpeed = WalkSpeed;
}


void APlayerCharacter::OnFire()
{
	//UE_LOG(LogTemp, Warning, TEXT("Fire"));

	APlayerCharacter *PCReference;
	PCReference	= this;

	PlayerGun->Fire(300.0f, PCReference);
}

