template <class data_t, uint8_t BUF_SIZE>
class SteadyKalmanFilter:public Filter<data_t, BUF_SIZE> {
public:
	SteadyKalmanFilter():P(0.0) {};

	data_t update(data_t raw);

private:
	data_t P, K;
	data_t Q = 0.0001;
	data_t R = 0.01;
	void measurementUpdate()
	{
		K = (P + Q) / (P + Q + R);
		P = R * (P + Q) / (R + P + Q);
	}
};

template <class data_t, uint8_t BUF_SIZE>
data_t SteadyKalmanFilter<data_t, BUF_SIZE>::update(data_t raw) {
		measurementUpdate();
		this->dst = this->dst + (raw - this->dst) * K;
		return this->dst;
}

