// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"




// Sets default values
AProjectile::AProjectile()
{

	// Use a sphere as a simple collision representation
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));

	RootComponent = SphereCollision;

	SphereCollision->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	//SphereCollision->BodyInstance.SetCollisionProfileName("Projectile");

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	
	ProjectileMovement->InitialSpeed = ProjectileSpeed;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	//ProjectileMovement->Bounciness = 0.3f;

}


// setters

void AProjectile::SetFriendlyPlayer(APlayerCharacter *ProjectileOwner) { FriendlyPlayer = ProjectileOwner; }

void AProjectile::SetDamage(float DamageToSet) { Damage = DamageToSet; }



void AProjectile::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{

	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		DestroyProjectile();
	}
	else if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{

		//UE_LOG(LogTemp, Warning, TEXT("Other Actor hit!"));

		//UE_LOG(LogTemp, Warning, TEXT("Other Actor is: %s"), *OtherActor->GetName());

		if (OtherActor->IsA(APlayerCharacter::StaticClass()))
		{

			if (OtherActor == FriendlyPlayer)
			{
				UE_LOG(LogTemp, Warning, TEXT("FriendlyPlayer Hit!"));
			}
			else
			{

				//UE_LOG(LogTemp, Warning, TEXT("PlayerHit!"));

				APlayerCharacter* TargetActor = Cast<APlayerCharacter>(OtherActor);

				HitPlayer = TargetActor;

				if (HitPlayer != nullptr)
				{
					UE_LOG(LogTemp, Warning, TEXT("HitPlayer is: %s"), *HitPlayer->GetName());
				}

				TargetActor->CalculateHealth(Damage);

				//OtherActor->CalculateHealth(Damage);

				FOutputDeviceNull ar;
				this->CallFunctionByNameWithArguments(TEXT("TestSoundMethod"), ar, NULL, true);

				//DestroyProjectile();

			}

		}
		else if (OtherActor->IsA(AProjectile::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("FriendlyPlayer Hit!"));
		}
		else
		{
			DestroyProjectile();
		}

	}

	DestroyProjectile();
		
}


void AProjectile::DestroyProjectile()
{
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("BPOnHit"), ar, NULL, true);
}

