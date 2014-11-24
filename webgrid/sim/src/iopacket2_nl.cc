#include "iopacket2_nl.hh"
#include <assert.h>
#include <iostream>
#include </usr/include/string.h>

// ***************************************************************************
// Private help functions
// ***************************************************************************


int mpi_rank () {
	int rnk;
	int err=MPI_Comm_rank(MPI_COMM_WORLD,&rnk);
	assert(err==MPI_SUCCESS);
	return rnk;
}

int mpi_size () {
	int sze;
	int err=MPI_Comm_size(MPI_COMM_WORLD,&sze);
	assert(err==MPI_SUCCESS);
	return sze;
}

const bool debug = false;


// ***************************************************************************
// ipacket2
// ***************************************************************************


ipacket2::ipacket2 (int src) 
{
	assert(src==MPI_ANY_SOURCE || (src>=0 && src<mpi_size()));

	sze=0;
	buf=0;	 
	pos=0;
        
        opacket2 cerr;

	if (debug) cerr << "# proc " << mpi_rank() << ": start recv(" << src << ")\n";

   	MPI_Status status;   
	int err1=MPI_Probe(src,0,MPI_COMM_WORLD,&status);
	assert(err1==MPI_SUCCESS);
    int err3=MPI_Get_count(&status,MPI_PACKED,&sze);
	assert(err3==MPI_SUCCESS);
	buf=new char[sze];   
   	int err2=MPI_Recv(buf,sze,MPI_PACKED,src,0,MPI_COMM_WORLD,&status);	   
	assert(err2==MPI_SUCCESS);

	if (debug) cerr << "# proc " << mpi_rank() << ": end recv(" << src << ") (" << sze <<" bytes)\n";
}


ipacket2::~ipacket2 () {
	if (buf) {
		delete[] buf;
}	}


void ipacket2::unpack (void* data, const MPI_Datatype datatype, const int outcount)
{
	int err=MPI_Unpack(buf,sze,&pos,data,outcount,datatype,MPI_COMM_WORLD);
	assert(err==MPI_SUCCESS);
}
                         

ipacket2& ipacket2::operator>> (bool& data) {
	char y=' ';	// (we initialize it to avoid compalation warning)
  	unpack(&y,MPI_CHAR);
  	data=y;	
  	return *this;
}


ipacket2& ipacket2::operator>> (char& data) {
  	unpack(&data,MPI_CHAR);
	return *this;
}


ipacket2& ipacket2::operator>> (short& data) {
  	unpack(&data,MPI_SHORT);
	return *this;
}


ipacket2& ipacket2::operator>> (int& data) {
  	unpack(&data,MPI_INT);
	return *this;
}


ipacket2& ipacket2::operator>> (long& data) {
  	unpack(&data,MPI_LONG);
	return *this;
}


ipacket2& ipacket2::operator>> (float& data) {
  	unpack(&data,MPI_FLOAT);
	return *this;
}


ipacket2& ipacket2::operator>> (double& data) {
  	unpack(&data,MPI_DOUBLE);
	return *this;
}


ipacket2& ipacket2::operator>> (char* data) {
	int len;
	unpack(&len,MPI_INT);
	unpack(data,MPI_CHAR,len);
	return *this;
}


ipacket2& ipacket2::operator>> (std::string& data) {
	int len;
	unpack(&len,MPI_INT);
	char str[len+1];
	unpack(str,MPI_CHAR,len);
	data = str;
	return *this;
}	



// *************************************************************************************
// Implementation - opacket2
// *************************************************************************************

opacket2::opacket2 (int initsze) 
{
	assert(initsze>0);

	sze=initsze;
	pos=0;
	buf=new char[sze];	
}


opacket2::~opacket2 () 
{
	delete[] buf;
}


void opacket2::send (int dst) 
{
	assert(dst>=0 && dst<mpi_size());
        opacket2 cerr;
	if (debug) cerr << std::string("# proc %i: start send(%i)\n",mpi_rank(),dst);

	int err = MPI_Send(buf,pos, MPI_PACKED, dst, 0, MPI_COMM_WORLD);	
	assert(err==MPI_SUCCESS);

	if (debug) cerr << std::string("# proc %i: end send(%i)\n",mpi_rank(),dst);
}


void opacket2::pack (const void* data, const MPI_Datatype datatype, const int incount)
{
	int elemsze;
	int err1=MPI_Pack_size(incount,datatype,MPI_COMM_WORLD,&elemsze);
 	assert(err1==MPI_SUCCESS);
        opacket2 cerr;
	while (pos+elemsze>=sze) {
		if (debug) cerr << std::string("# proc %i: enlarging opacket2 buffer\n",mpi_rank());
		sze*=2;
		char* newbuf=new char[sze];
		memcpy(newbuf,buf,pos);
		delete[] buf;
		buf=newbuf;
	}
	int err2=MPI_Pack ((void*)data,incount,datatype,buf,sze,&pos,MPI_COMM_WORLD);
 	assert(err2==MPI_SUCCESS);
}   
          

opacket2& opacket2::operator<< (bool data) {
	char y=data;
	pack(&y,MPI_CHAR);
	return *this;
}


opacket2& opacket2::operator<< (char data) {
	pack(&data,MPI_CHAR);
	return *this;
}

opacket2& opacket2::operator<< (short data) {
	pack(&data,MPI_SHORT);
	return *this;
}


opacket2& opacket2::operator<< (int data) {
	pack(&data,MPI_INT);
	return *this;
}


opacket2& opacket2::operator<< (long data) {
	pack(&data,MPI_LONG);
	return *this;
}


opacket2& opacket2::operator<< (float data) {
	pack(&data,MPI_FLOAT);
	return *this;
}


opacket2& opacket2::operator<< (double data) {
	pack(&data,MPI_DOUBLE);
	return *this;	
}


opacket2& opacket2::operator<< (const char* const data) {  
  	int len = strlen(data)+1;
	pack(&len,MPI_INT);
	pack(data,MPI_CHAR,len);
	return *this;
}


opacket2& opacket2::operator<< (const std::string& data) {
	int len = data.length();
	pack(&len,MPI_INT);
	pack(data.c_str(),MPI_CHAR,len);
	return *this;
}


