#include "MCubeComponent.h"


UMCubeComponent::UMCubeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	BoxLength = 50.f;
	Threshold = 0.5f;
	Scale = 200.f;

	XScale = 30;
	ZScale = 30;
	YScale = 30;

	XBorderLerp = 5;
	YBorderLerp = 5;
	ZBorderLerp = 0;

	AddedBorderLerp = 0.6f;
}


void UMCubeComponent::GenerateMarchingCubeMesh(int X_Iterations, int Y_Iterations, int Z_Iterations, float _BoxLength)
{
	if(RuntimeMesh == nullptr || Noise == nullptr)
	{
		return;
	}
	XScale = X_Iterations;
	YScale = Y_Iterations;
	Z_Iterations = ZScale;
	BoxLength = _BoxLength;

	/* Deletes previos mesh */
	//RuntimeMesh->ClearSection(0,0);

	GenerateVertices();
	//Normals.Init(FVector(0, 0, 1), Vertices.Num());
	//Tangents.Init(FRuntimeMeshTangent(0, 1, 0),  Vertices.Num());
	//VertexColors.Init(FColor::White,  Vertices.Num());
	//GenerateTriangles();
	GenerateMesh();
}

void UMCubeComponent::GenerateVertices() {
	for(int32 y = 0; y < YScale; y++)
	{
		for(int32 x = 0; x < XScale; x++)
		{
			for(int32 z = 0; z < ZScale; z++)
			{
				FVector4 Points[8];
				for(int i = 0; i < 8; i++)
				{
					Points[i] = FVector4();
				}
				int Index = GetTriangulationIndexForCube(x, y, z, Points);

				if(edgeTable[Index] != 0)
				{
					for(int32 i = 0; triangulation[Index][i] != -1; i +=3)
					{
						int a0 = cornerIndexAFromEdge[triangulation[Index][i]];
						int b0 = cornerIndexBFromEdge[triangulation[Index][i]];

						int a1 = cornerIndexAFromEdge[triangulation[Index][i+1]];
						int b1 = cornerIndexBFromEdge[triangulation[Index][i+1]];

						int a2 = cornerIndexAFromEdge[triangulation[Index][i+2]];
						int b2 = cornerIndexBFromEdge[triangulation[Index][i+2]];

						FVector FirstVertice = InterpolateVerts(Points[a2], Points[b2]);
						FVector SecondVertice = InterpolateVerts(Points[a1], Points[b1]);
						FVector ThirdVertice = InterpolateVerts(Points[a0], Points[b0]);

						int FirstVerticeIndex = Vertices.Add(FirstVertice);
						int SecondVerticeIndex = Vertices.Add(SecondVertice);
						int ThirdVerticeIndex = Vertices.Add(ThirdVertice);

						Triangles.Add(FirstVerticeIndex);
						Triangles.Add(SecondVerticeIndex);
						Triangles.Add(ThirdVerticeIndex);

						UV.Add(FVector2D(x,y));
						UV.Add(FVector2D(x,y));
						UV.Add(FVector2D(x,y));

						FVector Normal = FVector::CrossProduct(FirstVertice - ThirdVertice, SecondVertice - ThirdVertice).GetSafeNormal();
						Normals.Add(Normal);
						Normals.Add(Normal);
						Normals.Add(Normal);

						FVector Tanget = (FirstVertice - SecondVertice).GetSafeNormal2D();
						Tangents.Add(Tanget);
						Tangents.Add(Tanget);
						Tangents.Add(Tanget);


					}
				}
			}

		}
	}
}

FVector UMCubeComponent::InterpolateVerts(FVector4& FirstCorner, FVector4& SecondCorner) {
    const float t = (Threshold - FirstCorner.W) / (SecondCorner.W - FirstCorner.W);
    return FVector(FirstCorner + t * (SecondCorner - FirstCorner)) * BoxLength;
}

int UMCubeComponent::GetTriangulationIndexForCube(int x, int y, int z, FVector4 (&PointsOut)[8])
{
	int Index = 0;
	//      4--------5
	//     /|       /|
	//    / |      / |               Z
	//   7--------6  |               |
	//   |  |     |  |               |
	//   |  0-----|--1              []-------X
	//   | /      | /               /
	//   |/       |/               /
	//   3--------2               Y

	// 0
	float Value = GetNoiseValueForGridCoordinates(x,y,z);
	PointsOut[0] = FVector4(x, y ,z, Value);
	if(Value < Threshold)
	{
		Index |=  1;
	}
	// 1
	Value = GetNoiseValueForGridCoordinates(x + 1, y, z);
	PointsOut[1] = FVector4(x + 1, y , z, Value);
	if(Value < Threshold)
	{
		Index |=  2;
	}
	// 2
	Value = GetNoiseValueForGridCoordinates(x + 1, y + 1, z);
	PointsOut[2] = FVector4(x + 1, y + 1, z, Value);
	if(Value < Threshold)
	{
		Index |=  4;
	}
	// 3
	Value = GetNoiseValueForGridCoordinates(x, y + 1, z);
	PointsOut[3] = FVector4(x , y + 1 , z, Value);
	if(Value < Threshold)
	{
		Index |=  8;
	}
	// 4
	Value = GetNoiseValueForGridCoordinates(x, y, z + 1);
	PointsOut[4] = FVector4(x, y ,z + 1, Value);
	if(Value < Threshold)
	{
		Index |=  16;
	}
	// 5
	Value = GetNoiseValueForGridCoordinates(x + 1,y  ,z + 1);
	PointsOut[5] = FVector4(x + 1,y ,z + 1, Value);
	if(Value < Threshold)
	{
		Index |=  32;
	}
	// 6
	Value = GetNoiseValueForGridCoordinates(x + 1, y + 1, z + 1);
	PointsOut[6] = FVector4(x + 1, y + 1, z + 1, Value);
	if(Value < Threshold)
	{
		Index |= 64;
	}
	// 7
	Value = GetNoiseValueForGridCoordinates(x,y + 1, z + 1);
	PointsOut[7] = FVector4(x ,y + 1, z + 1, Value);
	if(Value < Threshold)
	{
		Index |= 128;
	}

	return Index;
}

void UMCubeComponent::GenerateMesh() {
	RuntimeMesh->CreateSectionFromComponents(0,
		0,
		0,
		Vertices,
		Triangles,
		Normals,
		UV,
		VertexColors,
		Tangents,
		ERuntimeMeshUpdateFrequency::Infrequent,
		true);
}




float UMCubeComponent::GetNoiseValueForGridCoordinates(int x, int y, int z) {
	if(x == 0 || x >= XScale || y == 0 || y >= YScale || z == 0 || z >= ZScale)
	{
		return 0.f;
	}
	float Res = Noise->GetValue(
		(x * NoiseInputScale),
		(y * NoiseInputScale),
		(z * NoiseInputScale)
	);

	return Res * Scale;
}


void UMCubeComponent::BeginPlay()
{
	Super::BeginPlay();
	//Noise = Cast<UPerlinNoiseComponent>(GetOwner()->GetComponentByClass(UPerlinNoiseComponent::StaticClass()));
	//RuntimeMesh = Cast<URuntimeMeshComponentStatic>(GetOwner()->GetComponentByClass(URuntimeMeshComponentStatic::StaticClass()));

}


void UMCubeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
