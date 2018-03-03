// ModularBuilding.cpp

#include "ModularBuilding.h"


// Sets default values
AModularBuilding::AModularBuilding() {
 	// Set this actor to never call Tick()
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	Base = CreateDefaultSubobject<UGrammarShape>(TEXT("Base"));
	Base->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Base->expand();
}

// Called when the game starts or when spawned
void AModularBuilding::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void AModularBuilding::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

