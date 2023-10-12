// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "test.generated.h"


DECLARE_DELEGATE_ThreeParams(FOnGetOnlineIndexComplete, bool, int32,int);
DECLARE_DELEGATE_ThreeParams(FOnGetIndexDetail, bool, TArray<FString>,bool);
DECLARE_DELEGATE_OneParam(FOnPutSuccessful, bool);
UCLASS()
class IITTPAPP_API Atest : public AActor
{
	GENERATED_BODY()
	
public:	
	Atest();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;	
public:
	
	// Variable for index

	UFUNCTION(BlueprintCallable)
	void GetindexBP(int ind);

	UPROPERTY(BlueprintReadOnly)
	bool DidOnlineIndexGet = false;
	
	UPROPERTY(BlueprintReadOnly)
	int OnlineIndex = 0;

	UPROPERTY(BlueprintReadOnly)
	bool DidOnlineIndexGet2 = false;

	UPROPERTY(BlueprintReadOnly)
	int OnlineIndex2 = 0;

public:

	// variable for GetData
	UPROPERTY(BlueprintReadOnly)
	bool HasDataOrNot = true;

	UPROPERTY(BlueprintReadOnly)
	bool HasDataOrNot2 = true;

    void GetData(int index, FOnGetIndexDetail DetailDel,int ind);

	UFUNCTION(BlueprintCallable)
	void GetDataBp(int index,int ind);

	UPROPERTY(BlueprintReadOnly)
	bool DidGetIndexData = false;

	UPROPERTY(BlueprintReadOnly)
	bool DidGetIndexData2 = false;

	UPROPERTY(BlueprintReadOnly)
	FString tempRollnumber;

	UPROPERTY(BlueprintReadOnly)
	FString tempDescription;

	UPROPERTY(BlueprintReadOnly)
	FString tempimageUrl;

	UPROPERTY(BlueprintReadOnly)
	FString tempitemLost;

	UPROPERTY(BlueprintReadOnly)
	FString tempRollnumber2;

	UPROPERTY(BlueprintReadOnly)
	FString tempDescription2;

	UPROPERTY(BlueprintReadOnly)
	FString tempimageUrl2;

	UPROPERTY(BlueprintReadOnly)
	FString tempitemLost2;
	

public:

	// variable for input data
	UFUNCTION(BlueprintCallable)
	void PutDataBP(int index, FString Rn, FString itemL, FString Descrip, FString imgURL,int ind);
	void Putdata(int index,FString Rn , FString itemL , FString Descrip , FString imgURL,int ind);

	UPROPERTY(BlueprintReadOnly)
	bool DidUpdateSuccess = false;

	UPROPERTY(BlueprintReadOnly)
		bool DidUpdateSuccess2 = false;

public:
	//////update avariable +1

	UFUNCTION(BlueprintCallable)
	void UpdateIndexBPPlus(int index,int ind);
	
	UPROPERTY(BlueprintReadonly)
	bool UpdateIndexPlusSuccessful = false;

	UPROPERTY(BlueprintReadonly)
	bool UpdateIndexPlusSuccessful2 = false;

public:
	//////// Delete index
	UFUNCTION(Blueprintcallable)
	void DeleteDataAtIndexBP(int index,int ind);

	UPROPERTY(BlueprintReadOnly)
	bool DidDelSuccess = false;

	UPROPERTY(BlueprintReadOnly)
	bool DidDelSuccess2 = false;

public:
	UFUNCTION(BlueprintCallable)
	void UploadImageToFirebase(const FString& FP,int index);

	UFUNCTION(BlueprintCallable)
	void DownloadImageFromFirebase(int index);

public:
	UPROPERTY(BlueprintReadOnly)
	bool ImageUpdatedDone = false;

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* MyTexture;

	UPROPERTY(BlueprintReadOnly)
	FSlateBrush Brush;

public:
	UFUNCTION(BlueprintCallable)
	void OpenImageFileDialog();

	//UFUNCTION(BlueprintCallable, Category = "MyCategory")
	//void OpenGallery();

public:
	

	UFUNCTION(BlueprintCallable)
	void UpdateTempVariable(int ind);

	UPROPERTY(BlueprintReadWrite)
	int TempOnlineVariable;

	UPROPERTY(BlueprintReadWrite)
	int TempOnlineVariable2;

	UPROPERTY(BlueprintReadWrite)
	int lastindex;

public:

	UFUNCTION(BlueprintCallable)
	void StartCombo();

	UFUNCTION(BlueprintImplementableEvent)
	void StartCombo_Implementation();
};
