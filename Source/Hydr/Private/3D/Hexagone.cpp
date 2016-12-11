#include "Hydr.h"
#include "Hexagone.h"


// Sets default values
AHexagone::AHexagone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	static ConstructorHelpers::FObjectFinder <UStaticMesh>StaticMesh(TEXT("StaticMesh'/Game/HexagoneBalzar.HexagoneBalzar'"));
	Mesh->SetStaticMesh(StaticMesh.Object);
	//Mesh->SetMaterial(0, Material.Object);
	Mesh->SetMobility(EComponentMobility::Movable);

}

// Called when the game starts or when spawned
void AHexagone::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHexagone::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);
	//FRotator rotationToApply;
	//rotationToApply.Pitch += 1;
	//FHitResult hitResult;
	//Super::AddActorLocalRotation(rotationToApply,false, &hitResult, ETeleportType::TeleportPhysics);

}

void AHexagone::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//MaterialHexagone->
	//Mesh->SetMaterial()
}

void AHexagone::OnClicked()
{
	Mesh->bRenderCustomDepth = true;
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Yellow, FString::Printf(TEXT("%s clicked !"), this->GetName()));
}


