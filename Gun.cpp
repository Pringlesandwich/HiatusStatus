// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"
#include "Projectile.h"


// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	
	SetRootComponent(GunMesh);


	MuzzleLocation = CreateDefaultSubobject<USceneComponent>("MuzzleLocation");

	

	//MuzzleLocation->SetRelativeLocation(FVector(0, 0, 0));
	//MuzzleLocation->SetRelativeRotation(FRotator(0, 90.0f, 0));


}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
	// had to put this here due to Actor lifecycle https://docs.unrealengine.com/en-us/Programming/UnrealArchitecture/Actors/ActorLifecycle
	MuzzleLocation->AttachTo(GunMesh);

}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::Fire(float Speed, APlayerCharacter *PlayerCharacter)
{

	if (ProjectileBP != NULL)
	{


		FVector SpawnLocation = MuzzleLocation->GetComponentLocation();
		FRotator SpawnRotation = MuzzleLocation->GetComponentRotation();

		if (bUsingInaccuracy)
		{

			SpawnRotation.Pitch += FMath::RandRange(-Inaccuracy, Inaccuracy);

			SpawnRotation.Yaw += FMath::RandRange(-Inaccuracy, Inaccuracy);
		}
		

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		//ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// spawn the projectile at the muzzle
		AProjectile* ProjectileTarget = GetWorld()->SpawnActor<AProjectile>(ProjectileBP, SpawnLocation, SpawnRotation, ActorSpawnParams);

		//ProjectileBP->SetFriendlyPlayer(*PlayerCharacter);

		//APlayerCharacter* TargetActor = Cast<APlayerCharacter>(OtherActor);

		//AProjectile* ProjectileTarget = Cast<AProjectile>(ProjectileBP);

		//ProjectileTarget->SetFriendlyPlayer(PlayerCharacter);

		if (PlayerCharacter == NULL)
		{

			UE_LOG(LogTemp, Warning, TEXT("Character Reference is NULL!"));
		}
		else
		{
			if (ProjectileTarget != NULL)
			{
				UE_LOG(LogTemp, Warning, TEXT("*PlayerCharacter is: %s"), *PlayerCharacter->GetName());
				//ProjectileTarget->SetFriendlyPlayer(PlayerCharacter); // THIS CRASHES THE GAME!!!!
				ProjectileTarget->SetFriendlyPlayer(PlayerCharacter);
				//ProjectileTarget->SetDamage(10.0f);
			}
		}


		//ProjectileBP->SetFriendlyPlayer(*PlayerCharacter);

	}
	else
	{

		UE_LOG(LogTemp, Warning, TEXT("There is no projectile class set within blueprint, you're shooting blanks bro!"));

	}
	
}



