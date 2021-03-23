// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/DungeonEx/DA_PathNameSelector.h"
#include "Builders/GridFlow/GridFlowQuery.h"
#include "Builders/GridFlow/GridFlowModel.h"

UDA_PathNameSelector::UDA_PathNameSelector(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

bool UDA_PathNameSelector::SelectNode_Implementation(UGridFlowModel* Model, UGridFlowConfig* Config, UGridFlowBuilder* Builder, UGridFlowQuery* Query, const FRandomStream& RandomStream, int32 TileX, int32 TileY, const FTransform& MarkerTransform)
{
	bool bSuccessGetPathName = false;
	if (!Model || !Model->Tilemap || !Model->AbstractGraph || !Config) {
		UE_LOG(LogGame, Error, TEXT("Invalid grid flow query object state"));
		return false;
	}

	FString PathName = FString("");
	const FVector TileCoords = Query->ConvertWorldToTileCoord(MarkerTransform.GetLocation());
	const FGridFlowTilemapCell* CellPtr = Model->Tilemap->GetSafe(TileCoords.X, TileCoords.Y);
	if (CellPtr) {
		const FVector NodeCoord = CellPtr->ChunkCoord;
		for (const UFlowAbstractNode* Node : Model->AbstractGraph->GraphNodes) {
			if (!Node) continue;
			if (NodeCoord.Equals(Node->Coord)) {
				UFANodeTilemapDomainData* TilemapDomainData = Node->FindDomainData<UFANodeTilemapDomainData>();
				if (TilemapDomainData) {
					PathName = Node->PathName;
					bSuccessGetPathName = true;
				}
			}
		}
	}

	if (!bSuccessGetPathName || PathName.IsEmpty() || PathNameArr.Num() < 1)
	{
		return false;
	}

	if (PathNameArr.Contains(PathName))
	{
		return true;
	}

	return false;
}
