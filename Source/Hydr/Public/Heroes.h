// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Heroes.generated.h"

UCLASS()
class HYDR_API AHeroes : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHeroes();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heroes")
	uint8 BaseHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heroes")
	uint8 CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heroes")
	uint8 BaseForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heroes")
	uint8 CurrentForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heroes")
	uint8 BaseDefense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heroes")
	uint8 CurrentDefense;

	//TArray<UGearComponent>;

};
