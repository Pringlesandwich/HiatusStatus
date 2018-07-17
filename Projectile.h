// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"



//class APlayerCharacter;

UCLASS()
class TURNBASEDSHOOTER_API AProjectile : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AProjectile();

	//friendly fire test
	UPROPERTY(BlueprintReadWrite)
	APlayerCharacter *FriendlyPlayer;

	UFUNCTION(BlueprintCallable)
	void SetFriendlyPlayer(APlayerCharacter *ProjectileOwner);

	UPROPERTY(BlueprintReadWrite)
	APlayerCharacter *HitPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	UProjectileMovementComponent* ProjectileMovement = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float ProjectileSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float Damage;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	

	UFUNCTION(BlueprintCallable)
	void SetDamage(float DamageToSet);

	UFUNCTION()
	void DestroyProjectile();

};
