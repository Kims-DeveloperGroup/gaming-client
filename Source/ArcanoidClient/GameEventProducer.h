#pragma once
#include "CoreMinimal.h"
#include "Engine.h"
#include "SocketSubsystem.h"
#include "Networking.h"
#include "Sockets.h"
#include "GameEventProducer.generated.h"

UENUM()
enum EventType
{
	LOGIN,
	LOGOUT,
	HEALTH
};

USTRUCT()
struct FGameEvent
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FText UserName;

	UPROPERTY()
	TEnumAsByte<EventType> eventType;


};

class ARCANOIDCLIENT_API GameEventProducer
{
public:
	GameEventProducer(FIPv4Address* host, int32 port);
	~GameEventProducer();

	FIPv4Address* Host;
	int32 Port;
	FSocket* Socket;
	virtual void ConnectServer();
	virtual void SendEvent(EventType eventType);

private:
	virtual FString Serialize(FGameEvent * Event);
};
