#ifndef VOLTAGESOURCE_H_
#define VOLTAGESOURCE_H_


class VoltageSource
// factory, client must delete. kindof a singleton too
{
	static double setpoint;

protected:
	VoltageSource(double voltage, const char* txt);
				
public:
	const char* client_txt;
	virtual ~VoltageSource();
	static void create(double voltage, const char* txt);

	static double getSetpoint() {  return setpoint; }
};



#endif /*VOLTAGESOURCE_H_*/
