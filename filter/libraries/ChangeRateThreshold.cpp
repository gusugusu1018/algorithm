template <class data_t, uint8_t BUF_SIZE>
class ChangeRateThresholdFilter:public Filter<data_t, BUF_SIZE> {
private:
	data_t _threshold = 1;
public:
	ChangeRateThresholdFilter(void) {}
	virtual ~ChangeRateThresholdFilter() {}
	data_t update(data_t raw);
	void set_threshold(data_t threshold) { _threshold = threshold;}
};
template <class data_t, uint8_t BUF_SIZE>
data_t ChangeRateThresholdFilter<data_t, BUF_SIZE>::update(data_t raw) {
	//update old value
	this->buf_update(this->dst);
	if (raw > this->samples[0] + _threshold) {
		this->dst = this->samples[0] + _threshold;
	} else if (raw < this->samples[0] - _threshold) {
		this->dst = this->samples[0] - _threshold;
	} else {
		this->dst = raw;
	}
	return this->dst;
}
