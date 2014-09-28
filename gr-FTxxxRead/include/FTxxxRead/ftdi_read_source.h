/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
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


#ifndef INCLUDED_FTXXXREAD_FTDI_READ_SOURCE_H
#define INCLUDED_FTXXXREAD_FTDI_READ_SOURCE_H

#include <FTxxxRead/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace FTxxxRead {

    /*!
     * \brief <+description of block+>
     * \ingroup FTxxxRead
     *
     */
    class FTXXXREAD_API ftdi_read_source : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<ftdi_read_source> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of FTxxxRead::ftdi_read_source.
       *
       * To avoid accidental use of raw pointers, FTxxxRead::ftdi_read_source's
       * constructor is in a private implementation
       * class. FTxxxRead::ftdi_read_source::make is the public interface for
       * creating new instances.
       */
      static sptr make(int VendorID=0x0403, int ProductID=0x6014, unsigned int BufferLength=52428800);
    };

  } // namespace FTxxxRead
} // namespace gr

#endif /* INCLUDED_FTXXXREAD_FTDI_READ_SOURCE_H */

