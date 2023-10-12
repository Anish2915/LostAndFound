// Fill out your copyright notice in the Description page of Project Settings.


#include "test.h"
#include "Http.h"
#include "HttpModule.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Kismet/GameplayStatics.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Misc/FileHelper.h"
#include "Misc/Base64.h"
#include "ImageUtils.h"
#include "Brushes/SlateImageBrush.h"
#include <string>
#include <Engine.h>


#include "Engine/World.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"




Atest::Atest()
{
	PrimaryActorTick.bCanEverTick = true;
}

void Atest::BeginPlay()
{
	Super::BeginPlay();

}



void Atest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void Atest::GetData(int index,FOnGetIndexDetail DetailDel,int ind)
{
    FString Url = FString::Printf(TEXT("https://iittpapp-default-rtdb.firebaseio.com/data%d/%d.json"),ind,index);
    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->SetVerb("GET");
    Request->SetURL(Url);
    Request->OnProcessRequestComplete().BindLambda(
        [DetailDel,ind](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess) {
            bool SuccessOrNot = false;
            bool GetDataOrNot = true;
            TArray<FString> temp;
            
                if (bSuccess) {
                    int32 StatusCode = Response->GetResponseCode();
                    FString ContentType = Response->GetContentType();
                    FString Content = Response->GetContentAsString();
                    SuccessOrNot = true;
                    if (ContentType.Contains("application/json")) {
                        if (!Content.IsEmpty()) {
                            TSharedPtr<FJsonObject> JsonObject;
                            TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Content);

                            if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid()) {
                                FString rollNumber = JsonObject->GetStringField("Rollnumber");
                                FString desc = JsonObject->GetStringField("desc");
                                FString imageUrl = JsonObject->GetStringField("imageUrl");
                                FString itemLost = JsonObject->GetStringField("itemlost");
                                temp.Add(rollNumber);
                                temp.Add(itemLost);
                                temp.Add(desc);
                                temp.Add(imageUrl);
                            }
                        }

                        if (temp.Num() != 4) {
                            GetDataOrNot = false;
                            temp.Empty();
                            temp.Init(FString(), 4);
                        }
                        else {
                            GetDataOrNot = true;
                        }
                    }
                }
            
            DetailDel.ExecuteIfBound(SuccessOrNot, temp,GetDataOrNot);
    });
    Request->ProcessRequest();
}

void Atest::GetDataBp(int index,int ind)
{
    FOnGetIndexDetail DetailDel = FOnGetIndexDetail::CreateLambda(
        [this,ind](bool SuccessOrNot, TArray<FString> temp, bool GetDataOrNot) {
            if (ind == 1) {
                if (SuccessOrNot) {
                    this->DidGetIndexData = true;
                    this->tempRollnumber = temp[0];
                    this->tempitemLost = temp[1];
                    this->tempDescription = temp[2];
                    this->tempimageUrl = temp[3];
                }
                else {
                    this->DidGetIndexData = false;
                }
                this->HasDataOrNot = GetDataOrNot;
            }
            else if (ind == 2) {
                if (SuccessOrNot) {
                    this->DidGetIndexData2 = true;
                    this->tempRollnumber2 = temp[0];
                    this->tempitemLost2 = temp[1];
                    this->tempDescription2 = temp[2];
                    this->tempimageUrl2 = temp[3];
                }
                else {
                    this->DidGetIndexData2 = false;
                }
                this->HasDataOrNot2 = GetDataOrNot;
            }
        }
    );
    GetData(index, DetailDel,ind);
}



void Atest::GetindexBP(int ind)
{
    FString Url = FString::Printf(TEXT("https://iittpapp-default-rtdb.firebaseio.com/avariable%d.json"), ind);
    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->SetVerb("GET");
    Request->SetURL(Url);
    Request->OnProcessRequestComplete().BindLambda(
        [ind, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess) {
            if (ind == 1) {
                this->DidOnlineIndexGet = bSuccess;
                if (bSuccess) {
                    FString ContentAsString = Response->GetContentAsString();
                    this->OnlineIndex = FCString::Atoi(*ContentAsString);
                }
            }
            if (ind == 2) {
                this->DidOnlineIndexGet2 = bSuccess;
                if (bSuccess) {
                    FString ContentAsString = Response->GetContentAsString();
                    this->OnlineIndex2 = FCString::Atoi(*ContentAsString);
                }
            }
        }
    );
    Request->ProcessRequest();
}


void Atest::PutDataBP(int index, FString Rn, FString itemL, FString Descrip, FString imgURL,int ind)
{
    
    FString Url = FString::Printf(TEXT("https://iittpapp-default-rtdb.firebaseio.com/data%d/%d.json"), ind, index);
    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->SetVerb("PATCH");
    Request->SetURL(Url);
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
    FString JsonData = FString::Printf(TEXT("{ \"RollNumber\": \"%s\", \"desc\": \"%s\", \"imageUrl\": \"%s\", \"itemLost\": \"%s\" }"), *Rn, *Descrip, *imgURL, *itemL);
    Request->SetContentAsString(JsonData);
    Request->OnProcessRequestComplete().BindLambda([ind, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess) {
        
            if (ind == 1) {
                this->DidUpdateSuccess = bSuccess;
            }
            else if (ind == 2) {
                this->DidUpdateSuccess2 = bSuccess;
            }
        }
    );

    Request->ProcessRequest();
}



void Atest::UpdateIndexBPPlus(int index,int ind)
{
    FString Url = FString::Printf(TEXT("https://iittpapp-default-rtdb.firebaseio.com/.json"));
    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->SetVerb("PATCH");
    Request->SetURL(Url);
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
    FString JsonData = FString::Printf(TEXT("{\"avariable%d\": %d}"),ind, index+1);
    Request->SetContentAsString(JsonData);
    Request->OnProcessRequestComplete().BindLambda([index, this, ind](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess) {
        if (ind == 1) {
            this->UpdateIndexPlusSuccessful = bSuccess;
            if (bSuccess) {
                this->OnlineIndex = index + 1;
            }
        }
        else if (ind == 2) {
            this->UpdateIndexPlusSuccessful2 = bSuccess;
            if (bSuccess) {
                this->OnlineIndex2 = index + 1;
            }
        }
        }
    );
    Request->ProcessRequest();
}

void Atest::DeleteDataAtIndexBP(int index,int ind)
{
    
    FString Url = FString::Printf(TEXT("https://iittpapp-default-rtdb.firebaseio.com/data%d/%d.json"),ind,index);
    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->SetVerb("DELETE");
    Request->SetURL(Url);
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));

    Request->OnProcessRequestComplete().BindLambda([this,ind](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess) {
        if (ind == 1) {
            this->DidDelSuccess = bSuccess;
        }
        else if (ind == 2) {
            this->DidDelSuccess2 = bSuccess;
        }
        }
    );

    Request->ProcessRequest();
}

void Atest::UploadImageToFirebase(const FString& FP,int index)
{
    FString FilePath = FPaths::Combine(FPaths::ProjectContentDir(), "images" , "Resumee.jpg");

    TArray<uint8> ImageData;
    FFileHelper::LoadFileToArray(ImageData, *FilePath);
    FString ImageBase64 = FBase64::Encode(ImageData);

    
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    JsonObject->SetStringField("imageUrl", ImageBase64);

    FString JsonString;
    TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

    FString Url = FString::Printf(TEXT("https://iittpapp-default-rtdb.firebaseio.com/data/%d.json"),index);
    
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetVerb("PATCH");
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
    HttpRequest->SetURL(Url);
    HttpRequest->SetContentAsString(JsonString);
    HttpRequest->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess) {
        if (bSuccess) {
            UE_LOG(LogTemp, Warning, TEXT("Image Added Succesfully"));
            }
        }
    );

    HttpRequest->ProcessRequest();
}

void Atest::DownloadImageFromFirebase(int index)
{
    FString Url = FString::Printf(TEXT("https://iittpapp-default-rtdb.firebaseio.com/data/%d.json"), index);

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetVerb("GET");
    HttpRequest->SetURL(Url);
    HttpRequest->OnProcessRequestComplete().BindLambda([index, this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess) {
        this->ImageUpdatedDone = bSuccess;
        if (bSuccess && Response.IsValid()) {
            FString ResponseString = Response->GetContentAsString();
            TSharedPtr<FJsonObject> JsonObject;
            TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(ResponseString);
            if (FJsonSerializer::Deserialize(JsonReader, JsonObject)) {
                FString ImageBase64 = JsonObject->GetStringField("imageUrl");
                TArray<uint8> ImageData;
                FBase64::Decode(ImageBase64, ImageData);
                FString FilePath = FPaths::Combine(FPaths::ProjectContentDir(), "downloadimages", FString::FromInt(index) + ".jpg");
                FFileHelper::SaveArrayToFile(ImageData, *FilePath);
                UE_LOG(LogTemp, Warning, TEXT("Image Downloaded Successfully"));
                
                
                //////////////////
                Brush = FSlateImageBrush(FilePath,FVector2D(100));
                /////////////////
                UTexture2D* LoadedTexture = FImageUtils::ImportFileAsTexture2D(FilePath);

                if (LoadedTexture != nullptr)
                {
                    this->MyTexture = LoadedTexture;
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to create texture from file: %s"), *FilePath);
                }
            }
            
        }
        }
    );

    HttpRequest->ProcessRequest();
}
void Atest::OpenImageFileDialog()
{
    FString defaultPath = FString("C:/");
    FString fileTypes = FString("Image Files (*.png;*.jpg)|*.png;*.jpg");
    TArray<FString, FDefaultAllocator> selectedFiles;
    bool result = FDesktopPlatformModule::Get()->OpenFileDialog(nullptr, FString("Select Image"), defaultPath, FString(""), fileTypes, EFileDialogFlags::None, selectedFiles);
    if (result)
    {
        FString filePath = selectedFiles[0];
        UE_LOG(LogTemp, Log, TEXT("Selected image file path: %s"), *filePath);
    }

}
/*
void Atest::OpenGallery()
{
#if PLATFORM_ANDROID
    JavaVM* JavaVM = FAndroidApplication::GetJavaVM();
    JNIEnv* JNIEnv = FAndroidApplication::GetJNIEnv(JavaVM);

    jclass JavaClassID = FJavaWrapper::FindClassGlobalRef(JNIEnv, "com/epicgames/ue4/GameActivity");
    jmethodID JavaMethodID = FJavaWrapper::FindMethod(
        JNIEnv,
        JavaClassID,
        "AndroidThunkJava_OpenGallery",
        "()V",
        false
    );

    FJavaWrapper::CallVoidMethod(JNIEnv, FJavaWrapper::GameActivityThis, JavaMethodID);

    FJavaWrapper::DeleteGlobalRef(JNIEnv, JavaClassID);
#endif
}
*/


void Atest::UpdateTempVariable(int ind)
{
    if (ind == 1) {
        TempOnlineVariable = OnlineIndex;
    }
    else if (ind == 2) {
        TempOnlineVariable2 = OnlineIndex2; 
    }

    

}



void Atest::StartCombo() {

}





