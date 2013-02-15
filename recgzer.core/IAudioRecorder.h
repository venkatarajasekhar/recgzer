namespace recgzer_core {

	class IAudioRecorder
	{
	public:
		virtual void Start() = 0;
		virtual void Stop() = 0;
	};
}
