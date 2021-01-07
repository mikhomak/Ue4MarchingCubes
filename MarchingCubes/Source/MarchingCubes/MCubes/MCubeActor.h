
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PerlinNoiseComponent.h"
#include "MCubeActor.generated.h"

UCLASS()
class AMCubeActor : public AActor
{
	GENERATED_BODY()

public:
	AMCubeActor();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    class URuntimeMeshComponentStatic* RuntimeMesh;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    class UPerlinNoiseComponent* Noise;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    class UMCubeComponent* MCube;

    UFUNCTION(BlueprintCallable)
    void GenerateMesh();


    UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "Marching cube")
    float BoxLength;

    UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "Marching cube")
    int XLength;

    UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "Marching cube")
    int YLength;

    UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "Marching cube")
    int ZLength;

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

private:
    bool bFirstCreated;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

};
