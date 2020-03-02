/*
 * sMegaTune.cpp
 *
 *  Created on: 28 dec. 2018
 *      Author: ola
 */

#include <sMegaTune.hpp>

// Konstruktor av MegatuenSerial. Öppna serieporten i rätt hastighet.
sMegaTune::sMegaTune(int BaudRate)
{
	MySerial.init(115);
    sec = 0;
}

void sMegaTune::send_EpromVar()
/*{
  // print all EProm variables in MegaTune style to serial port.
  // Need to match MegaTune ini file.
}*/
{
	int i;
	for (i=0; i < PG1S; i++)     // i = 0 till page1size
	{
		MySerial.putChar(GlobalDB.getpg1Data(i));     // Skicka värdet på den positionen
	}
}

void sMegaTune::get_EpromVar()
{
	uint8_t pos;
	pos = MySerial.getChar();						// First get the offset to byte that should be changed
	GlobalDB.setpg1Data(pos,MySerial.getChar());    // Put the value in the right position in GlobalDB.
}

void sMegaTune::send_Rev(char* rev)
{
	printf("%s",rev);
}
void sMegaTune::send_Sec(int sec)
{
	printf("%i",sec);
}

void sMegaTune::send_RPage()
{
	// Subroutine to send latest value to serial port/MegaTune.
	// Parameter include all variables in MegaTune format
	// Need to match MegaTune ini-file.
	uint8_t i;
	for (i=0; i < RTPS; i++)     					// i = 0 till page1size
	{
		MySerial.putChar(GlobalDB.getrtpData(i));	// Send value on the specified position
	}
}

void sMegaTune::processSerial()
{
	char c = getchar();
	switch(c)										// Evaluate received data
    {
      case 'A':
        send_RPage();								// Send real time variables as an array
        break;
      case 'c':
        send_Sec(GlobalDB.rtPage.seconds);			//	Test communications - echo back SECL (timer, 0 to 255) as a byte
        break;
      case 'C':
    	  send_Sec(GlobalDB.rtPage.seconds);		//	Same as "c" (Old or ver 001??)
        break;
      case 'S':
        send_Rev((char*)CodeRev);					// Send firmware version to display in title bar in TunerStudio (not in ini file??)
         break;
      case 'Q':
        send_Rev((char*)CodeString);				//	Send code string/signature. Used to determine serial protocol/device that is connected.
         break;
      case 'F':
        send_Rev((char*)"001");						//	Send serial protocol version. 001 for compatible mode (need change ??)
         break;
      case 'W':										//	"W"+<offset>+<newbyte> = receives new constant byte value and stores in 'offset' location in pg1
        get_EpromVar();
        break;
      case 'X':										//	"X"+<offset>+<count>+<newbyte>+<newbyte>.... receives series of new data bytes
        break;
      case 'B':										//	jump to flash burner routine and burn pg1 constant values in RAM into flash
    	  GlobalDB.savePG1();						//  Save (burn) pg1 to EEPROM
    	  break;
      case 'V':										//	"V" = sends constants (pg1) as an array of bytes
    	  send_EpromVar();
        break;

      default: break;
    }
  }

bool sMegaTune::dataRecived()
{
	return MySerial.dataRecived();         			// See if there is something available on serial port
}
