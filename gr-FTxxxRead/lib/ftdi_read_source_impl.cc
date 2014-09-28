/* -*- c++ -*- */
/* 
 * Copyright 2014 Daniel Pelikan.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <gnuradio/thread/thread.h>

#include "ftdi_read_source_impl.h"
#include  <iostream>


namespace gr {
  namespace FTxxxRead {

  	//declaration of the static variables

  	bool ftdi_read_source_impl::exitRequested;
  	//boost::circular_buffer<unsigned char> ftdi_read_source_impl::m_RingBuffer;
    //size_t ftdi_read_source_impl::USBData_Pos;
    //size_t ftdi_read_source_impl::readData_Pos;
  //  size_t ftdi_read_source_impl::m_BufferSize;
    bool ftdi_read_source_impl::m_StartSendData;
    long int ftdi_read_source_impl::m_DataAvailable;
    struct ftdi_context *ftdi_read_source_impl::ftdi;
    FiFo ftdi_read_source_impl::m_FiFo;

  	//code

    ftdi_read_source::sptr
    ftdi_read_source::make(int VendorID, int ProductID, unsigned int BufferLength)
    {
      return gnuradio::get_initial_sptr
        (new ftdi_read_source_impl(VendorID, ProductID, BufferLength));
    }

    /*
     * The private constructor
     */
    ftdi_read_source_impl::ftdi_read_source_impl(int VendorID, int ProductID, unsigned int BufferLength)
      : gr::sync_block("ftdi_read_source",
              gr::io_signature::make(0,0,0),
              gr::io_signature::make(1, 1, sizeof(char)))
    {

    	//m_BufferSize=1000000;

    	m_FiFo.fifo_changeSize(100*1024*1024);//Set Buffer size to 100M

    	//This is the Setup of the FT232 devide
    	m_VendorID=0x0403;
    	m_Product_ID=0x6014;



    	exitRequested=false;
    	//m_RingBuffer.set_capacity(m_BufferSize);//Set the size of the ring buffer
    	//USBData_Pos=0;
    	//readData_Pos=0;
    	m_StartSendData=false;
    	m_DataAvailable=0;


    	open();

    	std::cout<<"FTxxxRead Initialized"<<std::endl;

    }


    void ftdi_read_source_impl::open(){
    	// obtain exclusive access for duration of this function
    	gr::thread::scoped_lock lock(fp_mutex);

    	   char *descstring = NULL;
    	   int err;

    	   if ((ftdi = ftdi_new()) == 0)
    	   {
    	       std::cerr<<"ftdi_new failed\n"<<std::endl;
    	       return ;
    	   }

    	   if (ftdi_set_interface(ftdi, INTERFACE_A) < 0)
    	   {
    		   std::cerr<<"ftdi_set_interface failed\n"<<std::endl;
    	       ftdi_free(ftdi);
    	       return ;
    	   }

    	   if (ftdi_usb_open_desc(ftdi, m_VendorID, m_Product_ID, descstring, NULL) < 0)// tHIS WE CAN BRING OUT AS ARGUMENTS
    	   {
    		   std::cerr<<"Can't open ftdi device: "<<ftdi_get_error_string(ftdi)<<std::endl;
    	       ftdi_free(ftdi);
    	       return ;
    	   }

    	   /* A timeout value of 1 results in may skipped blocks */
    	   if(ftdi_set_latency_timer(ftdi, 2))
    	   {
    		   std::cerr<<"Can't set latency, Error"<<ftdi_get_error_string(ftdi)<<std::endl;;
    	       ftdi_usb_close(ftdi);
    	       ftdi_free(ftdi);
    	       return ;
    	   }

    	   std::cout<<"Before Thread Start"<<std::endl;

    	   _thread = gr::thread::thread(rtlsdr_wait, this);

    	   std::cout<<"After Thread Start"<<std::endl;

/*
    	   err = ftdi_readstream(ftdi, readCallback, NULL, 8, 256);
    	    if (err < 0 && !exitRequested)
    	        exit(1);*/
    }




    void ftdi_read_source_impl::rtlsdr_wait(ftdi_read_source_impl *obj)
    {
    	std::cout<<"Before Start"<<std::endl;

      ftdi_readstream(ftdi, readCallback, NULL, 8, 256);

      std::cout<<"After Start"<<std::endl;
    }

    void ftdi_read_source_impl::close(){
     	// obtain exclusive access for duration of this function
     	gr::thread::scoped_lock lock(fp_mutex);

        if (ftdi_set_bitmode(ftdi,  0xff, BITMODE_RESET) < 0)
        {
            std::cerr<<"Can't set synchronous fifo mode, Error"<<ftdi_get_error_string(ftdi)<<std::endl;
            ftdi_usb_close(ftdi);
            ftdi_free(ftdi);
            return;
        }

        exitRequested=true;
        ftdi_usb_close(ftdi);
        ftdi_free(ftdi);
        m_FiFo.~FiFo();

     }



    /*
     * Our virtual destructor.
     */
    ftdi_read_source_impl::~ftdi_read_source_impl()
    {
    	//close file
    	std::cout<<"FTxxxRead Destructor"<<std::endl;
    	close();
    }



    //------------------------------------------
    //This part does the reading


    /*static*/ int
    ftdi_read_source_impl::readCallback(uint8_t *buffer, int length, FTDIProgressInfo *progress, void *userdata)
    {
       if (length)
       {
           if (true)//fill the buffer here
           {

        	   if(m_FiFo.fifo_write((const char*)buffer,length)!=length){
        		   std::cerr<<"Buffer Overflow"<<std::endl;
        	   }

        	   m_DataAvailable++;

        	   //if(m_FiFo.fifo_getHead()>m_FiFo.fifo_getSize()/2)m_StartSendData=true;

/*
        	   for(int i=0;i<length;i++){




        		   m_RingBuffer.push_back((unsigned char)buffer[i]);
        		   m_DataAvailable++;

        		   USBData_Pos++;
        		   //reset the USB position when we have looped through the ring buffer
        		   if(USBData_Pos>=m_BufferSize)USBData_Pos=0;

        		   if(USBData_Pos==readData_Pos){
        			   std::cerr<<"SameRead and write pos in buffer, this could mean that data gets lost! "
        					   <<m_RingBuffer.size()<<"\t"<<m_DataAvailable<<std::endl;
        		   }

        		   //Active data transfer to gnuplot when buffer is half full
        		   if(USBData_Pos>=m_BufferSize/2)m_StartSendData=true;

        	   }*/

           }
       }
       if (progress)
       {
           std::cout<< progress->totalTime<<" total time "<< progress->current.totalBytes / (1024.0 * 1024.0)
        		   <<" MiB captured "<<progress->currentRate / 1024.0
        		   <<" kB/s curr rate "<<progress->totalRate / 1024.0
        		   <<" kB/s totalrate "<<std::endl;

       }
       return exitRequested ? 1 : 0;
    }
















    int
    ftdi_read_source_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        //const <+ITYPE+> *in = (const <+ITYPE+> *) input_items[0];
        char *out = (char *) output_items[0];

        int size = noutput_items;

        int n_out_item=0;


       // std::cout<<"Inside work"<<std::endl;
        //std::cout<<"Max Size "<<m_RingBuffer.max_size()<<std::endl;


        gr::thread::scoped_lock lock(fp_mutex); // hold for the rest of this function
        // Do <+signal processing+>
/*
        while(size) {
        	out=(char *)'7';

        	size=0;
        	if(size == 0)		// done
        		break;
        }*/

        //std::cout<<"Before While Loop"<<std::endl;
        while(!m_StartSendData){//Wait until starting to send data, buffer needs to have some data first
        	//std::cout<<"Data Av: "<<m_DataAvailable<<std::endl;
        	//std::cout<<"USBData_Pos: "<<USBData_Pos<<std::endl;

        	if(m_DataAvailable<m_FiFo.fifo_getSize()/2) m_StartSendData=true;
        	std::cout<<"m_DataAvailable "<<m_DataAvailable<<std::endl;

        }

        //std::cout<<"After While Loop"<<std::endl;

       // if(noutput_items!=1)std::cout<<"ERROR noutput_items!=1 noutput is: "<<noutput_items<<std::endl;



        	for(int i = 0; i < noutput_items; i++) {
        		if(m_FiFo.fifo_read(out[i])!=1){
        			//std::cerr<<"Not enough data to read"<<std::endl;
        		}
        		n_out_item++;

        		/*
        		if(m_RingBuffer.empty()){
        			std::cerr<<"ERROR Ring Buffer is empty!!!"<<std::endl;
        		}
        		else{
                    out[i] = m_RingBuffer[readData_Pos];
                    m_DataAvailable--;
                    if(m_DataAvailable<0){
                    	std::cerr<<"There is no data available "<<m_DataAvailable<<std::endl;
                    }
                    readData_Pos++;
                    //Reset data pos when through the buffer
                    if(readData_Pos>=m_BufferSize)readData_Pos=0;
        		}*/
            }


        noutput_items=n_out_item;
        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace FTxxxRead */
} /* namespace gr */

