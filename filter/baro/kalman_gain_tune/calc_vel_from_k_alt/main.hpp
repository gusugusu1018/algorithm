class SimpleKalman
{
	double P, K;
	double X;
public:
	double Q;// = 0.0001;
	double R;// = 0.01;
	SimpleKalman()
		:P(0.0), X(0.0)
		 {};

	double initialize(double first_value)
	{
		X = first_value;
	}

	double update(double measurement)
	{
		measurementUpdate();
		double result = X + (measurement - X) * K;
		X = result;
		return result;
	}

private:
	void measurementUpdate()
	{
		K = (P + Q) / (P + Q + R);
		P = R * (P + Q) / (R + P + Q);
	}
};


class SimpleButterWorth
{
	constexpr static float tau = 0.0025f;
	constexpr static float fc = 1.0f;
	float old1,old2;
	float dst;
   public:
	SimpleButterWorth() {
	}
	float initialize(float first_value) {
		old1 = first_value;
		old2 = first_value;
		dst = first_value;
		return dst;
	}
	float update(float src) {
		float Omg, tOmg, ss, cs, a1, a2, b1;
		float theta = M_PI / 4.0;

		//update old value
		old2 = old1;
		old1 = dst;

		Omg = 2.0 * M_PI * fc;
		tOmg = Omg * tau;

		ss = tOmg * sin(theta);
		cs = tOmg * cos(theta);

		b1 = tOmg * exp(-ss)*sin(cs)/cos(theta);
		a1 = 2.0 * exp(-ss)*cos(cs);
		a2 = -exp(-2.0*ss);

		dst = b1*src + a1*old1 + a2*old2;
		return dst;
	}
};

