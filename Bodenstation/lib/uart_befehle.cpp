/* This codes provides the basics for using the UART of a Raspberry */

// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

// SETUP
int serial_port = open("/dev/ttyUSB0", O_RDWR); // research path and exchange it 

// Check for errors
if (serial_port < 0) {
    printf("Error %i from open: %s\n", errno, strerror(errno));
}

// Create new termios struct, we call it 'tty' for convention
// No need for "= {0}" at the end as we'll immediately write the existing
// config to this struct
struct termios tty;

// Read in existing settings, and handle any error
// NOTE: This is important! POSIX states that the struct passed to tcsetattr()
// must have been initialized with a call to tcgetattr() overwise behaviour
// is undefined
if(tcgetattr(serial_port, &tty) != 0) {
    printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
}

// c_cflags
tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)

tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)

tty.c_cflag &= ~CSIZE; // Clear all the size bits, then use one of the statements below
tty.c_cflag |= CS8; // 8 bits per byte (most common)

tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)

tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

// c_flags
tty.c_lflag &= ~ICANON;

tty.c_lflag &= ~ECHO; // Disable echo
tty.c_lflag &= ~ECHOE; // Disable erasure
tty.c_lflag &= ~ECHONL; // Disable new-line echo

tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP

// c_iflag
tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

// c_oflag
tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

tty.c_cc[VTIME] = 1;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
tty.c_cc[VMIN] = 32;    // max. 40 byte payload

// Set in/out baud rate to be 115200
cfsetispeed(&tty, B115200);
cfsetospeed(&tty, B115200);

// Save tty settings, also checking for error
if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
    printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
}

//WRTING
unsigned char msg[] = { 'H', 'e', 'l', 'l', 'o', '\r' };
write(serial_port, msg, sizeof(msg));

//READING
// Allocate memory for read buffer, set size according to your needs
char read_buf [256];

// Read bytes. The behaviour of read() (e.g. does it block?,
// how long does it block for?) depends on the configuration
// settings above, specifically VMIN and VTIME
int n = read(serial_port, &read_buf, sizeof(read_buf));

// n is the number of bytes read. n may be 0 if no bytes were received, and can also be negative to signal an error.

//CLOSING
close(serial_port);