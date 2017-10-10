template <class data_t, uint8_t BUF_SIZE>
class ButterWorthFilter:public Filter<data_t, BUF_SIZE> {
private:
	constexpr static data_t tau = 0.0025f;
	constexpr static data_t fc = 2.8f;
public:
	ButterWorthFilter(void) {}
	virtual ~ButterWorthFilter() {}
	data_t update(data_t raw);
};
template <class data_t, uint8_t BUF_SIZE>
data_t ButterWorthFilter<data_t, BUF_SIZE>::update(data_t raw) {
	data_t Omg, tOmg, ss, cs, a1, a2, b1;
	data_t theta = M_PI / 4.0;

	//update old value
	this->buf_update(this->dst);

	Omg = 2.0 * M_PI * fc;
	tOmg = Omg * tau;

	ss = tOmg * sin(theta);
	cs = tOmg * cos(theta);

	b1 = tOmg * exp(-ss)*sin(cs)/cos(theta);
	a1 = 2.0 * exp(-ss)*cos(cs);
	a2 = -exp(-2.0*ss);

	this->dst = b1*raw + a1*this->samples[0] + a2*this->samples[1];
	return this->dst;
}
