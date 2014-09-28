/*
 * FiFo.h
 *
 *  Created on: Aug 10, 2014
 *      Author: imp
 */

#ifndef FIFO_H_
#define FIFO_H_


const size_t BufferSize=1024*1024*100;


struct fifo_t {
     char * buf;
     int head;
     int tail;
     int size;
};



class FiFo {
private:



	struct fifo_t m_myFIFO;
	char *m_Buffer;

public:

	FiFo(size_t Size=52428800){
		m_Buffer= new char[Size];
		fifo_init(m_Buffer, Size);
	};

	~FiFo(){
		delete m_Buffer;
	};

	void fifo_init(char * buf, int size);
	int fifo_read(char &a);
	int fifo_write(const char * buf, int nbytes);
	int fifo_getHead(){return m_myFIFO.head;}
	int fifo_getSize(){return m_myFIFO.size;}
	void fifo_changeSize(size_t size);


};

void FiFo::fifo_changeSize(size_t size){
	if(m_Buffer!=NULL){
		delete m_Buffer;
	}
	m_Buffer= new char[size];
	fifo_init(m_Buffer, size);

}

//This initializes the FIFO structure with the given buffer and size
void FiFo::fifo_init(char * buf, int size){
	m_myFIFO.head = 0;
	m_myFIFO.tail = 0;
	m_myFIFO.size = size;
	m_myFIFO.buf = buf;
}


//Read one byte from fifo
int FiFo::fifo_read(char &a){

      if( m_myFIFO.tail != m_myFIFO.head ){ //see if any data is available
           a = m_myFIFO.buf[m_myFIFO.tail];  //grab a byte from the buffer
           m_myFIFO.tail++;  //increment the tail
           if( m_myFIFO.tail == m_myFIFO.size ){  //check for wrap-around
        	   m_myFIFO.tail = 0;
           }
      } else {
           return 0; //number of bytes read
      }
      return 1;

      //returns 0 when no byte was read
      //returns 1 when byte was read

}

//This writes up to nbytes bytes to the FIFO
//If the head runs in to the tail, not all bytes are written
//The number of bytes written is returned
int FiFo::fifo_write(const char * buf, int nbytes){
     int i;
     const char * p;
     p = buf;
     for(i=0; i < nbytes; i++){
           //first check to see if there is space in the buffer
           if( (m_myFIFO.head + 1 == m_myFIFO.tail) ||
                ( (m_myFIFO.head + 1 == m_myFIFO.size) && (m_myFIFO.tail == 0) )){
                 return i; //no more room
           } else {
        	   m_myFIFO.buf[m_myFIFO.head] = *p++;
        	   m_myFIFO.head++;  //increment the head
               if( m_myFIFO.head == m_myFIFO.size ){  //check for wrap-around
            	   m_myFIFO.head = 0;
               }
           }
     }
     return nbytes;
}







#endif /* FIFO_H_ */
