

#ifndef CONNECT_H
#define CONNECT_H

#include "basicSockets.h"
#include <sstream>
#include <vector>
#include "../util/TedKrovetzAesNiWrapperC.h"
#include <stdint.h>
#include <iomanip>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

extern BmrNet ** communicationSenders;
extern BmrNet ** communicationReceivers;

extern int partyNum;



//setting up communication
void initCommunication(string addr, int port, int player, int mode);
void initializeCommunication(int* ports);
void initializeCommunicationSerial(int* ports); //Use this for many parties
void initializeCommunication(char* filename, int p);


//synchronization functions
void sendByte(int player, char* toSend, int length, int conn);
void receiveByte(int player, int length, int conn);
void synchronize(int length = 1);

void start_communication();
void pause_communication();
void resume_communication();
void end_communication(string str);


template<typename T>
void sendVector(const vector<T> &vec, size_t player, size_t size);
template<typename T>
void sendArr(T* arr, size_t player, size_t size);

template<typename T>
void receiveVector(vector<T> &vec, size_t player, size_t size);
template<typename T>
void receiveArr(T* arr, size_t player, size_t size);

template<typename T>
void sendTwoVectors(const vector<T> &vec1, const vector<T> &vec2, size_t player, size_t size1, size_t size2);
template<typename T>
void receiveTwoVectors(vector<T> &vec1, vector<T> &vec2, size_t player, size_t size1, size_t size2);

template<typename T>
void sendThreeVectors(const vector<T> &vec1, const vector<T> &vec2, const vector<T> &vec3, 
					  size_t player, size_t size1, size_t size2, size_t size3);
template<typename T>
void receiveThreeVectors(vector<T> &vec1, vector<T> &vec2, vector<T> &vec3, 
					  size_t player, size_t size1, size_t size2, size_t size3);

template<typename T>
void sendFourVectors(const vector<T> &vec1, const vector<T> &vec2, const vector<T> &vec3,
					 const vector<T> &vec4, size_t player, size_t size1, size_t size2, size_t size3, size_t size4);
template<typename T>
void receiveFourVectors(vector<T> &vec1, vector<T> &vec2, vector<T> &vec3, 
						vector<T> &vec4, size_t player, size_t size1, size_t size2, size_t size3, size_t size4);

template<typename T>
void sendSixVectors(const vector<T> &vec1, const vector<T> &vec2, const vector<T> &vec3,
					 const vector<T> &vec4, const vector<T> &vec5, const vector<T> &vec6, 
					 size_t player, size_t size1, size_t size2, size_t size3, size_t size4, size_t size5, size_t size6);
template<typename T>
void receiveSixVectors(vector<T> &vec1, vector<T> &vec2, vector<T> &vec3, 
						vector<T> &vec4, vector<T> &vec5, vector<T> &vec6,
					 size_t player, size_t size1, size_t size2, size_t size3, size_t size4, size_t size5, size_t size6);


uint64_t getCurrentTime();

template<typename T>
void sendVector(const vector<T> &vec, size_t player, size_t size)
{
	uint64_t bytesSent = size*sizeof(T);

#if (LOG_DEBUG)
	cout << "Sending " << bytesSent << " Bytes to player " << player << " at "<<getCurrentTime()<< " via ";
	if (sizeof(T) == 8)
		cout << "myType" << endl;
	else 
		cout << "smallType" << endl;
	
	auto t1 = high_resolution_clock::now();
#endif

	if(!communicationSenders[player]->sendMsg(vec.data(), size * sizeof(T), 0))
		cout << "Send vector error" << endl;

#if (LOG_DEBUG)
	
	auto t2 = high_resolution_clock::now();
	auto tt = (duration_cast<duration<double>>(t2 - t1)).count();

	cout << "Done Sending " << bytesSent << " Bytes to player " << player << " at "<<getCurrentTime()<< " via ";
	if (sizeof(T) == 8)
		cout << "myType";
	else 
		cout << "smallType";
	cout<<" in time "<<tt<<" seconds at speed = "<<(bytesSent/(1.0*tt*1000000))<<" MBps."<<endl;
#endif
}


template<typename T>
void sendArr(T* arr, size_t player, size_t size)
{
	uint64_t bytesSent = size*sizeof(T);

#if (LOG_DEBUG)
	cout << "Sending " << bytesSent << " Bytes to player " << player << " at "<<getCurrentTime()<< " via ";
	if (sizeof(T) == 8)
		cout << "myType" << endl;
	else 
		cout << "smallType" << endl;
	
	auto t1 = high_resolution_clock::now();
#endif

	if(!communicationSenders[player]->sendMsg(arr, size * sizeof(T), 0))
		cout << "Send array error" << endl;

#if (LOG_DEBUG)
	
	auto t2 = high_resolution_clock::now();
	auto tt = (duration_cast<duration<double>>(t2 - t1)).count();

	cout << "Done Sending " << bytesSent << " Bytes to player " << player << " at "<<getCurrentTime()<< " via ";
	if (sizeof(T) == 8)
		cout << "myType";
	else 
		cout << "smallType";
	cout<<" in time "<<tt<<" seconds at speed = "<<(bytesSent/(1.0*tt*1000000))<<" MBps."<<endl;
#endif
}

template<typename T>
void receiveVector(vector<T> &vec, size_t player, size_t size)
{
	uint64_t bytesReceived = size*sizeof(T);

#if (LOG_DEBUG)
	cout << "Receiving " << bytesReceived << " Bytes from player " << player << " at "<<getCurrentTime()<< " via ";
	if (sizeof(T) == 8)
		cout << "myType" << endl;
	else 
		cout << "smallType" << endl;

	auto t1 = high_resolution_clock::now();
#endif

	if(!communicationReceivers[player]->receiveMsg(vec.data(), size * sizeof(T), 0))
		cout << "Receive myType vector error" << endl;
		
#if (LOG_DEBUG)
	
	auto t2 = high_resolution_clock::now();
	auto tt = (duration_cast<duration<double>>(t2 - t1)).count();
	
	cout << "Done Receiving " << bytesReceived << " Bytes from player " << player << " at "<<getCurrentTime()<< " via ";
	if (sizeof(T) == 8)
		cout << "myType";
	else 
		cout << "smallType";
	cout<<" in time "<<tt<<" seconds at speed = "<<(bytesReceived/(1.0*tt*1000000))<<" MBps."<<endl;
#endif

}

template<typename T>
void receiveArr(T* arr, size_t player, size_t size)
{
	uint64_t bytesReceived = size*sizeof(T);

#if (LOG_DEBUG)
	cout << "Receiving " << bytesReceived << " Bytes from player " << player << " at "<<getCurrentTime()<< " via ";
	if (sizeof(T) == 8)
		cout << "myType" << endl;
	else 
		cout << "smallType" << endl;

	auto t1 = high_resolution_clock::now();
#endif

	if(!communicationReceivers[player]->receiveMsg(arr, size * sizeof(T), 0))
		cout << "Receive myType vector error" << endl;
		
#if (LOG_DEBUG)
	
	auto t2 = high_resolution_clock::now();
	auto tt = (duration_cast<duration<double>>(t2 - t1)).count();
	
	cout << "Done Receiving " << bytesReceived << " Bytes from player " << player << " at "<<getCurrentTime()<< " via ";
	if (sizeof(T) == 8)
		cout << "myType";
	else 
		cout << "smallType";
	cout<<" in time "<<tt<<" seconds at speed = "<<(bytesReceived/(1.0*tt*1000000))<<" MBps."<<endl;
#endif
}


template<typename T>
void sendTwoVectors(const vector<T> &vec1, const vector<T> &vec2, size_t player, size_t size1, size_t size2)
{
	vector<T> temp(size1+size2);
	for (size_t i = 0; i < size1; ++i)
		temp[i] = vec1[i];

	for (size_t i = 0; i < size2; ++i)
		temp[size1 + i] = vec2[i];

	sendVector<T>(temp, player, size1+size2);
}

template<typename T>
void receiveTwoVectors(vector<T> &vec1, vector<T> &vec2, size_t player, size_t size1, size_t size2)
{
	vector<T> temp(size1+size2);
	receiveVector<T>(temp, player, size1+size2);

	for (size_t i = 0; i < size1; ++i)
		vec1[i] = temp[i];

	for (size_t i = 0; i < size2; ++i)
		vec2[i] = temp[size1 + i];
}

//Random size vectors allowed here.
template<typename T>
void sendThreeVectors(const vector<T> &vec1, const vector<T> &vec2, const vector<T> &vec3,
					 size_t player, size_t size1, size_t size2, size_t size3)
{
	vector<T> temp(size1+size2+size3);
	for (size_t i = 0; i < size1; ++i)
		temp[i] = vec1[i];

	for (size_t i = 0; i < size2; ++i)
		temp[size1 + i] = vec2[i];

	for (size_t i = 0; i < size3; ++i)
		temp[size1 + size2 + i] = vec3[i];

	sendVector<T>(temp, player, size1+size2+size3);
}

//Random size vectors allowed here.
template<typename T>
void receiveThreeVectors(vector<T> &vec1, vector<T> &vec2, vector<T> &vec3, 
						size_t player, size_t size1, size_t size2, size_t size3)
{
	vector<T> temp(size1+size2+size3);
	receiveVector<T>(temp, player, size1+size2+size3);

	for (size_t i = 0; i < size1; ++i)
		vec1[i] = temp[i];

	for (size_t i = 0; i < size2; ++i)
		vec2[i] = temp[size1 + i];

	for (size_t i = 0; i < size3; ++i)
		vec3[i] = temp[size1 + size2 + i];
}


template<typename T>
void sendFourVectors(const vector<T> &vec1, const vector<T> &vec2, const vector<T> &vec3,
					 const vector<T> &vec4, size_t player, size_t size1, size_t size2, size_t size3, size_t size4)
{
	vector<T> temp(size1+size2+size3+size4);

	for (size_t i = 0; i < size1; ++i)
		temp[i] = vec1[i];

	for (size_t i = 0; i < size2; ++i)
		temp[size1 + i] = vec2[i];

	for (size_t i = 0; i < size3; ++i)
		temp[size1 + size2+ i] = vec3[i];	

	for (size_t i = 0; i < size4; ++i)
		temp[size1 + size2 + size3 + i] = vec4[i];	

	sendVector<T>(temp, player, size1+size2+size3+size4);
}

template<typename T>
void receiveFourVectors(vector<T> &vec1, vector<T> &vec2, vector<T> &vec3, 
						vector<T> &vec4, size_t player, size_t size1, size_t size2, size_t size3, size_t size4)
{
	vector<T> temp(size1+size2+size3+size4);
	receiveVector<T>(temp, player, size1+size2+size3+size4);

	for (size_t i = 0; i < size1; ++i)
		vec1[i] = temp[i];

	for (size_t i = 0; i < size2; ++i)
		vec2[i] = temp[size1 + i];

	for (size_t i = 0; i < size3; ++i)
		vec3[i] = temp[size1 + size2 + i];

	for (size_t i = 0; i < size4; ++i)
		vec4[i] = temp[size1 + size2 + size3 + i];

}

template<typename T>
void sendSixVectors(const vector<T> &vec1, const vector<T> &vec2, const vector<T> &vec3,
					 const vector<T> &vec4, const vector<T> &vec5, const vector<T> &vec6, 
					 size_t player, size_t size1, size_t size2, size_t size3, size_t size4, size_t size5, size_t size6)
{
	vector<T> temp(size1+size2+size3+size4+size5+size6);
	size_t offset = 0;

	for (size_t i = 0; i < size1; ++i)
		temp[i + offset] = vec1[i];

	offset += size1;
	for (size_t i = 0; i < size2; ++i)
		temp[i + offset] = vec2[i];

	offset += size2;
	for (size_t i = 0; i < size3; ++i)
		temp[i + offset] = vec3[i];

	offset += size3;
	for (size_t i = 0; i < size4; ++i)
		temp[i + offset] = vec4[i];

	offset += size4;
	for (size_t i = 0; i < size5; ++i)
		temp[i + offset] = vec5[i];

	offset += size5;
	for (size_t i = 0; i < size6; ++i)
		temp[i + offset] = vec6[i];
	
	sendVector<T>(temp, player, size1+size2+size3+size4+size5+size6);
}

template<typename T>
void receiveSixVectors(vector<T> &vec1, vector<T> &vec2, vector<T> &vec3, 
						vector<T> &vec4, vector<T> &vec5, vector<T> &vec6,
					 size_t player, size_t size1, size_t size2, size_t size3, size_t size4, size_t size5, size_t size6)
{
	vector<T> temp(size1+size2+size3+size4+size5+size6);
	size_t offset = 0;

	receiveVector<T>(temp, player, size1+size2+size3+size4+size5+size6);

	for (size_t i = 0; i < size1; ++i)
		vec1[i] = temp[i + offset];

	offset += size1;
	for (size_t i = 0; i < size2; ++i)
		vec2[i] = temp[i + offset];

	offset += size2;
	for (size_t i = 0; i < size3; ++i)
		vec3[i] = temp[i + offset];

	offset += size3;
	for (size_t i = 0; i < size4; ++i)
		vec4[i] = temp[i + offset];

	offset += size4;
	for (size_t i = 0; i < size5; ++i)
		vec5[i] = temp[i + offset];

	offset += size5;
	for (size_t i = 0; i < size6; ++i)
		vec6[i] = temp[i + offset];
}

#endif