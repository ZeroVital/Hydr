#pragma once

#include "GameFramework/Actor.h"
#include "FieldHexagone.generated.h"

UCLASS()
class HYDR_API AFieldHexagone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFieldHexagone();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
