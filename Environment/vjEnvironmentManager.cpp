// vjEnvironmentManager.C
// implementation of Environment Manager
//
// author: Christopher Just


#include <vjConfig.h>
#include <strings.h>    // For bzero()
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <Environment/vjEnvironmentManager.h>
#include <Kernel/vjKernel.h>
#include <Config/vjChunkDescDB.h>
#include <Config/vjConfigChunkDB.h>
#include <Environment/vjTimedUpdate.h>


vjEnvironmentManager::vjEnvironmentManager():
                          connections(),
                          updaters(),
                          perf_buffers() {

    /* I want some hardcoded defaults, yes? */
    Port = 4450;
    listen_thread = NULL;
    listen_socket = -1;
    activated = false;
    configured_to_accept = false;

    perf_buffer_reader = new vjTuPerfBufReader(NULL, 500);
    updaters.push_back (perf_buffer_reader);

}



vjEnvironmentManager::~vjEnvironmentManager() {
    deactivate();
}



void vjEnvironmentManager::activate() {

    /* start updaters */
    for (int i = 0; i < updaters.size(); i++) {
	updaters[i]->startProcess();
    }

    activated = true;
    if (configured_to_accept) {
	acceptConnections();
    }
    else
	vjDEBUG(0) << "NOTE: EM not configured to accept connections!!!\n" << vjDEBUG_FLUSH;
}



void vjEnvironmentManager::deactivate() {
    activated = false;
    rejectConnections();
    killConnections();

    /* kill updaters - this needs work */
    for (int i = 0; i < updaters.size(); i++) {
	updaters[i]->stopProcess();
    }
}



bool vjEnvironmentManager::acceptConnections() {
    struct sockaddr_in sockaddress;

    if (!activated)
	return false;
    if (listen_thread != NULL)
	return true;

    /* here, we open a socket & get ready to read connections */
    listen_socket = socket (AF_INET, SOCK_STREAM, 0);
    bzero(&sockaddress, sizeof (struct sockaddr_in));
    sockaddress.sin_family = PF_INET;
    sockaddress.sin_port = htons(Port);

    if (bind ( listen_socket, (sockaddr*)&sockaddress,
	       sizeof (struct sockaddr_in))) {
	vjDEBUG(0) << "vjEnvironmentManager couldn't open socket\n"
		   << vjDEBUG_FLUSH;
	return false;
    }
    else
	vjDEBUG(0) << "vjEnvironmentManager accepting connections on port "
		   << Port << '\n' << vjDEBUG_FLUSH;

    /* now we ought to spin off a thread to do the listening */
    vjThreadMemberFunctor<vjEnvironmentManager>* memberFunctor =
	new vjThreadMemberFunctor<vjEnvironmentManager>(
				this,
				&vjEnvironmentManager::controlLoop,
	       			NULL);
    listen_thread = new vjThread (memberFunctor, 0);

    return (listen_thread != NULL);
}



bool vjEnvironmentManager::rejectConnections () {
    if (listen_thread) {
	listen_thread->kill();
	listen_thread = NULL;
	close(listen_socket);
    }

    return 1;
}



void vjEnvironmentManager::killConnections() {
    int i;

    for (i = 0; i < connections.size(); i++) {
	connections[i]->stopProcess();
	delete (connections[i]);
    }
    connections.erase (connections.begin(), connections.end());
}



bool vjEnvironmentManager::isAccepting() {
    return (listen_thread != NULL);
}



void vjEnvironmentManager::addPerfDataBuffer (vjPerfDataBuffer *v) {
    perf_buffers.push_back(v);
    if (configuredToActivate(v)) {
	v->activate();
	perf_buffer_reader->addBuffer(v);
    }
}


void vjEnvironmentManager::removePerfDataBuffer (vjPerfDataBuffer *b) {
    std::vector<vjPerfDataBuffer*>::iterator it;

    b->deactivate();

    // this is one of those things I really hate:
    for (it = perf_buffers.begin(); it != perf_buffers.end(); it++) {
        if (*it == b) {
            perf_buffers.erase(it);
            break;
        }
    }

    perf_buffer_reader->removeBuffer(b);
}



vjConnect* vjEnvironmentManager::getConnect (std::string s) {
    for (int i = 0; i < connections.size(); i++)
	if (s == connections[i]->name)
	    return connections[i];
    return NULL;
}



void vjEnvironmentManager::controlLoop (void* nullParam) {
    // Child process used to listen for new network connections
    struct sockaddr_in servaddr;
    int servsock;
    int len;
    vjConnect* connection;

    vjDEBUG(3) << "vjEnvironmentManager network server running.\n"
	       << vjDEBUG_FLUSH;

    /* start listening for connections */
    if (listen (listen_socket, 0)) {
	vjDEBUG(1) << "ERROR: vjEnvironmentManager socket listen "
		   << "failed\n" << vjDEBUG_FLUSH;
	return;
    }

    for (;;) {
	len = sizeof (struct sockaddr_in);
	servsock = accept (listen_socket,
			   (sockaddr*)&servaddr, &len);
	connection = new vjConnect (servsock);
	connections.push_back( connection );
	connection->startProcess();
    }
}






void vjEnvironmentManager::sendRefresh() {
    for (int i = 0; i < connections.size(); i++) {
	connections[i]->sendRefresh();
    }

}


void vjEnvironmentManager::activatePerfBuffersWithPrefix (std::string prefix) {
    // activates all perf buffers whose names start with prefix
    vjPerfDataBuffer* b;
    int i;

    for (i = 0; i < perf_buffers.size(); i++) {
	b = perf_buffers[i];
	std::string n = b->getName();
	if (!n.compare (0, prefix.size(), prefix)) {
	    perf_buffer_reader->addBuffer(b);
	}
    }
}


void vjEnvironmentManager::deactivatePerfBuffersWithPrefix (std::string prefix) {
    vjPerfDataBuffer* b;
    int i;

    for (i = 0; i < perf_buffers.size(); i++) {
	b = perf_buffers[i];
	// this below is correct!!! returns false when strings match, true else
	// just like strcmp
	std::string n = b->getName();
	if (!n.compare (0, prefix.size(), prefix)) {
	    perf_buffer_reader->removeBuffer(b);
	}
    }
}


//: ConfigChunkHandler stuff
//! PRE: configCanHandle(chunk) == true
//! RETURNS: success
bool vjEnvironmentManager::configAdd(vjConfigChunk* chunk) { 
    bool networkingchanged;
    int newport;

    cout << "EM configAdd recv'd chunk:\n" << *chunk << endl;
    std::string s = chunk->getType();
    if (s == "EnvironmentManager") {
	configured_to_accept = chunk->getProperty ("AcceptConnections");
	newport = chunk->getProperty("Port");

	if (newport == 0)
	    newport = Port;
	if (newport != Port || configured_to_accept != isAccepting())
	    networkingchanged = 1;
	std::string s = chunk->getProperty ("PerformanceTarget");
	perf_buffer_reader->setTarget(getConnect(s));

	if (networkingchanged) {
	    Port = newport;
	    if (isAccepting())
		rejectConnections();
	    if (configured_to_accept)
		acceptConnections();
	    else
		killConnections();
	}
	return true;
    }
    else if (s == "PerfMeasure") {
	vjConfigChunk* c2;
	for (int i = 0; i < chunk->getNum ("TimingTests"); i++) {
	    c2 = chunk->getProperty ("TimingTests", i);
	    if ((bool)c2->getProperty ("Enabled") == true) {
		activatePerfBuffersWithPrefix ((std::string)c2->getProperty ("Prefix"));
	    }
	    else {
		deactivatePerfBuffersWithPrefix ((std::string)c2->getProperty ("Prefix"));
	    }
	}
	sendRefresh();
	return true;
    }
    else if (s == "FileConnect") {
	vjConnect* vn = new vjConnect (chunk);
	connections.push_back (vn);
	vn->startProcess();
	return true;
    }
    vjDEBUG(1) << "EnvironmentManager::configAdd - Unrecognized Chunk " + s << endl 
	       << vjDEBUG_FLUSH;
    return false; 
}

//: Remove the chunk from the current configuration
//! PRE: configCanHandle(chunk) == true
//!RETURNS: success
bool vjEnvironmentManager::configRemove(vjConfigChunk* chunk)
{ return false; }
    
//: Can the handler handle the given chunk?
//! RETURNS: true - Can handle it
//+          false - Can't handle it
bool vjEnvironmentManager::configCanHandle(vjConfigChunk* chunk) {
    std::string s = chunk->getType();
    if (s == "EnvironmentManager")
	return true;
    if (s == "PerfMeasure")
	return true;
    if (s == "FileConnect")
	return true;
    return false;
}


// should this buffer be active under our current config?
// used when a buffer is added after configure
bool vjEnvironmentManager::configuredToActivate (vjPerfDataBuffer* b) {
    return false;
}

