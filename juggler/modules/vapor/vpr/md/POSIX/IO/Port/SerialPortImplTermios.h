/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_SERIAL_PORT_IMP_TERMIOS_H_
#define _VPR_SERIAL_PORT_IMP_TERMIOS_H_

#include <termios.h>
#include <string>
#include <vector>

#include <IO/Port/Port.h>
#include <IO/Port/SerialTypes.h>
#include <md/POSIX/FileHandleUNIX.h>


namespace vpr {

// ----------------------------------------------------------------------------
//: vpr::SerialPort implementation using termios.
// ----------------------------------------------------------------------------
class SerialPortImpTermios : public Port {
public:
    // ------------------------------------------------------------------------
    //: Constructor.  This creates a file handle object connected to the given
    //+ port name and sets the update action to happen immediately.
    //
    //! PRE: None.
    //! POST: m_handle is allocated and connected to port_name, and the update
    //+       action is set to vpr::SerialTypes::NOW.
    //
    //! ARGS: port_name - The name of the serial port that will be accessed.
    // ------------------------------------------------------------------------
    SerialPortImpTermios(const std::string& port_name);

    // ------------------------------------------------------------------------
    //: Destructor.  If the file handle is non-NULL, its memory is released.
    //
    //! PRE: None.
    //! POST: If m_handle is non-NULL, its memory is released.
    // ------------------------------------------------------------------------
    virtual ~SerialPortImpTermios(void);

    // ========================================================================
    // vpr::BlockIO overrides.
    // ========================================================================

    // ------------------------------------------------------------------------
    //: Set the open flags so that the serial port is opened in read-only
    //+ mode.
    //
    //! PRE: None.
    //! POST: The open flags are updated so that when the port is opened, it
    //+       is opened in read-only mode.  If the port is already open, this
    //+       has no effect.
    // ------------------------------------------------------------------------
    inline virtual void
    setOpenReadOnly (void) {
        m_handle->setOpenReadOnly();
    }

    // ------------------------------------------------------------------------
    //: Set the open flags so that the serial port is opened in write-only
    //: mode.
    //
    //! PRE: None.
    //! POST: The open flags are updated so that when the port is opened, it
    //+       is opened in write-only mode.  If the port is already open, this
    //+       has no effect.
    // ------------------------------------------------------------------------
    inline virtual void
    setOpenWriteOnly (void) {
        m_handle->setOpenWriteOnly();
    }

    // ------------------------------------------------------------------------
    //: Set the open flags so that the serial port is opened in read/write
    //+ mode.
    //
    //! PRE: None.
    //! POST: The open flags are updated so that when the port is opened, it
    //+       is opened in read/write mode.  If the port is already open, this
    //+       has no effect.
    // ------------------------------------------------------------------------
    inline virtual void
    setOpenReadWrite (void) {
        m_handle->setOpenReadWrite();
    }

    // ------------------------------------------------------------------------
    //: Set the blocking flags so that the serial port is opened in blocking
    //+ mode.
    //
    //! PRE: None.
    //! POST: The open flags are updated so that when the port is opened, it
    //+       is opened in blocking mode.  If the port is already open, this
    //+       has no effect.
    // ------------------------------------------------------------------------
    inline virtual void
    setOpenBlocking (void) {
        m_handle->setOpenBlocking();
    }

    // ------------------------------------------------------------------------
    //: Set the blocking flags so that the serial port is opened in
    //+ non-blocking mode.
    //
    //! PRE: None.
    //! POST: The open flags are updated so that when the port is opened, it
    //+       is opened in non-blocking mode.  If the port is already open,
    //+       this has no effect.
    // ------------------------------------------------------------------------
    inline virtual void
    setOpenNonBlocking (void) {
        m_handle->setOpenNonBlocking();
    }

    // ------------------------------------------------------------------------
    //: Open the serial port and initialize its flags.
    //
    //! PRE: The serial port is not already open.
    //! POST: An attempt is made to open the port.  If it is successful, the
    //+       port's flags are initaialized to 0.  The resulting status is
    //+       returned to the caller.  If the port is opened, m_open is set to
    //+       true.
    //
    //! RETURNS: true  - The serial port was opened successfully.
    //! RETURNS: false - The serial port could not be opened
    // ------------------------------------------------------------------------
    virtual Status open(void);

    // ------------------------------------------------------------------------
    //: Close the serial port.
    //
    //! PRE: The serial port is open.
    //! POST: An attempt is made to close the port.  The resulting status is
    //+       returned to the caller.  If the port is closed, m_open is set to
    //+       false.
    //
    //! RETURNS: true  - The serial port was closed successfully.
    //! RETURNS: false - The serial port could not be closed for some reason.
    // ------------------------------------------------------------------------
    inline virtual Status
    close (void) {
        Status retval;

        retval = m_handle->close();
        m_open = (retval.success() ? false : true);

        return retval;
    }

    // ------------------------------------------------------------------------
    //: Reconfigure the serial port so that it is in blocking mode.
    //
    //! PRE: The serial port is open.
    //! POST: Processes will block when accessing the port.
    //
    //! RETURNS: true  - The blocking mode was changed successfully.
    //! RETURNS: false - The blocking mode could not be changed for some
    //+                  reason.
    // ------------------------------------------------------------------------
    inline virtual Status
    enableBlocking (void) {
       return m_handle->enableBlocking();
    }

    // ------------------------------------------------------------------------
    //: Reconfigure the serial port so that it is in non-blocking mode.
    //
    //! PRE: The serial port is open.
    //! POST: Processes will not block when accessing the port.
    //
    //! RETURNS:  0 - The blocking mode was changed successfully.
    //! RETURNS: -1 - The blocking mode could not be changed for some reason.
    // ------------------------------------------------------------------------
    inline virtual Status
    enableNonBlocking (void) {
       return m_handle->enableNonBlocking();
    }

    // ========================================================================
    // VPR serial port interface implementation.
    // ========================================================================

    // ------------------------------------------------------------------------
    //: Get the current update action.  This tells when updates to the serial
    //+ device take effect.
    //
    //! PRE: None.
    //! POST: The current update action is returned to the caller.
    //
    //! RETURNS: A vpr::SerialTypes::UpdateActionOption value stating when
    //+          updates take effect.
    // ------------------------------------------------------------------------
    SerialTypes::UpdateActionOption getUpdateAction(void);

    // ------------------------------------------------------------------------
    //: Change the current update action to take place as described by the
    //+ given value.
    //
    //! PRE: None.
    //! POST: The update action is modified to use the new value.  The results
    //+       will be seen with the next device operation.
    //
    //! ARGS: action - The new update action value.
    // ------------------------------------------------------------------------
    void setUpdateAction(SerialTypes::UpdateActionOption action);

    // ------------------------------------------------------------------------
    //: Query the serial port for the maximum buffer size.
    //
    //! PRE: The serial port is open.
    //! POST: The maximum buffer size is returned to the caller through the
    //+       by-reference argument.
    //
    //! ARGS: size - A reference to a vpr::Uint16 where the buffer size is
    //+              stored for return to the caller.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status getBufferSize(Uint16& size);

    // ------------------------------------------------------------------------
    //: Attempt to change the buffer size to the given argument.
    //
    //! PRE: The serial port is open.
    //! POST: If the buffer size is usable on the port, the port attributes
    //+       are updated and success is returned.  Otherwise, a failure
    //+       status is returned.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status setBufferSize(const Uint8 size);

    // ------------------------------------------------------------------------
    //: Get the value of the timeout (in tenths of a second) to wait for data
    //+ to arrive.  This is only applicable in non-canonical mode.
    //
    //! PRE: The serial port is open.
    //! POST: The current timeout setting is returned to the caller in the
    //+       by-reference argument.
    //
    //! ARGS: timeout - A reference to a vpr::Uint8 to be used as storage for
    //+                 the timeout value.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    //
    //! NOTE: See page 353 of <I>Advanced Programming in the UNIX
    //+       Environment</I> for more details.
    // ------------------------------------------------------------------------
    Status getTimeout(Uint8& timeout);

    // ------------------------------------------------------------------------
    //: Set the value of the timeout to wait for data to arrive.  The value
    //+ given must be in tenths of a second.  This is only applicable in
    //+ non-canonical mode.
    //
    //! PRE: The serial port is open.
    //! POST: The timeout interval is updated to use the given value.
    //
    //! ARGS: timeout_val - The new timeout value measured in tenths of a
    //+                     second.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    //
    //! NOTE: See page 353 of <I>Advanced Programming in the UNIX
    //+       Environment</I> for more details.
    // ------------------------------------------------------------------------
    Status setTimeout(const Uint8 timeout);

    // ------------------------------------------------------------------------
    //: Get the character size (the bits per byte).
    //
    //! PRE: The serial port is open.
    //! POST: The current character size is returned to the caller in the
    //+       by-reference argument.
    //
    //! ARGS: size - A reference to a vpr::SerialTypes::CharacterSizeOption
    //+              object to be used as storage for the character size
    //+              value.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status getCharacterSize(SerialTypes::CharacterSizeOption& size);

    // ------------------------------------------------------------------------
    //: Set the current character size (the bits per byte) to the size in the
    //+ given value.  This is used for both reding and writing, and the size
    //+ does not include the parity bit (if any).
    //
    //! PRE: The serial port is open.
    //! POST: The bits per byte for the serial port is updated.
    //
    //! ARGS: bpb - The new character size.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status setCharacterSize(const SerialTypes::CharacterSizeOption bpb);

    // ------------------------------------------------------------------------
    //: Get the number of stop bits in use.  This will be either 1 or 2.
    //
    //! PRE: The serial port is open.
    //! POST: The number of stop bits currently in use is returned via the
    //+       by-reference argument.
    //
    //! ARGS: num_bits - A reference to a vpr::Uint8 that will be used to
    //+                  store the number of stop bits.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status getStopBits(Uint8& num_bits);

    // ------------------------------------------------------------------------
    //: Set the number of stop bits to use.  The value must be either 1 or 2.
    //
    //! PRE: The serial port is open, and the given number of stop bits is
    //+      either 1 or 2.
    //! POST: The number of stop bits is updated to use the given value.
    //
    //! ARGS: num_bits - The number of stop bits to use.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status setStopBits(const Uint8 num_bits);

    // ------------------------------------------------------------------------
    //: Query the canonical input state of the serial port.  If canonical mode
    //+ is enabled, the characters EOF, EOL, EOL2, ERASE, KILL, REPRINT,
    //+ STATUS, and WERASE are enabled, and the input characters are
    //+ assembled into lines.  Otherwise, read requests are satisfied directly
    //+ from the input queue, and a read will not return until the buffer is
    //+ at its minimum capacity or the timeout has expired.  See
    //+ getBufferSize() and getTimeout() for more information.
    //
    //! PRE: The serial port is open.
    //! POST: The current state of canonical input is returnd to the caller.
    //
    //! RETURNS: true  - Canonical input is enabled.
    //! RETURNS: false - Canonical input is disabled.
    // ------------------------------------------------------------------------
    bool getCanonicalState(void);

    // ------------------------------------------------------------------------
    //: Enable canonical input.  See getCanonicalState() for more information
    //+ about what this means.
    //
    //! PRE: The serial port is open.
    //! POST: Canonical input is enabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status enableCanonicalInput(void);

    // ------------------------------------------------------------------------
    //: Disable canonical input.  See getCanonicalState() for more information
    //+ about what this means.
    //
    //! PRE: The serial port is open.
    //! POST: Canonical input is disabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status disableCanonicalInput(void);

    // ------------------------------------------------------------------------
    //: Get the current state of ignoring bytes with framing errors (other
    //+ than a BREAK) or parity errors.
    //
    //! PRE: The serial port is open.
    //! POST: The erroneous byte ignore state is returned to the caller.
    //
    //! RETURNS: true  - Bad bytes are ignored.
    //! RETURNS: false - Bad bytes are not ignored.
    // ------------------------------------------------------------------------
    bool getBadByteIgnoreState(void);

    // ------------------------------------------------------------------------
    //: Enable ignoring of received bytes with framing errors or parity
    //+ errors.
    //
    //! PRE: The serial port is open.
    //! POST: Erroneous byte ignoring is enabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status enableBadByteIgnore(void);

    // ------------------------------------------------------------------------
    //: Disable ignoring of received bytes with framing errors or parity
    //+ errors.
    //
    //! PRE: The serial port is open.
    //! POST: Erroneous byte ignoring is disabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status disableBadByteIgnore(void);

    // ------------------------------------------------------------------------
    //: Get the state of parity checking for input.
    //
    //! PRE: The serial port is open.
    //! POST: The state of input parity checking is returned to the caller.
    //
    //! RETURNS: true  - Input parity checking is enabled.
    //! RETURNS: false - Input parity checking is disabled.
    // ------------------------------------------------------------------------
    bool getInputParityCheckState(void);

    // ------------------------------------------------------------------------
    //: Enable input parity checking.
    //
    //! PRE: The serial port is open.
    //! POST: Input parity checking is enabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status enableInputParityCheck(void);

    // ------------------------------------------------------------------------
    //: Disable input parity checking.
    //
    //! PRE: The serial port is open.
    //! POST: Input parity checking is disabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status disableInputParityCheck(void);

    // ------------------------------------------------------------------------
    //: Get the current state of bit stripping.  When enabled, input bytes are
    //+ stripped to seven bits.  Otherwise, all eight bits are processed.
    //
    //! PRE: The serial port is open.
    //! POST: The current bit stripping state is returned to the caller.
    //
    //! RETURNS: true  - Input bytes are stripped to seven bits.
    //! RETURNS: false - Input bytes are not stripped.
    // ------------------------------------------------------------------------
    bool getBitStripState(void);

    // ------------------------------------------------------------------------
    //: Enable stripping of input bytes to seven bits.
    //
    //! PRE: The serial port is open.
    //! POST: Input bit stripping is enabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status enableBitStripping(void);

    // ------------------------------------------------------------------------
    //: Disable stripping of input bytes to seven bits.
    //
    //! PRE: The serial port is open.
    //! POST: Input bit stripping is disabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status disableBitStripping(void);

    // ------------------------------------------------------------------------
    //: Get the state of start-stop input control.  When enabled, if the
    //+ terminal driver sees that the receive buffer is getting full, a STOP
    //+ command is sent to the serial device.  The device should respond by
    //+ halting its data flow.  When the input queue has been processed, a
    //+ START command is sent to the serial device which should cause it to
    //+ resume putting bytes onto the input queue.
    //
    //! PRE: The serial port is open.
    //! POST: The current state of start-stop input control is returned to the
    //+       caller.
    //
    //! RETURNS: true  - Start-stop input control is enabled.
    //! RETURNS: true  - Start-stop input control is not enabled.
    // ------------------------------------------------------------------------
    bool getStartStopInputState(void);

    // ------------------------------------------------------------------------
    //: Enable start-stop input control.  See getStartStopInputState() for
    //+ more information.
    //
    //! PRE: The serial port is open.
    //! POST: Start-stop input control is enabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status enableStartStopInput(void);

    // ------------------------------------------------------------------------
    //: Disable start-stop input control.  See getStartStopInputState() for
    //+ more information.
    //
    //! PRE: The serial port is open.
    //! POST: Start-stop input control is disabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status disableStartStopInput(void);

    // ------------------------------------------------------------------------
    //: Get the state of start-stop output control.  When enabled, when the
    //+ terminal driver receives a STOP command, output stops.  When it
    //+ receives a START command, output resumes.  If disabled, these
    //+ commands are read as normal characters.
    //
    //! PRE: The serial port is open.
    //! POST: The current state of start-stop output control is returned to
    //+       the caller.
    //
    //! RETURNS: true  - Start-stop output control is enabled.
    //! RETURNS: true  - Start-stop output control is not enabled.
    // ------------------------------------------------------------------------
    bool getStartStopOutputState(void);

    // ------------------------------------------------------------------------
    //: Enable start-stop input control.  See getStartStopInputState() for
    //+ more information.
    //
    //! PRE: The serial port is open.
    //! POST: Start-stop input control is ensabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status enableStartStopOutput(void);

    // ------------------------------------------------------------------------
    //: Disable start-stop output control.  See getStartStopOutputState() for
    //+ more information.
    //
    //! PRE: The serial port is open.
    //! POST: Start-stop output control is disabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status disableStartStopOutput(void);

    // ------------------------------------------------------------------------
    //: Get the current state of parity generation for outgoing bytes and
    //+ parity checking for incoming bytes.
    //
    //! PRE: The serial port is open.
    //! POST: The current parity generation/checking state is returned to the
    //+       caller.
    //
    //! RETURNS: true  - Parity generation and checking are enabled for
    //+                  outgoing and incoming bytes respectively.
    //! RETURNS: false - Parity generation and checking are disabled.
    // ------------------------------------------------------------------------
    bool getParityGenerationState(void);

    // ------------------------------------------------------------------------
    //: Enable parity generation for outgoing bytes and parity checking for
    //+ incoming bytes.
    //
    //! PRE: The serial port is open.
    //! POST: Parity generation and checking are enabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status enableParityGeneration(void);

    // ------------------------------------------------------------------------
    //: Disable parity generation for outgoing bytes and parity checking for
    //+ incoming bytes.
    //
    //! PRE: The serial port is open.
    //! POST: Parity generation and checking are disabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status disableParityGeneration(void);

    // ------------------------------------------------------------------------
    //: Enable marking of bytes with parity errors or framing errors (except
    //+ BREAKs).  This is only active if input parity and framing error
    //+ reporting is enabled (see getInputParityCheckState() for more
    //+ information).  The mark is the three-byte sequence \377 \0 X where X
    //+ is the byte received in error.  If bit stripping is enabled, a valid
    //+ \377 byte is passed as the two-byte sequence \377 \377.
    //
    //! PRE: The serial port is open.
    //! POST: Bytes with an error are marked and passed on to the reader.
    //
    //! RETURNS: true  - Parity error marking is enabled.
    //! RETURNS: false - Parity error marking is disabled.
    // ------------------------------------------------------------------------
    bool getParityErrorMarkingState(void);

    // ------------------------------------------------------------------------
    //: Enable parity error and framing error marking.
    //
    //! PRE: The serial port is open.
    //! POST: Parity error marking is enabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status enableParityErrorMarking(void);

    // ------------------------------------------------------------------------
    //: Disable parity error and framing error marking.
    //
    //! PRE: The serial port is open.
    //! POST: Parity error marking is disabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status disableParityErrorMarking(void);

    // ------------------------------------------------------------------------
    //: Get the current parity checking type (either odd or even).
    //
    //! PRE: The serial port is open.
    //! POST: The parity type is returned to the caller.
    //
    //! RETURNS: A vpr::SerialTypes::ParityType value giving the parity
    //+          checking being done.
    // ------------------------------------------------------------------------
    SerialTypes::ParityType getParity(void);

    // ------------------------------------------------------------------------
    //: Enable odd parity.
    //
    //! PRE: The serial port is open.
    //! POST: Odd parity is enabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status setOddParity(void);

    // ------------------------------------------------------------------------
    //: Enable even parity.
    //
    //! PRE: The serial port is open.
    //! POST: Even parity is enabled.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status setEvenParity(void);

    // ------------------------------------------------------------------------
    //: Get the current input baud rate.
    //
    //! PRE: The serial port is open.
    //! POST: The current input baud rate is returned to the caller via the
    //+       by-reference parameter.
    //
    //! ARGS: rate - A reference to a vpr::Uint32 to be used as storage for
    //+              the current input baud rate.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status getInputBaudRate(Uint32& rate);

    // ------------------------------------------------------------------------
    //: Set the current input baud rate.
    //
    //! PRE: The serial port is open, and the rate is a valid baud rate.
    //! POST: The input baud rate is updated to use the given value.
    //
    //! ARGS: rate - The new input baud rate.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status setInputBaudRate(const Uint32 rate);

    // ------------------------------------------------------------------------
    //: Get the current output baud rate.
    //
    //! PRE: The serial port is open.
    //! POST: The current output baud rate is returned to the caller via the
    //+       by-reference parameter.
    //
    //! ARGS: rate - A reference to a vpr::Uint32 to be used as storage for
    //+              the current output baud rate.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status getOutputBaudRate(Uint32& rate);

    // ------------------------------------------------------------------------
    //: Set the current output baud rate.
    //
    //! PRE: The serial port is open, and the rate is a valid baud rate.
    //! POST: The output baud rate is updated to use the given value.
    //
    //! ARGS: rate - The new output baud rate.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status setOutputBaudRate(const Uint32 rate);

    // ------------------------------------------------------------------------
    //: Wait for all output to be transmitted.
    //
    //! PRE: The serial port is open.
    //! POST: All output is transmitted.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status drainOutput(void);

    // ------------------------------------------------------------------------
    //: Alter the input or output flow control.  Based on the
    //+ vpr::SerialTypes::FlowActionOption argument, output can be suspended
    //+ and restarted or the terminal device can be told to stop or to resume
    //+ sending data.
    //
    //! PRE: The serial device is open.
    //! POST: Based on the argument value, the input or output flow control is
    //+       updated.
    //
    //! ARGS: opt - The flow control change.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status controlFlow(SerialTypes::FlowActionOption opt);

    // ------------------------------------------------------------------------
    //: Discard either the input buffer (unread data received from the
    //+ terminal device) or the output buffer (data written but not yet
    //+ transmitted to the terminal device).  The argument tells which queue
    //+ (or queues) to flush.
    //
    //! PRE: The serial device is open.
    //! POST: Based on the argument, the appropriate queue is flushed of its
    //+       current contents.
    //
    //! ARGS: queue - The queue (or queues) to be flushed.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status flushQueue(SerialTypes::FlushQueueOption queue);

    // ------------------------------------------------------------------------
    //: Transmit a continuous stream of zero bits for the given duration.  If
    //+ the argument is 0, the transmission will last between 0.25 and 0.5
    //+ seconds.  Otherwise, the duration specfies the number of seconds to
    //+ send the zero bit stream.
    //
    //! PRE: The serial device is open.
    //! POST: A stream of zero bits are sent to the serial device for the
    //+       given duration.
    //
    //! ARGS: duration - The length of time to send a zero bit stream to the
    //+                  serial device.
    //
    //! RETURNS: A vpr::Status object describing the results of the operation.
    // ------------------------------------------------------------------------
    Status sendBreak(const Int32 duration);

protected:
    // Symbolic names for the four types of flags that can be set in the
    // termios struct.
    enum _term_flag {
        IFLAG,		// Input flag
        OFLAG,		// Output flag
        CFLAG,		// Control flag
        LFLAG		// Local flag
    };

    // ------------------------------------------------------------------------
    //: Set the control character at the given index to the given value.
    // ------------------------------------------------------------------------
    void setControlCharacter(const Uint32 index, const Uint8 value);

    // ------------------------------------------------------------------------
    //: Get the value of the control character at the given index.
    // ------------------------------------------------------------------------
    Uint8 getControlCharacter(const Uint32 index);

    // ------------------------------------------------------------------------
    //! Read up to the specified number of bytes from the serial port into the
    //+ given buffer.
    //
    //! PRE: The port's file handle is valid, and the buffer is at least
    //+      length bytes long.
    //! POST: The given buffer has length bytes copied into it from the serial
    //+       port, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer where the data read from the
    //+                port is to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the serial
    //+                port.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    virtual Status
    read_i (void* buffer, const size_t length, ssize_t& bytes_read) {
        return m_handle->read(buffer, length, bytes_read);
    }

    // ------------------------------------------------------------------------
    //: Read exactly the specified number of bytes from the serial port into
    //+ the given buffer.
    //
    //! PRE: The port's file handle is valid, and the buffer is at least
    //+      length bytes long.
    //! POST: The given buffer has length bytes copied into it from the serial
    //+       port, and the number of bytes read successfully is returned to
    //+       the caller.
    //
    //! ARGS: buffer - A pointer to the buffer where the port's buffer
    //+                contents are to be stored.
    //! ARGS: length - The number of bytes to be read.
    //
    //! RETURNS: >-1 - The number of bytes successfully read from the serial
    //+                port.
    //! RETURNS:  -1 - An error occurred when reading.
    // ------------------------------------------------------------------------
    virtual Status
    readn_i (void* buffer, const size_t length, ssize_t& bytes_read) {
        return m_handle->readn(buffer, length, bytes_read);
    }

    // ------------------------------------------------------------------------
    //: Write the buffer to the serial port.
    //
    //! PRE: The port's file handle is valid.
    //! POST: The given buffer is written to the serial port, and the number
    //+       of bytes written successfully is returned to the caller.
    //
    //! ARGS: buffer - A pointer to the buffer to be written.
    //! ARGS: length - The length of the buffer.
    //
    //! RETURNS: >-1 - The number of bytes successfully written to the serail
    //+                port.
    //! RETURNS:  -1 - An error occurred when writing.
    // ------------------------------------------------------------------------
    virtual Status
    write_i (const void* buffer, const size_t length, ssize_t& bytes_written) {
        return m_handle->write(buffer, length, bytes_written);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status getAttrs(struct termios* term);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status setAttrs(struct termios* term, const char* err_msg,
                    const bool print_sys_err = true);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status setAttrs(struct termios* term, const std::string& err_msg,
                    const bool print_sys_err = true);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    bool getBit(const tcflag_t bit, _term_flag flag);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    Status setBit(const tcflag_t bit, _term_flag flag, const bool enable,
                  const std::string& err_msg, const bool print_sys_err = true);

    // ------------------------------------------------------------------------
    //: Convert a termios baud rate to its corresponding integer value.
    // ------------------------------------------------------------------------
    Uint32 baudToInt(const speed_t baud_rate);

    // ------------------------------------------------------------------------
    //: Convert an integer baud rate to the corresponding termios rate
    //+ constant.
    // ------------------------------------------------------------------------
    speed_t intToBaud(const Uint32 speed_int);

    FileHandleUNIX*	m_handle;    //: File handle for the serial port
    int			m_actions;
};

}; // End of vpr namespace


#endif	/* _VPR_SERIAL_PORT_IMP_TERMIOS_H_ */
