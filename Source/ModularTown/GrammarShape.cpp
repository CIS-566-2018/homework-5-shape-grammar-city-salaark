// GrammarShape.cpp

#include "GrammarShape.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include <iostream>
#include <cstring>
#include <string>

const int grid_size = 600;

UGrammarShape::UGrammarShape() {
 	rule = 0;
 	spaces = 1;
 	space_dir = 0;
}

void UGrammarShape::expand() {
	switch(rule) {
		case 0: //create room foundations (1-5 rooms)
			rule_rooms(rand()%5);
			break;
		case 1: //add walls to room (terminal)
			rule_walls();
			break;
		case 2: //add roof to room (terminal)
			rule_roof();
			break;
	}
}

void UGrammarShape::rule_rooms(int count) {
	for(int i = 0; i <= count; i++) {
		UGrammarShape* shape = NewObject<UGrammarShape>(this, UGrammarShape::StaticClass()); //*FString("Room" + FString::FromInt(i))
		static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Game/EternalTemple/Environment/Bases/Thin/Base_Floor_01"));
 		shape->SetStaticMesh(MeshObj.Object);
 		shape->rule = 1;

 		switch(i) {
 			case 0:
 				shape->spaces = count;
 				shape->space_dir = 3;
 				if(count == 0) shape->SetRelativeScale3D(FVector(2,2,2));
 				break;
			case 1:
				shape->SetRelativeLocation(FVector(grid_size, 0, 0));
				break;
			case 2:
				shape->SetRelativeLocation(FVector(0, grid_size, 0));
				shape->space_dir = 1;
				break;
			case 3:
				shape->SetRelativeLocation(FVector(0, -1*grid_size, 0));
				shape->space_dir = 2;
				break;
			case 4:
				shape->SetRelativeLocation(FVector(-1*grid_size, 0, 0));
				shape->space_dir = 3;
				break;
 		}
 		shape->SetupAttachment(this);
		shape->expand();
	}
}

void UGrammarShape::rule_walls() {
	switch(spaces) {
		case 0:
			construct_wall(0); construct_wall(1);
			construct_wall(2); construct_wall(3);
			break;
		case 1:
			for(int i = 0; i < 4; i++) {
				if(i != space_dir) {
					construct_wall(i);
				}
			}
			break;
		case 2:
			construct_wall(0); construct_wall(1);
			break;
		case 3:
			construct_wall(0);
			break;
	}
	this->rule = 2;
	this->expand();
}

void UGrammarShape::rule_roof() {
	UGrammarShape* roof = NewObject<UGrammarShape>(this, UGrammarShape::StaticClass());
	if(spaces != 1) {
		static ConstructorHelpers::FObjectFinder<UStaticMesh> RoofObj(TEXT("/Game/EternalTemple/MeshParts/SM_Roof_3lvl_01.SM_Roof_3lvl_01"));
		roof->SetStaticMesh(RoofObj.Object);
		roof->SetRelativeLocation(FVector(0,0,grid_size/2));
		if(spaces == 0) roof->SetRelativeLocation(FVector(grid_size/-12,0,grid_size/2));
		else roof->SetRelativeScale3D(FVector(1.25,1.25,1.25));
		roof->SetupAttachment(this);
		return;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> RoofObj(TEXT("/Game/EternalTemple/MeshParts/SM_Roof_Corridor_End_01"));
 	roof->SetStaticMesh(RoofObj.Object);

 	roof->SetRelativeScale3D(FVector(0.9,0.9,0.9));
	switch(space_dir) {
		case 0:
			roof->SetRelativeLocation(FVector(grid_size/-2-grid_size/12,0,grid_size/-6));
			roof->SetRelativeRotation(FRotator(0,-90,0));
			break;
		case 1:
			roof->SetRelativeLocation(FVector(0,grid_size/-2+grid_size/24,grid_size/-6));
			break;
		case 2:
			roof->SetRelativeLocation(FVector(0,grid_size/2-grid_size/24,grid_size/-6));
			roof->SetRelativeRotation(FRotator(0,180,0));
			break;
		case 3:
			roof->SetRelativeLocation(FVector(grid_size/2-grid_size/12,0,grid_size/-6));
			roof->SetRelativeRotation(FRotator(0,90,0));
			break;
	}
	roof->SetupAttachment(this);
}

void UGrammarShape::construct_wall(int dir) {
	UGrammarShape* wall = NewObject<UGrammarShape>(this, UGrammarShape::StaticClass());
	if(rand()%4 == 0) {
		static ConstructorHelpers::FObjectFinder<UStaticMesh> WallObj(TEXT("/Game/EternalTemple/MeshParts/SM_Building_Entrance_02"));
	 	wall->SetStaticMesh(WallObj.Object);
	 } else {
	 	static ConstructorHelpers::FObjectFinder<UStaticMesh> WallObj(TEXT("/Game/EternalTemple/MeshParts/SM_Wall_Segment_03"));
	 	wall->SetStaticMesh(WallObj.Object);
	 }

 	UGrammarShape* column = NewObject<UGrammarShape>(this, UGrammarShape::StaticClass());
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ColObj(TEXT("/Game/EternalTemple/MeshParts/SM_Building_Column_01"));
 	column->SetStaticMesh(ColObj.Object);

	switch(dir) {
		case 0:
			wall->SetRelativeLocation(FVector(-1*(grid_size/2+grid_size/12), 0, grid_size/-6));
			column->SetRelativeLocation(FVector(-1*(grid_size/2+grid_size/12), grid_size/2, grid_size/-6));
			break;
		case 3:
			wall->SetRelativeLocation(FVector(grid_size/2-grid_size/12, 0, grid_size/-6));
			column->SetRelativeLocation(FVector(grid_size/2-grid_size/12, grid_size/-2, grid_size/-6));
			break;
		case 2:
			wall->SetRelativeLocation(FVector(grid_size/-12, grid_size/2, grid_size/-6));
			wall->SetRelativeRotation(FRotator(0,90,0));
			column->SetRelativeLocation(FVector(grid_size/2-grid_size/12, grid_size/2, grid_size/-6));
			break;
		case 1:
			wall->SetRelativeLocation(FVector(grid_size/-12, -1*(grid_size/2), grid_size/-6));
			wall->SetRelativeRotation(FRotator(0,90,0));
			column->SetRelativeLocation(FVector(grid_size/-2-grid_size/12, -1*(grid_size/2), grid_size/-6));
			break;
	}
	wall->SetupAttachment(this);
	column->SetupAttachment(this);
}
