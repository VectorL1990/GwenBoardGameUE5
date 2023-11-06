// Fill out your copyright notice in the Description page of Project Settings.


#include "SkynetRunnable.h"
#include "GwenBoardGameInstance.h"

#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "IPAddress.h"
#include "Sockets.h"
#include "HAL/RunnableThread.h"
#include "Async/Async.h"

FSkynetRunnable::FSkynetRunnable(FString ip, 
				const int32 port,
				int32 inRunnableId,
				int32 inRecvBufferSize,
				int32 inSendBufferSize,
				float inTimeBetweenTicks,
				TWeakObjectPtr<UGwenBoardGameInstance> inOwner)
				: ipAddr(ip)
				, port(port)
				, runnableId(inRunnableId)
				, recvBufferSize(inRecvBufferSize)
				, sendBufferSize(inSendBufferSize)
				, timeBetweenTicks(inTimeBetweenTicks)
				, threadOwner(inOwner)
{
}

FSkynetRunnable::~FSkynetRunnable()
{
				Stop();
				if (skynetRunnableThread)
				{
								skynetRunnableThread->WaitForCompletion();
								delete skynetRunnableThread;
								skynetRunnableThread = nullptr;
				}
}

void FSkynetRunnable::Start()
{
				check(FPlatformProcess::SupportsMultithreading());
				skynetRunnableThread = FRunnableThread::Create(this, 
								*FString::Printf(TEXT("FSkynetRunnable %s:%d"), *ipAddr, port),
								128 * 1024,
								TPri_Normal);
}

void FSkynetRunnable::AddToOutbox(TArray<uint8> msg)
{
				outBox.Enqueue(msg);
}

TArray<uint8> FSkynetRunnable::ReadFromInbox()
{
				TArray<uint8> msg;
				inBox.Dequeue(msg);
				return msg;
}

bool FSkynetRunnable::Init()
{
				running = true;
				connected = false;
				return true;
}

uint32 FSkynetRunnable::Run()
{
				while (running)
				{
								FDateTime timeBeginOfTick = FDateTime::UtcNow();
								
								if (!connected)
								{
												socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
												if (!socket)
												{
																return 0;
												}
												socket->SetReceiveBufferSize(recvBufferSize, actualRecvBufferSize);
												socket->SetSendBufferSize(sendBufferSize, actualSendBufferSize);

												FIPv4Address ip;
												FIPv4Address::Parse(ipAddr, ip);

												TSharedRef<FInternetAddr> internetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
												internetAddr->SetIp(ip.Value);
												internetAddr->SetPort(port);

												connected = socket->Connect(*internetAddr);
												if (connected)
												{
																AsyncTask(ENamedThreads::GameThread, [this]() {
																				threadOwner.Get()->ExecuteOnConnected(runnableId, threadOwner);
																});
												}
												continue;
								}

								if (!socket)
								{
												continue;
								}

								socket->SetNonBlocking(true);
								int32 t_bytesRead;
								uint8 t_dummy;
								if (!socket->Recv(&t_dummy, 1, t_bytesRead, ESocketReceiveFlags::Peek))
								{
												running = false;
												continue;
								}
								socket->SetNonBlocking(false);

								while (!outBox.IsEmpty())
								{
												TArray<uint8> toSend;
												outBox.Dequeue(toSend);

												if (!BlockingSend(toSend.GetData(), toSend.Num()))
												{
																running = false;
																continue;
												}
								}

								uint32 pendingDataSize = 0;
								TArray<uint8> receivedData;
								int32 bytesReadTotal = 0;
								while (running)
								{
												if (!socket->HasPendingData(pendingDataSize))
												{
																break;
												}

												receivedData.SetNumUninitialized(bytesReadTotal + pendingDataSize);

												int32 bytesRead = 0;
												if (!socket->Recv(receivedData.GetData() + bytesReadTotal, pendingDataSize, bytesRead))
												{
																break;
												}
												bytesReadTotal += bytesRead;
								}

								if (running && receivedData.Num() != 0)
								{
												inBox.Enqueue(receivedData);
												AsyncTask(ENamedThreads::GameThread, [this](){
																threadOwner.Get()->ExecuteOnMessageReceived(runnableId, threadOwner);
												});
								}

								FDateTime timeEndOfTick = FDateTime::UtcNow();
								FTimespan tickDuration = timeEndOfTick - timeBeginOfTick;
								float secondsThisTickTook = tickDuration.GetTotalSeconds();
								float timeToSleep = timeBetweenTicks - secondsThisTickTook;
								if (timeToSleep > 0.f)
								{
												//AsyncTask(ENamedThreads::GameThread, [timeToSleep]() { ATcpSocketConnection::PrintToConsole(FString::Printf(TEXT("Sleeping: %f seconds"), timeToSleep), false); });
												FPlatformProcess::Sleep(timeToSleep);
								}
				}

				connected = false;

				AsyncTask(ENamedThreads::GameThread, [this](){
								threadOwner.Get()->ExecuteOnDisconnected(runnableId, threadOwner);
				});

				SocketShutdown();
				if (socket)
				{
								delete socket;
								socket = nullptr;
				}

				return 0;
}

void FSkynetRunnable::Stop()
{
				running = false;
}

void FSkynetRunnable::Exit()
{

}

void FSkynetRunnable::SocketShutdown()
{
				if (socket)
				{
								socket->Close();
				}
}

bool FSkynetRunnable::Connected()
{
				return connected;
}

bool FSkynetRunnable::BlockingSend(const uint8* data, int32 bytesToSend)
{
				if (bytesToSend)
				{
								int32 bytesSend = 0;
								if (!socket->Send(data, bytesToSend, bytesSend))
								{
												return false;
								}
				}
				return true;
}

