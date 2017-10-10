template <class data_t, uint8_t BUF_SIZE>
class Filter {
protected:
	data_t samples[BUF_SIZE];
	data_t dst;
public:
	Filter(void) {}
	virtual ~Filter(void) {}
	virtual void initialize(data_t raw);
	virtual void buf_update(data_t now);
};

template <class data_t, uint8_t BUF_SIZE>
void Filter<data_t, BUF_SIZE>::initialize(data_t raw) {
	for (int i=0;i<BUF_SIZE;i++) {
		samples[i] = raw;
	}
}

template <class data_t, uint8_t BUF_SIZE>
void Filter<data_t, BUF_SIZE>::buf_update(data_t now) {
	/*
	BUF_SIZE = 2;
	old2 = old1;
	old1 = now;
	*/
	for (int i=BUF_SIZE-1;i>0;i--) {
		samples[i] = samples[i-1];
	}
	samples[0] = now;
}
