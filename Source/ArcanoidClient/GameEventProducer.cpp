// Fill out your copyright notice in the Description page of Project Settings.
#include "GameEventProducer.h"
#include "CoreMinimal.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"


GameEventProducer::GameEventProducer(FIPv4Address* host, int32 port)
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_DGram, TEXT("event-sender-socket"), true);
	Host = host;
	Port = port;
}

GameEventProducer::~GameEventProducer()
{
	Socket->Close();
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("GameEventProducer has been closed."));
}

void GameEventProducer::ConnectServer()
{
	FIPv4Address ip(127, 0, 0, 1);
	TSharedRef<FInternetAddr> ServerAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	ServerAddress->SetIp(ip.Value);
	ServerAddress->SetPort(Port);

	bool connected = Socket->Connect(*ServerAddress);
	if (connected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Connect to Server"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Fail to connect to Server"));
	}
}

void GameEventProducer::SendEvent(EventType eventType)
{
	FText UserName = FText::FromString("rikim");
	FGameEvent GEvent;
	GEvent.eventType = eventType;
	GEvent.UserName = UserName;
	
	FString msg = Serialize(&GEvent);
	TCHAR *serializedChar = msg.GetCharArray().GetData();
	int32 size = FCString::Strlen(serializedChar);
	int32 sent = 0;
	
	bool successful = Socket->Send((uint8*)TCHAR_TO_UTF8(serializedChar), size, sent);
	if (successful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Message sent"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Fail to send message"));
	}
}

FString GameEventProducer::Serialize(FGameEvent* Event)
{
	FString Output;
	FJsonObjectConverter::UStructToJsonObjectString(Event->StaticStruct(), Event, Output, 0, 0, 1, NULL, true);
	return Output;
}
