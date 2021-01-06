
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    class UBoxComponent* Box;

    UFUNCTION(BlueprintCallable)
    void GenerateMesh();

    UPROPERTY(EditAnywhere , BlueprintReadWrite)
    float BoxLength;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

};
