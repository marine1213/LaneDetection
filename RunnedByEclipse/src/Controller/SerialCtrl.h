/*
 * SerialCtrl.h
 *
 *  Created on: Oct 26, 2016
 *      Author: gnouchnam
 */

#ifndef SERIALCTRL_H_
#define SERIALCTRL_H_

#include <fcntl.h>  /* File Control Definitions          */
#include <termios.h>/* POSIX Terminal Control Definitions*/
#include <unistd.h> /* UNIX Standard Definitions         */
#include <errno.h>  /* ERROR Number Definitions          */
#include <iostream>
#include <string.h>

using namespace std;

#define	device	"/dev/ttyUSB0" //dmesg | grep tty --> to check available port

class SerialCtrl {
private:
	//============= wireless parameters ===============
	struct termios serialPrtst;
	bool rfAbility;
	int  fd;
	int  bytes_read,bytes_written;
	//=================================================

	/*
	 * check RF ability
	 */
	bool checkRfAbility() {
		rfAbility = true;
		if ((fd = open(device, O_WRONLY | O_NOCTTY)) < 0)
			rfAbility = false;
		cout<<"RF descriptor: "<<fd<<endl;
		return rfAbility;
	}

	void settingUpRf(){
		tcgetattr(fd, &serialPrtst);

		//Configure Baudrate
		cfsetispeed(&serialPrtst, B9600);
		cfsetospeed(&serialPrtst, B9600);

		//Configure data bits
		serialPrtst.c_cflag &= ~PARENB;   			// No Parity
		serialPrtst.c_cflag &= ~CSTOPB; 			// Stop bits = 1

		serialPrtst.c_cflag &= ~CSIZE; 				/* Clears the Mask       */
		serialPrtst.c_cflag |=  CS8;   				/* Set the data bits = 8 */

		serialPrtst.c_cflag &= ~CRTSCTS;			// Turn off hardware based flow control (RTS/CTS)
		serialPrtst.c_cflag |= CREAD | CLOCAL; 		// Turn on the receiver

		//	Turn off software based flow control (XON/XOFF).
		serialPrtst.c_iflag &= ~(IXON | IXOFF | IXANY);

		/* Setting Time outs for reading*/
		serialPrtst.c_cc[VMIN]  = 10; 				/* Read 10 characters */
		serialPrtst.c_cc[VTIME] = 0;  				/* Wait indefinitely   */

		// NON Cannonical mode is recommended.
		serialPrtst.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);

		tcsetattr(fd,TCSANOW,&serialPrtst);
		//cout<< "setup RF parameter finished" << endl;
	}

public:
	SerialCtrl() {
		//cout<<"serial ctrl constructor"<<endl;
		checkRfAbility();
		settingUpRf();
	}

	~SerialCtrl() {
		cout<<"Serial Destructor"<<endl;
		close(fd);
	}


	void writeData(const char *data){
		int  len = strlen(data);
		bytes_written = write(fd,data, len);

		cout<<"Sent:"<<data<<endl;
		if(bytes_written != len){
			cout<<"byte written = "<< bytes_written<<endl;
			cout<<"size of data = "<< len<<endl;
		}
	}

	char* readData(int buffSize = 16){
		char *read_buffer = new char[buffSize];
		bytes_read = 0;

		bytes_read = read(fd, read_buffer,buffSize);
		return read_buffer;
	}

	/*
	 * return RF ability result
	 */
	bool getRfAbility() {
		return rfAbility;
	}

	int getBytesRead() {
		return bytes_read;
	}

	int getBytesWritten() {
		return bytes_written;
	}


};

#endif /* SERIALCTRL_H_ */
