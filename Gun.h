// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"


//forward declaration
class AProjectile;

UCLASS()
class TURNBASEDSHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();


	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* GunMesh;

	UPROPERTY(EditAnywhere)
	USceneComponent* MuzzleLocation;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable)
	void Fire(float Speed, APlayerCharacter *PlayerCharacter);


	//This needs to be set manually in blueprint
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ProjectileBP;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bUsingInaccuracy;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Inaccuracy;

	
	//TODO
	// static mesh for gun
	// look at bone mesh components
	// create a fire method
	//

		
};
