// Fill out your copyright notice in the Description page of Project Settings.

#include "PerlinNoiseComponent.h"

// Sets default values for this component's properties
UPerlinNoiseComponent::UPerlinNoiseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	UpdateOptions();
	// ...
}


// Called when the game starts
void UPerlinNoiseComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}


// Called every frame
void UPerlinNoiseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UPerlinNoiseComponent::GetFrequency() const
{
	return perlinNoise.GetFrequency();
}

float UPerlinNoiseComponent::GetLacunarity() const
{
	return perlinNoise.GetLacunarity();
}

qualities::PerlinNoiseQuality UPerlinNoiseComponent::GetNoiseQuality() const
{ 
	return convertBPNoiseQuality(perlinNoise.GetNoiseQuality());
}

int UPerlinNoiseComponent::GetOctaveCount() const
{
	return perlinNoise.GetOctaveCount();
}

float UPerlinNoiseComponent::GetPersistence() const
{
	return perlinNoise.GetPersistence();
}

int UPerlinNoiseComponent::GetSeed() const
{
	return perlinNoise.GetSeed();
}

int UPerlinNoiseComponent::GetSourceModuleCount() const
{
	return perlinNoise.GetSourceModuleCount();
}

float UPerlinNoiseComponent::GetValue(float x, float y, float z) const
{
	float value = perlinNoise.GetValue(x, y, z);
	// UE_LOG(LogTemp, Warning, TEXT("Perlin value for: %f,%f,%f: %f"), x, y, z, value);
	return value;
}

void UPerlinNoiseComponent::SetFrequency(float frequency)
{
	return perlinNoise.SetFrequency(frequency);
}

void UPerlinNoiseComponent::SetLacunarity(float lacunarity)
{
	return perlinNoise.SetLacunarity(lacunarity);
}

void UPerlinNoiseComponent::SetNoiseQuality(qualities::PerlinNoiseQuality noiseQuality)
{
	return perlinNoise.SetNoiseQuality(convertBPNoiseQuality(noiseQuality));
}

void UPerlinNoiseComponent::SetOctaveCount(int octaveCount)
{
	perlinNoise.SetOctaveCount(octaveCount);
}

void UPerlinNoiseComponent::SetPersistence(float persistence)
{
	perlinNoise.SetPersistence(persistence);
}

void UPerlinNoiseComponent::SetSeed(int seed)
{
	perlinNoise.SetSeed(seed);
}

void UPerlinNoiseComponent::SetRandomSeed()
{
	SetSeed(FMath::RandRange(0, 1000000));
}

void UPerlinNoiseComponent::SetupOptions(float Frequency_, float Lacunarity_, qualities::PerlinNoiseQuality NoiseQuality_, int OctaveCount_, float Persistence_, int Seed_) {
	SetFrequency(Frequency_);
	SetLacunarity(Lacunarity_);
	SetNoiseQuality(NoiseQuality_);
	SetOctaveCount(OctaveCount_);
	SetPersistence(Persistence_);
	SetSeed(Seed_);
}

noise::NoiseQuality UPerlinNoiseComponent::convertBPNoiseQuality(qualities::PerlinNoiseQuality quality) const
{
	switch (quality) {
		case qualities::QUALITY_FAST:
			return noise::QUALITY_FAST;
			break;
		break;
		case qualities::QUALITY_STD:
			return noise::QUALITY_STD;
			break;
		case qualities::QUALITY_BEST:
			return noise::QUALITY_BEST;
			break;
		default:
			return noise::QUALITY_STD;
	}
}

qualities::PerlinNoiseQuality UPerlinNoiseComponent::convertBPNoiseQuality(noise::NoiseQuality quality) const
{
	switch (quality) {
	case noise::QUALITY_FAST:
		return qualities::QUALITY_FAST;
		break;
	case noise::QUALITY_STD:
		return qualities ::QUALITY_STD;
		break;
	case noise::QUALITY_BEST:
		return qualities::QUALITY_BEST;
		break;
	default:
		return qualities::QUALITY_STD;
	}
}

void UPerlinNoiseComponent::UpdateOptions()
{
	SetupOptions(Frequency, Lacunarity, NoiseQuality, OctaveCount, Persistence, Seed);
}
