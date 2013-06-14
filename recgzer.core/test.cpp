#include "AudioDevice.h"

#include "ComInit.h"

using namespace recgzer_core;

int main()
{
	ComInit com;

	AudioDevice device = AudioDevice::DefaultAudioDevice();

	AudioDeviceId id = device.Id();
	std::wstring name = device.Name();
	float volume = device.PeakAudioLevel();
	//
	// get the sessions
	//          .. pid
	//          .. recorder <
	//     . ... . state 

	// encoding policy, intermediate policy, storage policy
	// .... PCM, float...
	//                   ... memory stream, disk backed..
	//                                        .. . wave, mp3, etc.
	std::unique_ptr<WaveAudioRecorder> recorder = device.Recorder(AudioRecorderType::Pcm16Float);

	recorder->Start();
	recorder->Length();
	recorder->Stop();
	
	
	
	
	
	
	
	
	
	recorder->Initialize();

	//recorder->i

	// MEMORY STREAM....
	// format and properties determined by the prototype for AcustId
	//
	//
	//recorder->Start();

	//recorder->Stop();
	////recorder->Close();
	//recorder->Stream()->Size();

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
	// +Stream() : IStream&
	//    +Length() : size_t
	//    +Read(pos, n) : PacketType[] // how large?
	//    + 
	//
	// IStream
	//    -Write(packet)
	//    -
	//
	//  check the IFileStream design
	//
	//  EH..  line numbers
	//
	//  IAudioRecorder<Pcm16FloatRecorder> recorder = device.Recorder(AudioRecorderType::Pcm16Float);
	//   -or-
	//  IAudioRecorder recorder = device.Recorder(AudioRecorderType::Pcm16Float);
	//   -or- 
	//  IAudioRecorder recorder = device.Recorder();
	//  recorder.Start(/*encoding*/);
	//  recorder.Stop();
	//
	//
	//
	//
	//
	//
	// encoding.... store it as a policy
	//
	// IRecorder<EncodingPolicy>
	//
	//
	//
	// device.Recorder<Pcm16Float>()
	// -or-
	// Pcm16Float encoding;
	// IRecorder recorder = device.Recorder(encoding);



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