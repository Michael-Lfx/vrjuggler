//===============================================================
// vjFlock (a Wrapper for aFlock)
//
// Purpose:
//      VR Juggler Ascention Flock of birds tracking class
//
// Author:
//	Kevin Meinert
//
// Date: 4-22-99
//===============================================================
#include <vjConfig.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <Input/vjPosition/aFlock.h>
#include <Input/vjPosition/vjFlock.h>
#include <Math/vjCoord.h>

#include <Kernel/vjDebug.h>


// Helper to return the index for theData array
// given the birdNum we are dealing with and the bufferIndex
// to get
//! ARGS: birdNum - The number of the bird we care about
//! ARGS:bufferIndex - the value of current, progress, or valid (it is an offset in the array)
// XXX: We are going to say the birds are 1 based
int vjFlock::getBirdIndex(int birdNum, int bufferIndex)
{
   int ret_val = (birdNum*3)+bufferIndex;
   vjASSERT((ret_val >= 0) && (ret_val < ((getNumBirds()+1)*3)));
   //assertIndexes();   // Can't assert here because it is possible the indexes are switching right now
   return ret_val;
}

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
//   report - flock report rate.                         <BR>
//   calfile - a calibration file, if "", then use none. <BR>
//                                                       <BR>
// Result: configures internal data members,
//         doesn't actually talk to the FOB yet.
vjFlock::vjFlock(const char* const port,
		const int& baud,
		const int& sync,
		const int& block,
		const int& numBrds,
		const int& transmit,
		const BIRD_HEMI& hemi,
		const BIRD_FILT& filt,
		const char& report,
		const char* const calfile) : mFlockOfBirds(port,
						    baud,
						    sync,
						    block,
						    numBrds,
						    transmit,
						    hemi,
						    filt,
						    report,
						    calfile)
{
}

bool vjFlock::config(vjConfigChunk *c)
{
   port_id = -1;
   myThread = NULL;

   vjDEBUG(vjDBG_ALL,0) << "	 vjFlock::vjFlock(vjConfigChunk*)" << endl << vjDEBUG_FLUSH;

   // read in vjPosition's config stuff,
   // --> this will be the port and baud fields
   if (!vjPosition::config(c))
      return false;

   // keep aFlock's port and baud members in sync with vjInput's port and baud members.
   mFlockOfBirds.setPort( vjInput::GetPort() );
   mFlockOfBirds.setBaudRate( vjInput::GetBaudRate() );

   // set mFlockOfBirds with the config info.
   mFlockOfBirds.setSync( static_cast<int>(c->getProperty("sync")) );
   mFlockOfBirds.setBlocking( static_cast<int>(c->getProperty("blocking")) );
   mFlockOfBirds.setNumBirds( static_cast<int>(c->getProperty("num")) );
   mFlockOfBirds.setTransmitter( static_cast<int>(c->getProperty("transmitter")) );
   mFlockOfBirds.setHemisphere( (BIRD_HEMI)(static_cast<int>(c->getProperty("hemi"))) ); //LOWER_HEMI
   mFlockOfBirds.setFilterType( (BIRD_FILT)(static_cast<int>(c->getProperty("filt"))) ); //

   // sanity check the report rate
   char r = static_cast<std::string>(c->getProperty("report")).c_str()[0];
   if ((r != 'Q') && (r != 'R') &&
       (r != 'S') && (r != 'T'))
   {
      vjDEBUG(vjDBG_ALL,0)  << "   illegal report rate from configChunk, defaulting to every other cycle (R)" << endl << vjDEBUG_FLUSH;
      mFlockOfBirds.setReportRate( 'R' );
   }
   else
      mFlockOfBirds.setReportRate( r );

   // output what was read.
   vjDEBUG(vjDBG_ALL,0) << "	  Flock Settings: " << endl
   << "	        aFlock::getTransmitter(): " << mFlockOfBirds.getTransmitter() << endl
   << "             aFlock::getNumBirds()      : " << mFlockOfBirds.getNumBirds() << endl
   << "	        aFlock::getBaudRate()      : " << mFlockOfBirds.getBaudRate() << endl
   << "	        deviceAbilities:" << deviceAbilities << endl
   << "	        aFlock::getPort()         : " << mFlockOfBirds.getPort() << endl
   << "		instance name : " << instName << endl
   << endl << vjDEBUG_FLUSH;

   // init the correction table with the calibration file.
   mFlockOfBirds.initCorrectionTable( ((std::string)c->getProperty("calfile")).c_str() );

   return true;
}

vjFlock::~vjFlock()
{
    vjDEBUG(vjDBG_ALL,0)  << "	vjFlock::~vjFlock()" << endl << vjDEBUG_FLUSH;
    this->StopSampling();
    if (theData != NULL)
       getMyMemPool()->deallocate((void*)theData);
    if (mDataTimes != NULL)
	delete mDataTimes;
}

static void SampleBirds(void* pointer)
{
   vjDEBUG(vjDBG_ALL,0) << "vjFlock: Spawned SampleBirds starting" << endl << vjDEBUG_FLUSH;

   vjFlock* devPointer = (vjFlock*) pointer;
   for (;;)
   {
      devPointer->Sample();
   }
}

int vjFlock::StartSampling()
{
   // make sure birds aren't already started
   if (this->isActive() == true)
   {
      vjDEBUG(vjDBG_ALL,0)  << "vjFlock was already started." << endl << vjDEBUG_FLUSH;
      return 0;
   }

   if (myThread == NULL)
   {
      if (theData != NULL)
         getMyMemPool()->deallocate((void*)theData);
      if (mDataTimes != NULL)
	  delete mDataTimes;

      // XXX: What is the +1 for
      // +1 is because birds are 1 based, not zero based
      int numbuffs = (mFlockOfBirds.getNumBirds()+1)*3;
      theData = (vjMatrix*) new vjMatrix[numbuffs];
      mDataTimes = new vjTimeStamp[numbuffs];

      // Reset current, progress, and valid
      resetIndexes();

      vjDEBUG(vjDBG_ALL,0) << "    Getting flock ready....\n" << vjDEBUG_FLUSH;


      mFlockOfBirds.start();

      //sanity check.. make sure birds actually started
      if (this->isActive() == false)
      {
         vjDEBUG(vjDBG_ALL,0)  << "vjFlock failed to start.." << endl << vjDEBUG_FLUSH;
         return 0;
      }

      vjDEBUG(vjDBG_ALL,0)  << "vjFlock ready to go.." << endl << vjDEBUG_FLUSH;

      vjFlock* devicePtr = this;

      myThread = new vjThread(SampleBirds, (void*) devicePtr, 0);

      if ( myThread == NULL )
      {
         return 0;  // Fail
      }
      else
      {
         return 1;   // success
      }
   }
   else
      return 0; // already sampling
}

int vjFlock::Sample()
{
   if (this->isActive() == false)
      return 0;

   int i;
   //int tmp;

   vjTimeStamp sampletime;
   sampletime.set();
   mFlockOfBirds.sample();

   //: XXX: +1 for the transmitter???
   for (i=1; i < (mFlockOfBirds.getNumBirds()+1); i++)
   {
      if (i == mFlockOfBirds.getTransmitter())
         continue;

      // int index = progress*(mFlockOfBirds.getNumBirds()+1)+i-1;
      int index = getBirdIndex(i,progress);

      // Sets index to current read buffer


      theData[index].makeZYXEuler(mFlockOfBirds.zRot( i ),
                                  mFlockOfBirds.yRot( i ),
                                  mFlockOfBirds.xRot( i ));



      /*
      theData[index].makeXYZEuler(mFlockOfBirds.xRot( i ),
                                  mFlockOfBirds.yRot( i ),
                                  mFlockOfBirds.zRot( i ));
                                  */


      theData[index].setTrans(mFlockOfBirds.xPos( i ),
                              mFlockOfBirds.yPos( i ),
                              mFlockOfBirds.zPos( i ));
      mDataTimes[index] = sampletime;

      //if (i==1)
         //vjDEBUG(vjDBG_ALL,2) << "Flock: bird1:    orig:" << vjCoord(theData[index]).pos << endl << vjDEBUG_FLUSH;


      // Transforms between the cord frames
      // See transform documentation and VR System pg 146
      // Since we want the reciver in the world system, Rw
      // wTr = wTt*tTr
      vjMatrix world_T_transmitter, transmitter_T_reciever, world_T_reciever;

      world_T_transmitter = xformMat;                    // Set transmitter offset from local info
      transmitter_T_reciever = theData[index];           // Get reciever data from sampled data
      world_T_reciever.mult(world_T_transmitter, transmitter_T_reciever);   // compute total transform
      theData[index] = world_T_reciever;                                     // Store corrected xform back into data

      //if (i==1)
         //vjDEBUG(vjDBG_ALL,2) << "Flock: bird1: xformed:" << vjCoord(theData[index]).pos << endl << vjDEBUG_FLUSH;
   }

   // Locks and then swaps the indices
   swapValidIndexes();

   return 1;
}

int vjFlock::StopSampling()
{
   if (this->isActive() == false)
      return 0;

   if (myThread != NULL)
   {
      vjDEBUG(vjDBG_ALL,0) << "Stopping the flock thread..." << vjDEBUG_FLUSH;

      myThread->kill();
      delete myThread;
      myThread = NULL;

      vjDEBUG(vjDBG_ALL,0) << "  Stopping the flock..." << vjDEBUG_FLUSH;

      mFlockOfBirds.stop();

      // sanity check: did the flock actually stop?
      if (this->isActive() == true)
      {
         vjDEBUG(vjDBG_ALL,0) << "Flock didn't stop." << endl << vjDEBUG_FLUSH;
         return 0;
      }

      vjDEBUG(vjDBG_ALL,0) << "stopped." << endl << vjDEBUG_FLUSH;
   }

   return 1;
}

vjMatrix* vjFlock::GetPosData( int d ) // d is 0 based
{
    if (this->isActive() == false)
	return NULL;
	
    return (&theData[getBirdIndex(d,current)]);
}

vjTimeStamp* vjFlock::getPosUpdateTime (int d) {
    if (this->isActive() == false)
	   return NULL;
	
    return (&mDataTimes[getBirdIndex(d,current)]);
}

void vjFlock::UpdateData()
{
   int new_index, old_index, tmp;

   if (this->isActive() == false)
      return;

   lock.acquire();
      // For each bird, ensure that we never swap an old value (stored in
      // valid) over a newer value (presently in current).
      for ( int i = 1; i <= mFlockOfBirds.getNumBirds(); i++ ) {
         if (i == mFlockOfBirds.getTransmitter())
            continue;

         new_index = getBirdIndex(i, valid);
         old_index = getBirdIndex(i, current);
         theData[old_index] = theData[new_index];
      }

      // This is copied from vjInput::swapCurrentIndexes().
      tmp = current;
      current = valid;
      valid = tmp;
   lock.release();

   return;
}

void vjFlock::setHemisphere(const BIRD_HEMI& h)
{
    if (this->isActive())
    {
	vjDEBUG(vjDBG_ALL,0) << "Cannot change the hemisphere while active\n" << vjDEBUG_FLUSH;
	return;
    }
    mFlockOfBirds.setHemisphere( h );
}

void vjFlock::setFilterType(const BIRD_FILT& f)
{
  if (this->isActive())
  {
      vjDEBUG(vjDBG_ALL,0) << "Cannot change filters while active\n" << vjDEBUG_FLUSH;
      return;
  }
  mFlockOfBirds.setFilterType( f );
}

void vjFlock::setReportRate(const char& rRate)
{
  if (this->isActive())
  {
      vjDEBUG(vjDBG_ALL,0) << "Cannot change report rate while active\n" << vjDEBUG_FLUSH;
      return;
  }
  mFlockOfBirds.setReportRate( rRate );
}

void vjFlock::setTransmitter(const int& Transmit)
{
  if (this->isActive())
  {
      vjDEBUG(vjDBG_ALL,0) << "Cannot change transmitter while active\n" << vjDEBUG_FLUSH;
      return;
  }
  mFlockOfBirds.setTransmitter( Transmit );
}
void vjFlock::setNumBirds(const int& n)
{
  if (this->isActive())
  {
      vjDEBUG(vjDBG_ALL,0) << "Cannot change num birds while active\n" << vjDEBUG_FLUSH;
      return;
  }
  mFlockOfBirds.setNumBirds( n );
}
void vjFlock::setSync(const int& sync)
{
  if (this->isActive())
  {
      vjDEBUG(vjDBG_ALL,0) << "Cannot change report rate while active\n" << vjDEBUG_FLUSH;
      return;
  }
  mFlockOfBirds.setSync( sync );
}

void vjFlock::setBlocking(const int& blVal)
{
  if (this->isActive())
  {
      vjDEBUG(vjDBG_ALL,0) << "Cannot change report rate while active\n" << vjDEBUG_FLUSH;
      return;
  }
  mFlockOfBirds.setBlocking( blVal );
}

//: set the port to use
//  this will be a string in the form of the native OS descriptor <BR>
//  ex: unix - "/dev/ttyd3", win32 - "COM3" <BR>
//  NOTE: flock.isActive() must be false to use this function
void vjFlock::setPort( const char* const serialPort )
{
    if (this->isActive())
    {
      vjDEBUG(vjDBG_ALL,0) << "Cannot change port while active\n" << vjDEBUG_FLUSH;
      return;
    }
    mFlockOfBirds.setPort( serialPort );

    // keep vjInput's port and baud members in sync
    // with aFlock's port and baud members.
    vjInput::SetPort( serialPort );
}

//: set the baud rate
//  this is generally 38400, consult flock manual for other rates. <BR>
//  NOTE: flock.isActive() must be false to use this function
void vjFlock::setBaudRate( const int& baud )
{
    if (this->isActive())
    {
      vjDEBUG(vjDBG_ALL,0) << "Cannot change baud rate while active\n" << vjDEBUG_FLUSH;
      return;
    }
    mFlockOfBirds.setBaudRate( baud );

    // keep vjInput's port and baud members in sync
    // with aFlock's port and baud members.
    vjInput::SetBaudRate( baud );
}
