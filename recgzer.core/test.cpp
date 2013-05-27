#include "AudioDevice.h"

#include "ComInit.h"

using namespace recgzer_core;

int main()
{
	(FAILED(S_OK) ? throw ComException(E_FAIL) : 0);

	ComInit com;

	AudioDevice device = AudioDevice::DefaultAudioDevice();

	AudioDeviceId id = device.Id();
	AudioDeviceId id2 = device.Id();

	//bool bb = id == id2;

	std::wstring name = device.Name();
	float volume = device.PeakAudioLevel();

	//if (FAILED(S_OK)) throw ComException(E_FAIL);

	


	std::unique_ptr<IAudioRecorder> recorder = device.Recorder(AudioRecorderType::Wave);

	// MEMORY STREAM....
	// format and properties determined by the prototype for AcustId
	//
	//
	recorder->Start();

	recorder->Stop();
	//recorder->Close();
	recorder->Stream()->Size();

	// FILE STREAM
	// 
	// write to file, as Wav......
	//  
	//
	// BufferedWaveStream
	//
	// the stream caps:....
	//
	// Read(pos, length)
	// Length
	// Save
	// Close


	// buffered recorder
	// record-------------------------------------------
	// ------packet --------- packet --------packet ----
	// ---------feed ------------feed ----------feed ----
	// file recorder // test only 
	// +stop,  async??  
	// +start, async??
	//
	// ------------------------------
	// IRecorder<IStream> // configuration.... bitrate, encoding......
	// ------------------------------
	// +Start() : void
	// +Stop() : void
	// +Stream() : IStream<PacketType>&
	//    +Length() : size_t
	//    +Read(position, len) : PacketType[] // how large?
	//
	//
	//  check the IFileStream design
	//
	//  EH..  line numbers
	//
	//  
	//
	//


	// FE
	//
	// recgzer -> recognize(async callback) ---> start -----------------------------o
	//  ------------------------------------------- [ ---> cancel -- ] --o
	// ---------------------------------------------------------> complete ---------o
	// ------------------------------------------------------------------> track info
	//
	//
	
	return 0;
}