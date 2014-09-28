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

#ifndef INCLUDED_FTXXXREAD_FTDI_READ_SOURCE_IMPL_H
#define INCLUDED_FTXXXREAD_FTDI_READ_SOURCE_IMPL_H

#include <FTxxxRead/ftdi_read_source.h>
#include <boost/circular_buffer.hpp>

#include  <ftdi.h>

#include "FiFo.h"

namespace gr {
  namespace FTxxxRead {

    class ftdi_read_source_impl : public ftdi_read_source
    {
     private:
    	boost::mutex fp_mutex;

    	static struct ftdi_context *ftdi;
    	static bool exitRequested;
    	//declare the ring buffer
    	static boost::circular_buffer<unsigned char> m_RingBuffer;
    	//static size_t USBData_Pos;
    	//static size_t readData_Pos;
    //	static size_t m_BufferSize;
    	static bool m_StartSendData;

    	static long int m_DataAvailable;

    	static FiFo m_FiFo;

    	gr::thread::thread _thread;

    	int m_VendorID;
    	int m_Product_ID;

     public:
      ftdi_read_source_impl(int VendorID, int ProductID, unsigned int BufferLength);
      ~ftdi_read_source_impl();

      static int readCallback(uint8_t *buffer, int length, FTDIProgressInfo *progress, void *userdata);

      static void rtlsdr_wait(ftdi_read_source_impl *obj);



      void open();
      void close();

      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace FTxxxRead
} // namespace gr



#endif /* INCLUDED_FTXXXREAD_FTDI_READ_SOURCE_IMPL_H */

