

class RawLimits {
public:
	const long word_size;
	const long code_min;
	const long code_max;

	RawLimits();

	static void print();
	static long readKnob(const char *fname, long _default);
	static int writeKnob(const char *knob, int value);
};
