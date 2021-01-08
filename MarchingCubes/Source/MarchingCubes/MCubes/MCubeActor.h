
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PerlinNoiseComponent.h"
#include "MCubeActor.generated.h"

/**
 * Example actor to generate 3D Perlin Noise mesh
 * Generates Mesh at the BeginPlay
 * @warning currently(07/01/2021) there is a bug of replacing the mesh in RuntimeMeshComponent see (https://github.com/TriAxis-Games/RuntimeMeshComponent/issues/194)
 */
UCLASS()
class AMCubeActor : public AActor
{
	GENERATED_BODY()

public:
	AMCubeActor();

	// -----------------------------------------------------------------------------------------------------------
	// Components
	// -----------------------------------------------------------------------------------------------------------

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    class URuntimeMeshComponentStatic* RuntimeMesh;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    class UPerlinNoiseComponent* Noise;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    class UMCubeComponent* MCube;

	// -----------------------------------------------------------------------------------------------------------
	// Generate Mesh method
	// -----------------------------------------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
    void GenerateMesh();

	// -----------------------------------------------------------------------------------------------------------
	// Meshes size attributes
	// -----------------------------------------------------------------------------------------------------------
    UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "Marching cube")
    float BoxLength;

    UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "Marching cube")
    int XLength;

    UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "Marching cube")
    int YLength;

    UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "Marching cube")
    int ZLength;

	// -----------------------------------------------------------------------------------------------------------
	// Noise attributes
	// -----------------------------------------------------------------------------------------------------------
	UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "Marching cube|Noise")
    float Frequency;

    UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "Marching cube|Noise")
    float Lacunarity;

    UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "Marching cube|Noise")
	TEnumAsByte<qualities::PerlinNoiseQuality> NoiseQuality;

    UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "Marching cube|Noise")
	int OctaveCount;

    UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "Marching cube|Noise")
	float Persistence;

    UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "Marching cube|Noise")
	int Seed;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

};
