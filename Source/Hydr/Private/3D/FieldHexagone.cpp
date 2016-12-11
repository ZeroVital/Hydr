#include "Hydr.h"
#include "FieldHexagone.h"


// Sets default values
AFieldHexagone::AFieldHexagone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFieldHexagone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFieldHexagone::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

