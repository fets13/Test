#ifndef _ESP_MEM_H_
#define _ESP_MEM_H_

//FETS	#include <Config.h>          //https://github.com/tzapu/WiFiManager
#include <c_types.h>          //https://github.com/tzapu/WiFiManager

#define RTCMEMBEGIN	68
#define MAGICBYTE	85

typedef enum {BOOT_NORMAL, BOOT_CONFIG, BOOT_CONFIG_TIMEOUT } eBootMode ;

class ESPMem {
public:
	ESPMem () ;
	virtual ~ESPMem () ;

	void Print () ;

	void WriteBoardMode (eBootMode mode) ;

	void InitWifi () ;

	void LoopWifiManager () ;

protected:
	bool	ReadMem () ;

	void WriteMem() ;

	eBootMode ReadBoardMode () ;

	virtual int	OffsetMode () ;

private:

	typedef struct {
	  uint8 markerFlag;
	  int bootTimes;
	} RtcMemDef __attribute__((aligned(4)));

	RtcMemDef	mRtcMem ;
	eBootMode		mBoardMode ;  // type de boot : normal (N) config (C)

} ;
#endif // _ESP_MEM_H_
