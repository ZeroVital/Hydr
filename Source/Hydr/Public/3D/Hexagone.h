#pragma once

#include "GameFramework/Actor.h"
#include "Hexagone.generated.h"

UCLASS()
class HYDR_API AHexagone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHexagone();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void PostInitializeComponents() override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Player1")
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player1")
	UMaterialInstanceDynamic* MaterialHexagone;
	UPROPERTY(EditAnywhere)
	int32	HealPoints;
	UFUNCTION(Category = Default)
	void OnClicked();
};
