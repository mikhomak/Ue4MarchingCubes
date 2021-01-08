// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <noise.h>

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PerlinNoiseComponent.generated.h"

// UE4 compatible enum that corresponds to perlin noise qualities.
UENUM(BluePrintType)
namespace qualities {
	enum PerlinNoiseQuality {
		QUALITY_FAST	UMETA(DisplayName = "Fast Quality"),
		QUALITY_STD		UMETA(DisplayName = "Standard Quality"),
		QUALITY_BEST	UMETA(DisplayName = "Best Quailty")
	};
}

// Change UNDERWATER_API to whatever your project API name is
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UPerlinNoiseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPerlinNoiseComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// The main function to call to get noise output
	UFUNCTION(BluePrintCallable, meta = (Keywords = "noise perlin"), Category = "Perlin")
	float GetValue(float x, float y, float z) const;

	// Easy setup function to set all the options at once
	UFUNCTION(BluePrintCallable, Category = "Perlin")
	void SetupOptions(
		float Frequency_,
		float Lacunarity_,
		qualities::PerlinNoiseQuality NoiseQuality_,
		int OctaveCount_,
		float Persistence_,
		int Seed_
	);

	// Setters/getters for all the individual noise properties
	UFUNCTION(BluePrintCallable, Category = "Perlin")
	float GetFrequency() const;
	UFUNCTION(BluePrintCallable, Category = "Perlin")
	float GetLacunarity() const;
	UFUNCTION(BluePrintCallable, Category = "Perlin")
	qualities::PerlinNoiseQuality GetNoiseQuality() const;
	UFUNCTION(BluePrintCallable, Category = "Perlin")
	int GetOctaveCount() const;
	UFUNCTION(BluePrintCallable, Category = "Perlin")
	float GetPersistence() const;
	UFUNCTION(BluePrintCallable, Category = "Perlin")
	int GetSeed() const;
	UFUNCTION(BluePrintCallable, Category = "Perlin")
	int GetSourceModuleCount() const;
	UFUNCTION(BluePrintCallable, Category = "Perlin")
	void SetFrequency(float frequency);
	UFUNCTION(BluePrintCallable, Category = "Perlin")
	void SetLacunarity(float lacunarity);
	UFUNCTION(BluePrintCallable, Category = "Perlin")
	void SetNoiseQuality(qualities::PerlinNoiseQuality noiseQuality);
	UFUNCTION(BluePrintCallable, Category = "Perlin")
	void SetOctaveCount(int octaveCount);
	UFUNCTION(BluePrintCallable, Category = "Perlin")
	void SetPersistence(float persistence);
	UFUNCTION(BluePrintCallable, Category = "Perlin")
	void SetSeed(int seed);
	UFUNCTION(BluePrintCallable, Category = "Perlin")
	void SetRandomSeed();

private:
	noise::module::Perlin perlinNoise;
	noise::NoiseQuality convertBPNoiseQuality(qualities::PerlinNoiseQuality quality) const;
	qualities::PerlinNoiseQuality convertBPNoiseQuality(noise::NoiseQuality quality) const;
	void UpdateOptions();

	// Those are private because they shouldn't be modified at runtime, but can be edited in the editor before running the game
	UPROPERTY(EditAnywhere, Category = "Perlin")
	float Frequency = 1.0;
	UPROPERTY(EditAnywhere, Category = "Perlin")
	float Lacunarity = 2.0;
	UPROPERTY(EditAnywhere, Category = "Perlin")
	TEnumAsByte<qualities::PerlinNoiseQuality> NoiseQuality = qualities::QUALITY_STD;
	UPROPERTY(EditAnywhere, Category = "Perlin")
	int OctaveCount = 6;
	UPROPERTY(EditAnywhere, Category = "Perlin")
	float Persistence = 0.5;
	UPROPERTY(EditAnywhere, Category = "Perlin")
	int Seed = 0;
};
