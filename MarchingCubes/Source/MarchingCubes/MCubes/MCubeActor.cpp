
#include "MCubeActor.h"
#include "MCubeComponent.h"
#include "PerlinNoiseComponent.h"
#include "Components/RuntimeMeshComponentStatic.h"
#include "Components/BoxComponent.h"


AMCubeActor::AMCubeActor()
{
    RuntimeMesh = CreateDefaultSubobject<URuntimeMeshComponentStatic>(TEXT("Runtime Mesh Component"));
    RootComponent = RuntimeMesh;

    /* Setting the attachment rules */
    const FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);

    /* Creates Perlin Noise Component */
    Noise = CreateDefaultSubobject<UPerlinNoiseComponent>(TEXT("Perlin Noise Component"));

    /* Creates Marching Cube Component */
    MCube = CreateDefaultSubobject<UMCubeComponent>(TEXT("Marching cube Component"));
    MCube->Noise = Noise;
    MCube->RuntimeMesh = RuntimeMesh;

    /* Creates Box Component */
    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
    Box->AttachToComponent(RuntimeMesh, AttachmentTransformRules);
    Box->SetRelativeScale3D(FVector(500)); /* Makes default scale of the box a lil bigger */

    /* Sets default box length*/
    BoxLength = 50.f;
}

void AMCubeActor::GenerateMesh()
{
    /* Updates the noises seed to get new noise values */
    Noise->SetRandomSeed();

    const FVector BoxScale = Box->GetRelativeScale3D() / BoxLength;
    MCube->GenerateMarchingCubeMesh((int) BoxScale.X, (int) BoxScale.Y, (int) BoxScale.Z, BoxLength);
}


void AMCubeActor::BeginPlay()
{
	Super::BeginPlay();

}


void AMCubeActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
