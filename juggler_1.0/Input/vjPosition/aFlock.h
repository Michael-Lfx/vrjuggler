/////////////////////////////////////////////////////////////////////////
// aFlock.h
//
// Flock of birds tracking class
//
////////////////////////////////////////////////////////////////////////
#ifndef _ASCENSION_FLOCKOFBIRD_H_
#define _ASCENSION_FLOCKOFBIRD_H_

// look into this, it may be a KLUDGE!!!
// it allows this driver to work under 6.5 (6.2 worked fine)
#define _OLD_TERMIOS

#define POSITION_RANGE 12.0f
#define ANGLE_RANGE   180.0f
#define MAX_SENSORS    128

#ifndef _BOOL
typedef bool int;
#define true 1
#define false 0
#endif

typedef struct {
        float xmin, ymin, zmin;
        float xmax, ymax, zmax;
        float xloc[15][10][15],yloc[15][10][15],zloc[15][10][15];
} CalStruct;

enum BIRD_HEMI  {FRONT_HEM, AFT_HEM, UPPER_HEM, LOWER_HEM, LEFT_HEM, RIGHT_HEM};
enum BIRD_FILT  {AC_NARROW, AC_WIDE, DC_FILTER};

//: class for running a Flock of Birds.
//  aFlock is a positional device driver for the Flock of Birds <br>
//!PUBLIC_API
class aFlock
{
public:
	//: Configure Constructor
	// Give:                                                 <BR>
	//   port - such as "/dev/ttyd3"                         <BR>
	//   baud - such as 38400, 19200, 9600, 14400, etc...    <BR>
	//   sync - sync type.                                   <BR>
	//   block - blocking                                    <BR>
	//   numBrds - number of birds in flock,                 <BR>
	//   transmit - transmitter unit number,                 <BR>
	//   hemi - hemisphere to track from,                    <BR>
	//   filt - filtering type,                              <BR>
	//   report -                                            <BR>
	//   calfile - a calibration file, if "", then use none. <BR>
	//                                                       <BR>
	// Result: configures internal data members, 
	//         doesn't actually talk to the FOB yet.
	aFlock(const char* const port = "/dev/ttyd3", 
		const int& baud = 38400, 
		const int& sync = 1, 
		const int& block = 0, 
		const int& numBrds = 3, 
		const int& transmit = 3, 
		const BIRD_HEMI& hemi = LOWER_HEM, 
		const BIRD_FILT& filt = AC_NARROW, 
		const char& report = 'R', 
		const char* const calfile = "");
	
	
	//: Destructor
	~aFlock();

	
	//: call this to connect to the flock device.
	//  NOTE: flock.isActive() must be false to use this function
	int	    start();
	
	//: stop the flock
	int	    stop();
	
	//: call this repeatedly to update the data from the birds.
	//  NOTE: flock.isActive() must be true to use this function
	int	    sample();
		
		
	//: set the port to use
	//  this will be a string in the form of the native OS descriptor <BR>
	//  ex: unix - "/dev/ttyd3", win32 - "COM3" <BR>
	//  NOTE: flock.isActive() must be false to use this function
	void	    setPort( const char* const serialPort );
	
	//: get the port used
	//  this will be a string in the form of the native OS descriptor <BR>
	//  ex: unix - "/dev/ttyd3", win32 - "COM3"
	const char* const getPort();
	
	
	//: set the baud rate
	//  this is generally 38400, consult flock manual for other rates. <BR>
	//  NOTE: flock.isActive() must be false to use this function
	void	    setBaudRate( const int& baud );
	
	//: get the baud rate
	//  this is generally 38400, consult flock manual for other rates. <BR>
	inline int  getBaudRate() { return _baud;}
   
	//: Set the unit number of the transmitter
	//  give - an integer that is the same as the dip switch 
	//         setting on the transmitter box (for the unit number) <BR>
	//  NOTE: flock.isActive() must be false to use this function
	void	    setTransmitter( const int& Transmit );
	
	//: Get the unit number of the transmitter
	//  returns - an integer that is the same as the dip switch 
	//         setting on the transmitter box (for the unit number) <BR>
	inline int  getTransmitter() { return _xmitterUnitNumber; }


	//: Set the number of birds to use in the flock.
	//  give - an integer number not more than the number of 
	//         birds attached to the system <BR>
	//  NOTE: flock.isActive() must be false to use this function
	void	    setNumBirds( const int& n );
	
	//: Get the number of birds to use in the flock.
	//  returns - an integer number not more than the number of 
	//         birds attached to the system <BR>
	inline int  getNumBirds() { return _numBirds; }


	//: set the video sync type
	//  this option allows the Flock to syncronize its pulses with 
	//  your video display.  This will eliminate most flicker caused 
	//  by the magnetic distortion. <BR>
	//  - Refer to your flock manual for what number to use.
	//  NOTE: flock.isActive() must be false to use this function
	void	    setSync( const int& sync );
	
	//: Get the video sync type
	//  this option allows the Flock to syncronize its pulses with 
	//  your video display.  This will eliminate most flicker caused 
	//  by the magnetic distortion. <BR>
	//  - Refer to your flock manual for what the return number means
	inline int  getSync() {return _syncStyle; }


	//: Set blocking of flock
	//  see flock manual for details.
	//  NOTE: flock.isActive() must be false to use this function
	void	    setBlocking( const int& blVal );
	
	//: Get blocking flock's blocking type
	//  see flock manual for details.
	inline int  getBlocking() { return _blocking; }


	//: Set the type of filtering that the flock uses
	//  NOTE: flock.isActive() must be false to use this function
	void	    setFilters( const BIRD_FILT& f );
	
	//: Set the type of filtering that the flock uses
	inline int  getFilters() { return _filter; }


	//: Set the hemisphere that the transmitter transmits from.
	//  NOTE: flock.isActive() must be false to use this function
	void	    setHemisphere( const BIRD_HEMI& h );
	
	//: Set the hemisphere that the transmitter transmits from.
	inline int  getHemisphere() {return _hemisphere; }


	//: Set the report rate that the flock uses
	//  NOTE: flock.isActive() must be false to use this function
	void	    setReportRate( const char& rRate );
        
	//: Set the report rate that the flock uses
	inline char getReportRate() {return _reportRate; }


	float&	    xPos( const int& i );
	float&	    yPos( const int& i );
	float&	    zPos( const int& i );
	float&	    zRot( const int& i );
	float&	    yRot( const int& i );
	float&	    xRot( const int& i );

// Private methods
private:
	//: with the calibration table info, correct a given position
	//  give - a position in x, y, z euclidian coordinates
	//  returns - a corrected position in x, y, z euclidian coordinates
	void        positionCorrect( float& x, float& y, float& z );
	
	//: init the correction table from a file
	//  give - a file name of the calibration file.
	//  result - initializes the correction table with the file's info
	void        initCorrectionTable( const char* const );

// Private data members
private:
	CalStruct   caltable;
	char        _reportRate;
	BIRD_HEMI   _hemisphere;
	BIRD_FILT   _filter;
	
	static const int   MAXCHARSTRINGSIZE;
	char	    _port[256];
	char	    _calibrationFileName[256];
	
	int	    _portId;
	int	    _baud;
	int	    _syncStyle;
	int	    _blocking;
	int	    _numBirds;
	int	    _xmitterUnitNumber;
	bool	    _usingCorrectionTable;
	
	//    x,y,z,        r,y,p
	float _position[MAX_SENSORS][3], _orientation[MAX_SENSORS][3];
	
	bool _active;
	
// static bird functions.
private:	
	//: get a reading
	//  give - n:    the bird unit number  <BR>
	//  give - port: the flock port number <BR>
	//  give - xyz positions               <BR>
	//  give - zyx rotations
	static int   getReading( const int& n, const int& port, 
				float& xPos, float& yPos, float& zPos, 
				float& zRot, float& yRot, float& xRot );
	static float rawToFloat(char& r1, char& r2);
	static void  pickBird(const int& sensor, const int& port_id);
	static int   open_port(const char* const serialPort, const int& baud, int& port_id );
	static void  set_blocking(const int& port, const int& blocking);
	static void  set_sync(const int& port, const int& sync);
	static void  set_hemisphere(const int& port, const BIRD_HEMI& hem, const int& transmitter, const int& numbirds);
	static void  set_rep_and_stream(const int& port, const char& repRate);
	static void  set_pos_angles(const int& port, const int& transmitter, const int& numbirds);
	static void  set_filter(const int& port, const BIRD_FILT& filter);
	static void  set_transmitter(const int& port, const int& transmitter);
	static void  set_autoconfig(const int& port, const int& numbirds);
	static void  set_group(const int& port);
};



#endif

	
