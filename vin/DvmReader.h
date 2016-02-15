#ifndef DVMREADER_H_
#define DVMREADER_H_


class DvmReader
// factory, client must delete
{
	protected:
		DvmReader() {}		
		DvmReader(double *voltage, const char* txt);
				
	public:
		virtual ~DvmReader();
		static DvmReader* create(double *voltage, const char* txt);			
};



#endif /*DVMREADER_H_*/
