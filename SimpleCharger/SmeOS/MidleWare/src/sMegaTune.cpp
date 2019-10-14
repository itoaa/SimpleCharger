/*
 * sMegaTune.cpp
 *
 *  Created on: 28 dec. 2018
 *      Author: ola
 */

#include <sMegaTune.hpp>
#include "sSerial.hpp"

// Konstruktor av MegatuenSerial. Öppna serieporten i rätt hastighet.
sMegaTune::sMegaTune(int BaudRate)
{
	MySerial.init(9600);
    sec = 0;
}


void sMegaTune::send_EpromVar(char* pg1)
/*{
  // print all EProm variables in MegaTune style to serial port.
  // Need to match MegaTune ini file.
}*/
{
  int i;
  char *temppointer;
  vTaskSuspendAll();
  for (i=0; i < PG1S; i++)     // i = 0 till page1size
  {
	  temppointer = pg1 + i;        // temppointer pekar på nästa position i pg1pointer
	  MySerial.putChar(*temppointer);     // Skicka värdet på den positionen
  }
  xTaskResumeAll();
}

void sMegaTune::get_EpromVar(char* pg1)
{
  uint8_t pos=0;
  char* temppointer;

  pos = MySerial.getChar();						// First get the offset to byte that should be changed
  temppointer = pg1 + pos;      				// Point temppointer to correct place in page1_var (Variable pointer + offsett)
  *temppointer = MySerial.getChar(); 			// Put the value in the position that temppointer points to.
}
void sMegaTune::send_Rev(char* rev)
{
	vTaskSuspendAll();
	{
		printf("%s",rev);
    }
	xTaskResumeAll();
}
void sMegaTune::send_Sec(int sec)
{
	vTaskSuspendAll();
	{
      printf("%i",sec);
    }
	xTaskResumeAll();
}

// Subrutine to send latest value to serialport/MegaTune.
// Parameter includ all variables
void sMegaTune::send_RPage()
//void sMegaTune::send_RPage(uint8_t *RPage)

{
 // print all variables in MegaTune styel to serialport.
  // Need to mach MegaTune-ini-file.
  uint8_t i;
  char* temppointer;
  vTaskSuspendAll();
  for (i=0; i < RTPS; i++)     // i = 0 till page1size
  {
	  temppointer = (char*)&RPage + i ;      // temppointer pekar på nästa position i pg1pointer
	  MySerial.putChar(*temppointer);
  }
  xTaskResumeAll();
}

void sMegaTune::processSerial()
{
	char c = getchar();
	switch(c)                // Evaluate resived data
    {
      case 'A':
        send_RPage();			// Send real time variables as an array
        break;
      case 'c':
        send_Sec(RPage.seconds);				//	Test communications - echo back SECL (timer, 0 to 255) as a byte
        break;
      case 'C':
        send_Sec(RPage.seconds);				//	Same as "c" (Old or ver 001??)
        break;
      case 'S':
        send_Rev((char*)CodeRev);				// Send firmware version to display in title bar in TunerStudio (not in ini file??)
         break;
      case 'Q':
        send_Rev((char*)CodeString);			//	Send code string/signature. Used to determine serial protocol/device that is connected.
         break;
      case 'F':
        send_Rev((char*)"001");					//	Send serial protocol version. 001 for compatible mode (need change ??)
         break;
      case 'W':									//	"W"+<offset>+<newbyte> = receives new constant byte value and stores in 'offset' location in pg1
        get_EpromVar((char*)&pg1);
        break;
      case 'X':									//	"X"+<offset>+<count>+<newbyte>+<newbyte>.... receives series of new data bytes
        get_EpromVar((char*)&pg1);
        break;
      case 'B':									//	jump to flash burner routine and burn pg1 constant values in RAM into flash
//        storage.saveConfig(pg1);
    	  break;
      case 'V':									//	"V" = sends constants (pg1) as an array of bytes
    	  send_EpromVar((char*)&pg1);
        break;

      default: break;
    }
  }


bool sMegaTune::dataRecived()
{
	return MySerial.dataRecived();         		// Se if there is someting avalible on serialport
}

