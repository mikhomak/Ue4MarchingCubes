
#include "MCubeActor.h"
#include "MCubeComponent.h"
#include "PerlinNoiseComponent.h"
#include "Components/RuntimeMeshComponentStatic.h"

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

    /* Sets default box length*/
    BoxLength = 50.f;

    XLength = 30;
    YLength = 30;
    ZLength = 30;

    /* Sets default noise parameters */
    Frequency = 1.f;
    Lacunarity = 2.f;
    NoiseQuality = qualities::QUALITY_STD;
    OctaveCount = 6;
    Persistence = 0.5f;
    Seed = 0;
    Noise->SetupOptions(Frequency, Lacunarity,
                        NoiseQuality, OctaveCount,
                        Persistence, Seed);
}

void AMCubeActor::GenerateMesh()
{
    /* Updates the noises seed to get new noise values */
    Noise->SetRandomSeed();

    MCube->GenerateMarchingCubeMesh(XLength, YLength, ZLength, BoxLength);
}


void AMCubeActor::BeginPlay()
{
	Super::BeginPlay();
}


void AMCubeActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
